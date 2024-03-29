#include <stdio.h>
#include <stdint.h>

#include <sched.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/sysctl.h>
#include <sys/utsname.h>

#include <mach/mach.h>
#include <mach/mach_time.h>
#include <mach/host_info.h>
#include <mach/processor_info.h>

#define Handle __EngineHandle

#include <Input/Input.h>
#include <System/System.h>
#include <System/Memory.h>
#include <Engine/Engine.h>
#include <Engine/Config.h>

#include "MacXPlatform.h"

#undef Handle
#import <Cocoa/Cocoa.h>

#include "EngineAppDelegate.h"

#define CPU_NAME_LEN	128

static uint64_t _machTimerFreq;
static natural_t _numCpus = 0;
static int32_t _cpuFreq;
static struct utsname _uname;
static char _cpuName[CPU_NAME_LEN];

static inline void _CpuInfo(void);

bool
Sys_InitDbgOut(void)
{
	return true;
}

void
Sys_DbgOut(int color, const char *module, const char *severity, const char *text)
{	
	NSLog(@"[%s][%s]: %s\n", module, severity, text);
}

void
Sys_TermDbgOut(void)
{
}

uint64_t
Sys_Time(void)
{
	return mach_absolute_time() * _machTimerFreq;
}

bool
Sys_MapFile(const char *path, bool write, void **ptr, uint64_t *size)
{
	FILE *fp = NULL;
	int fd, p = 0, f = 0, mode = 0;

	fp = fopen(path, "r");
	if (!fp)
		return false;

	fseek(fp, 0, SEEK_END);
	*size = ftell(fp);
	fclose(fp);

	if (write) {
		p = PROT_READ | PROT_WRITE;
		mode = O_RDWR;
		f = MAP_PRIVATE;
	} else {
		p = PROT_READ;
		mode = O_RDONLY;
		f = MAP_SHARED;
	}

	fd = open(path, mode);
	if (!fd)
		return false;

	*ptr = mmap(0, *size, p, f, fd, 0);

	close(fd);

	return true;
}

void
Sys_UnmapFile(const void *ptr, uint64_t size)
{
	munmap((void *)ptr, size);
}

const char *
Sys_Hostname(void)
{
	if (!_uname.sysname[0])
		uname(&_uname);

	return _uname.nodename;
}

const char *
Sys_Machine(void)
{
	return _uname.machine;
}

const char *
Sys_CpuName(void)
{
	return _cpuName;
}

int32_t
Sys_CpuFreq(void)
{
	return _cpuFreq;
}

int32_t
Sys_NumCpus(void)
{
	return _numCpus;
}

uint64_t
Sys_TotalMemory(void)
{
	uint64_t mem = 0;
	size_t len = sizeof(mem);
	
#if defined(HW_MEMSIZE)
	static int mib[2] = { CTL_HW, HW_MEMSIZE };
#else
	static int mib[2] = { CTL_HW, HW_PHYSMEM };
#endif
	
	if (sysctl(mib, 2, &mem, &len, NULL, 0) < 0)
		return 0;
	
	return mem;
}

uint64_t
Sys_FreeMemory(void)
{
	uint64_t mem = 0;
	size_t len = sizeof(mem);
	int mib[2] = { CTL_HW, HW_USERMEM };
	
	if (sysctl(mib, 2, &mem, &len, NULL, 0) < 0)
		return 0;
	
	return mem;
}

const char *
Sys_OperatingSystem(void)
{
	return "Mac OS X";
}

const char *
Sys_OperatingSystemVersion(void)
{
	return _uname.release;
}

enum MachineType
Sys_MachineType(void)
{
	return MT_PC;
}

uint32_t
Sys_Capabilities(void)
{
	return SC_MMIO;
}

bool
Sys_ScreenVisible(void)
{
	return true;
}

bool
Sys_UniversalWindows(void)
{
	return false;
}

void
Sys_MessageBox(const char *title, const char *message, int icon)
{	
	NSAlert *a = [[NSAlert alloc] init];
	[a addButtonWithTitle:@"OK"];
	[a setMessageText:[NSString stringWithFormat:@"%s", message]];
	
	switch (icon) {
	case MSG_ICON_NONE: break;
	case MSG_ICON_INFO: [a setAlertStyle: NSInformationalAlertStyle]; break;
	case MSG_ICON_WARN: [a setAlertStyle: NSWarningAlertStyle]; break;
	case MSG_ICON_ERROR: [a setAlertStyle: NSCriticalAlertStyle]; break;
	}
	
	[a runModal];
	[a release];
}

bool
Sys_ProcessEvents(void)
{
	while (1) {
		NSEvent *e = [NSApp nextEventMatchingMask: NSAnyEventMask
							untilDate: [NSDate distantPast]
							inMode: NSDefaultRunLoopMode
							dequeue: true];
							
		if (!e)
			break;
				
		[NSApp sendEvent: e];
	}

	return true;
}

void *
Sys_LoadLibrary(const char *name)
{
	char *path = NULL;
	
	if (!name)
		return dlopen(NULL, RTLD_NOW);
	
	if (access(name, R_OK) < 0) {
		char *prefix = "", *suffix = "";
		size_t len = strlen(name);
		
		path = Sys_Alloc(sizeof(char), 2048, MH_Transient);
		if (!path)
			return NULL;
		
		if (!strchr(name, '/') && strncmp(name, "lib", 3))
			prefix = "lib";
	
		if (len < 7 || strncmp(name + len - 6, ".dylib", 6))
			suffix = ".dylib";
		
		snprintf(path, 2048, "%s%s%s", prefix, name, suffix);
	} else {
		path = (char *)name;
	}
	
	return dlopen(path, RTLD_NOW);
}

void *
Sys_GetProcAddress(void *lib, const char *name)
{
	if (!lib || !name)
		return NULL;
	return dlsym(lib, name);
}

void
Sys_UnloadLibrary(void *lib)
{
	if (lib)
		dlclose(lib);
}

bool
Sys_InitPlatform(void)
{
	mach_timebase_info_data_t tb;
	mach_timebase_info(&tb);
	
	_machTimerFreq = tb.numer / tb.denom;
	
	NSApplicationLoad();
	[NSApplication sharedApplication];
	[NSApp finishLaunching];
	
#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
#endif
	
	EngineAppDelegate *d = [[EngineAppDelegate alloc] init];
	[NSApp setDelegate: d];
	[[NSApplication sharedApplication] setDelegate: d];

	char dataDir[2048];
	getcwd(dataDir, 2048);
	
	if (dataDir[strlen(dataDir) - 1] != '/')
		strncat(dataDir, "/", 1);
	
	strncat(dataDir, "/Data", 5);
	
	E_SetCVarStr("Engine_DataDir", dataDir);
	
	_CpuInfo();
	
	uname(&_uname);
	
	snprintf(dataDir, sizeof(_uname.release), "%s (%s %s)",
		[[[NSProcessInfo processInfo] operatingSystemVersionString] UTF8String],
		_uname.sysname, _uname.release);
	snprintf(_uname.release, sizeof(_uname.release), "%s", dataDir);
	
	return true;
}

void
Sys_TermPlatform(void)
{
}

void
_CpuInfo(void)
{	
	int null;
	FILE *fp = NULL;
	char buff[512], *p = buff;
	bool ghz = false;
	float f = 0.f;
	static int sout, serr;
	
	// slience stdout, stderr
	fflush(stdout);
	fflush(stderr);
	
	null = open("/dev/null", O_WRONLY);
	
	sout = dup(STDOUT_FILENO);
	dup2(null, STDOUT_FILENO);
	
	serr = dup(STDERR_FILENO);
	dup2(null, STDERR_FILENO);
	
	close(null);
	
	processor_basic_info_t cpuInfo;
	mach_msg_type_number_t msgCount;
	
	host_processor_info(mach_host_self(), PROCESSOR_BASIC_INFO, &_numCpus,
						(processor_info_array_t *)&cpuInfo, &msgCount);
	
	fp = popen("/usr/sbin/sysctl -n machdep.cpu.brand_string", "r");
	if (fgets(buff, sizeof(buff), fp) != NULL) { // Mac OS X 10.6+
		buff[strlen(buff) - 1] = 0x0;
		snprintf(_cpuName, sizeof(_cpuName), "%s", buff);
		
		pclose(fp);
		fp = popen("system_profiler SPHardwareDataType | grep 'Processor Speed' | cut -c 24-", "r");
		fgets(buff, sizeof(buff), fp);
	} else {
		pclose(fp);
		fp = popen("system_profiler SPHardwareDataType | grep 'Processor Name' | cut -c 23-", "r");
		if (fgets(buff, sizeof(buff), fp) != NULL) { // Mac OS X 10.5
			buff[strlen(buff) - 1] = 0x0;
			snprintf(_cpuName, sizeof(_cpuName), "%s", buff);
			
			pclose(fp);
			fp = popen("system_profiler SPHardwareDataType | grep 'Processor Speed' | cut -c 24-", "r");
			fgets(buff, sizeof(buff), fp);
		} else { // Mac OS X 10.4, perhaps earlier too but they're not supported. (yet)
			pclose(fp);
			fp = popen("system_profiler SPHardwareDataType | grep 'CPU Type' | cut -c 17-", "r");
			if (fgets(buff, sizeof(buff), fp) != NULL) {
				buff[strlen(buff) - 1] = 0x0;
				snprintf(_cpuName, sizeof(_cpuName), "%s", buff);
				
				pclose(fp);
				fp = popen("system_profiler SPHardwareDataType | grep 'CPU Speed' | cut -c 18-", "r");
				fgets(buff, sizeof(buff), fp);
			} else {
				snprintf(_cpuName, sizeof(_cpuName), "Unknown");
			}
		}
	}
	pclose(fp);
	
	while (*(p++)) {
		if (*p == ',')
			*p = '.';
		
		if (*p != ' ')
			continue;
			
		*p = 0x0;
		if (*(++p) == 'G')
			ghz = true;
			
		break;
	}
	
	f = atof(buff) * (ghz ? 1000.f : 1.f);
	_cpuFreq = (int32_t)f;
	
	// restore stdout, stderr
	fflush(stdout);
	dup2(sout, STDOUT_FILENO);
	
	fflush(stderr);
	dup2(serr, STDERR_FILENO);
	
	if (_cpuName[strlen(_cpuName) - 1] == '\n')
		_cpuName[strlen(_cpuName) - 1] = 0x0;
}

void *
Sys_AlignedAlloc(size_t size, size_t alignment)
{
	(void)alignment;
	return malloc(size); // malloc is aligned to 16 bytes
}

void
Sys_AlignedFree(void *mem)
{
	free(mem);
}

void
Sys_ZeroMemory(void *mem, size_t len)
{
	memset(mem, 0x0, len);
}
