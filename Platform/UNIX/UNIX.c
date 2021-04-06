#include <stdio.h>

#include <time.h>
#include <sched.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/utsname.h>

#include <Input/Input.h>
#include <System/System.h>
#include <System/Memory.h>
#include <Engine/Engine.h>

#include "UNIXPlatform.h"

#if (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600)
#	define	USE_POSIX_MEMALIGN
#elif (defined(__GLIBC__) && ((__GLIBC__ >= 2 && __GLIBC_MINOR__ >= 8) || __GLIBC__ > 2) && defined(__LP64__)) \
		|| (defined(__FreeBSD__) && !defined(__arm__) && !defined(__mips__)) \
		|| defined(__APPLE__)
#	define	USE_MALLOC
#elif __STDC_VERSION__ >= 201112L
#	define USE_ALIGNED_ALLOC
#elif __SSE__
#	include <intrin.h>
#	define USE_MM_MALLLOC
#else
#	define USE_MEMALIGN
#endif

Display *X11_display;
XVisualInfo X11_visualInfo;
Atom X11_WM_PROTOCOLS, X11_WM_DELETE_WINDOW, X11_NET_WM_STATE, X11_NET_WM_PID,
	X11_NET_WM_WINDOW_TYPE, X11_NET_WM_WINDOW_TYPE_NORMAL, X11_NET_WM_BYPASS_COMPOSITOR;

static int _numCpus = 0, _cpuFreq = 0;
static char _cpuName[128] = "Unknown";
static char _colors[4][8] =
{
	"\x1B[35m\0",
	"\x1B[0m\0",
	"\x1B[33m\0",
	"\x1B[31m\0"
};
static struct utsname _uname;

static inline void _CpuInfo(void);

bool
Sys_InitDbgOut(void)
{
	return true;
}

void
Sys_DbgOut(int color, const char *module, const char *severity, const char *text)
{
	fprintf(stderr, "%s[%s][%s]: %s\x1B[0m\n", _colors[color], module, severity, text);
}

void
Sys_TermDbgOut(void)
{
}

uint64_t
Sys_Time(void)
{
#ifdef CLOCK_MONOTONIC
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
 
    return (uint64_t)tp.tv_sec * (uint64_t)1000000000 + (uint64_t)tp.tv_nsec;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
 
    return (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec;
#endif
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
	if (!_uname.sysname[0])
		uname(&_uname);

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

int
Sys_NumCpus(void)
{
	return _numCpus;
}

uint64_t
Sys_TotalMemory(void)
{
#if defined(_SC_PHYS_PAGES)
	uint64_t pages = sysconf(_SC_PHYS_PAGES);
	uint64_t page_size = sysconf(_SC_PAGE_SIZE);
	return pages * page_size;
#elif defined(HW_PHYSMEM)
	uint64_t mem = 0;
	size_t len = sizeof(mem);
#if defined(SYS_PLATFORM_APPLE) && defined(HW_MEMSIZE)
	static int mib[2] = { CTL_HW, HW_MEMSIZE };
#else
	static int mib[2] = { CTL_HW, HW_PHYSMEM };
#endif
	
	if (sysctl(mib, 2, &mem, &len, NULL, 0) < 0)
		return 0;
	
	return mem;
#elif defined(SYS_PLATFORM_IRIX)
	struct rminfo rmi;
	
	sysmp(MP_KERNADDR, MPSA_RMINFO, &rmi);
	
	return rmi.physmem;
#else
#	error FATAL: sys_mem_total NOT IMPLEMENTED FOR THIS PLATFORM
	return 0;
#endif
}

uint64_t
Sys_FreeMemory(void)
{
#if defined(_SC_AVPHYS_PAGES)
	uint64_t pages = sysconf(_SC_AVPHYS_PAGES);
	uint64_t page_size = sysconf(_SC_PAGE_SIZE);
	return pages * page_size;
#elif defined(HW_USERMEM)
	uint64_t mem = 0;
	size_t len = sizeof(mem);
	int mib[2] = { CTL_HW, HW_USERMEM };
	
	if (sysctl(mib, 2, &mem, &len, NULL, 0) < 0)
		return 0;
	
	return mem;
#elif defined(SYS_PLATFORM_IRIX)
	struct rminfo rmi;
	
	sysmp(MP_KERNADDR, MPSA_RMINFO, &rmi);
	
	return rmi.freemem;
#else
#	error FATAL: sys_mem_free NOT IMPLEMENTED FOR THIS PLATFORM
	return 0;
#endif
}

const char *
Sys_OperatingSystem(void)
{
	if (!_uname.sysname[0])
		uname(&_uname);
	
	return _uname.sysname;
}

const char *
Sys_OperatingSystemVersion(void)
{
	if (!_uname.sysname[0])
		uname(&_uname);
	
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

void
Sys_MessageBox(const char *title, const char *message, int icon)
{
/*	UINT type = MB_OK;

	switch (icon) {
		case MSG_ICON_NONE:
			type = 0;
			break;
		case MSG_ICON_INFO:
			type |= MB_ICONINFORMATION;
			break;
		case MSG_ICON_WARN:
			type |= MB_ICONWARNING;
			break;
		case MSG_ICON_ERROR:
			type |= MB_ICONERROR;
			break;
	}

	MessageBox((HWND)E_Screen, message, title, type);*/
}

bool
Sys_ProcessEvents(void)
{
	XEvent ev, nev;
	while (XPending(X11_display)) {
		XNextEvent(X11_display, &ev);
		
		switch (ev.type) {
		case KeyPress: {
			In_buttonState[X11_keymap[ev.xkey.keycode]] = true;
		} break;
		case KeyRelease: {
			if (XEventsQueued(X11_display, QueuedAfterReading)) {
				XPeekEvent(X11_display, &nev);
				
				if (nev.type == KeyPress &&
						nev.xkey.time == ev.xkey.time &&
						nev.xkey.keycode == ev.xkey.keycode)
					break;
			}
			
			In_buttonState[X11_keymap[ev.xkey.keycode]] = false;
		} break;
		case ButtonPress:
		case ButtonRelease: {
			switch (ev.xbutton.button) {
			case Button1: In_buttonState[BTN_MOUSE_LMB] = ev.type == ButtonPress;
			case Button2: In_buttonState[BTN_MOUSE_RMB] = ev.type == ButtonPress;
			case Button3: In_buttonState[BTN_MOUSE_MMB] = ev.type == ButtonPress;
			case Button4: In_buttonState[BTN_MOUSE_BTN4] = ev.type == ButtonPress;
			case Button5: In_buttonState[BTN_MOUSE_BTN5] = ev.type == ButtonPress;
			}
		} break;
		case ConfigureNotify: {
			XConfigureEvent ce = ev.xconfigure;
			
			if (ce.width == *E_screenWidth && ce.height == *E_screenHeight)
				break;
			
			*E_screenWidth = ce.width;
			*E_screenHeight = ce.height;
			
			//ScreenResized
		} break;
		case ClientMessage: {
			if (ev.xclient.data.l[0] == X11_WM_DELETE_WINDOW)
				return false;
		} break;
		case DestroyNotify: {
			return false;
		} break;
		default: {
		} break;
		}
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
	
		if (len < 4 || strncmp(name + len - 3, ".so", 3))
			suffix = ".so";
		
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
#ifdef SYS_PLATFORM_IRIX
	_numCpus = sysconf(_SC_NPROC_ONLN);
#else
	#ifndef _SC_NPROCESSORS_ONLN
		_numCpus = sysconf(HW_NCPU);
	#else
		_numCpus = sysconf(_SC_NPROCESSORS_ONLN);
	#endif
#endif

	X11_display = XOpenDisplay(NULL);
	if (!X11_display)
		return false;
	
	int screen = XDefaultScreen(X11_display);
	if (!XMatchVisualInfo(X11_display, screen, 24, TrueColor, &X11_visualInfo))
		return false;
	
	X11_NET_WM_STATE = XInternAtom(X11_display, "_NET_WM_STATE", False);
	X11_NET_WM_PID = XInternAtom(X11_display, "_NET_WM_PID", False);
	X11_NET_WM_WINDOW_TYPE = XInternAtom(X11_display, "_NET_WM_WINDOW_TYPE", False);
	X11_NET_WM_WINDOW_TYPE_NORMAL = XInternAtom(X11_display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
	X11_NET_WM_BYPASS_COMPOSITOR = XInternAtom(X11_display, "_NET_WM_BYPASS_COMPOSITOR", False);
	X11_WM_PROTOCOLS = XInternAtom(X11_display, "WM_PROTOCOLS", False);
	X11_WM_DELETE_WINDOW = XInternAtom(X11_display, "WM_DELETE_WINDOW", False);

	_CpuInfo();

	return true;
}

void
Sys_TermPlatform(void)
{
	XCloseDisplay(X11_display);
}

void *
Sys_AlignedAlloc(size_t size, size_t alignment)
{
	#if defined(USE_POSIX_MEMALIGN)
	void *mem;
	if (posix_memalign(&mem, alignment, size))
		return NULL;
	return mem;
	#elif defined(USE_MEMALIGN)
	return memalign(alignment, size);
	#elif defined(USE_MALLOC)
	return malloc(size);
	#elif defined(USE_ALIGNED_ALLOC)
	return aligned_alloc(alignment, size);
	#elif defined(USE_MM_MALLOC)
	return _mm_malloc(size, alignment);
	#else
	#error	Aligned memory allocation not implemented for this platform
	#endif
}

void
Sys_AlignedFree(void *mem)
{
	#if defined(USE_MALLOC) || defined(USE_ALIGNED_ALLOC) || defined(USE_MEMALIGN) || defined(USE_POSIX_MEMALIGN)
	return free(mem);
	#elif defined(USE_MM_MALLOC)
	return _mm_free(mem);
	#else
	#error	Aligned memory allocation not implemented for this platform
	#endif
}

void
Sys_ZeroMemory(void *mem, size_t size)
{
	memset(mem, 0x0, size);
}

void
_CpuInfo(void)
{
#if defined(__linux__)
	char buff[512];
	char *cpuNameId = NULL, *cpuFreqId = NULL;
//	int cpuFreqDiv = 1;
	FILE *fp = fopen("/proc/cpuinfo", "r");
 
#	if defined(__i386__) || defined(__amd64__) || defined(__x86_64__)
		cpuNameId = "model name";
		cpuFreqId = "cpu MHz";
#	elif defined(SYS_ARCH_PPC) || defined(SYS_ARCH_PPC64)
		cpuNameId = "cpu";
		cpuFreqId = "clock";
#	elif defined(SYS_ARCH_ARM) || defined(SYS_ARCH_ARM64)
		cpuNameId = "model name";
		cpuFreqId = "clock";
#	elif defined(SYS_ARCH_MIPS) || defined(SYS_ARCH_MIPS64)
		cpuNameId = "cpu model";
		cpuFreqId = "clock";
#	elif defined(SYS_ARCH_SPARC) || defined(SYS_ARCH_SPARC64)
		cpuNameId = "cpu";
		cpuFreqId = "clock";
#	elif defined(SYS_ARCH_HPPA)
		cpuNameId = "cpu";
		cpuFreqId = "cpu MHz";
#	elif defined(SYS_ARCH_ALPHA)
		cpuNameId = "cpu model";
		cpuFreqId = "cycle frequency [Hz]";
//		cpuFreqDiv = 1000;
#	else
#		warning "Cpu info not implemented for this architecture (Platform/UNIX/UNIX.c)"
#	endif

	if (!cpuNameId || !cpuFreqId)
		return;
 
	_cpuFreq = 0;
	memset(buff, 0x0, 512);
 
	if (fp) {
		while (fgets(buff, 512, fp)) {
			if (strstr(buff, cpuNameId)) {
				char *ptr = strchr(buff, ':');
				snprintf(_cpuName, sizeof(_cpuName), "%s", ptr ? ptr + 2 : "Unknown");
			} else if (strstr(buff, cpuFreqId)) {
				char *ptr = strchr(buff, ':');
				if (ptr) {
					ptr += 2;
					ptr[strlen(ptr) - 1] = 0x0;
					_cpuFreq = atoi(ptr);
				}
				break;
			}
			memset(buff, 0x0, 512);
		}
		fclose(fp);
	}
#else
#	warning "Cpu info not implemented for this platform (Platform/UNIX/UNIX.c)"
#endif

	if (_cpuName[strlen(_cpuName) - 1] == '\n')
		_cpuName[strlen(_cpuName) - 1] = 0x0;
}
