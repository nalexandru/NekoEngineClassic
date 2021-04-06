#include <xtl.h>
#include <stdio.h>
#include <stdint.h>

#include <Engine/Config.h>

#include "../Win32/Win32Compat.h"

static inline void __cpuid(int dst[4], int code);
static inline uint64_t __rdtsc(void);

#if _MSC_VER < 1400 // This *will* break
#	define _aligned_malloc(x, y) malloc(x)
#	define _aligned_free(x) free(x)
#endif

#include <System/System.h>
#include <Engine/Engine.h>

static int32_t _cpuFreq = 0;
static char _xboxKernelVersion[64];

static inline void _CalcCPUFreq(void);

/*
 * Xbox kernel version info & frequency detection
 * taken from Xbmc4Xbox
 */
struct _XBOX_KRNL_VERSION
{
	WORD VersionMajor;
	WORD VersionMinor;
	WORD Build;
	WORD Qfe;
};
extern struct _XBOX_KRNL_VERSION *XboxKrnlVersion;

bool
Sys_InitDbgOut(void)
{
	return true;
}

void
Sys_DbgOut(int color, const char *module, const char *severity, const char *text)
{
	FILE *fp = fopen("D:\\NE_DbgOut.txt", "a");
	if (!fp)
		return;
	fprintf(fp, "[%s][%s]: %s\n", module, severity, text);
	fclose(fp);
}

void
Sys_TermDbgOut(void)
{

}

uint64_t
Sys_Time(void)
{
	LARGE_INTEGER freq, ctr;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&ctr);

	return ((ctr.QuadPart / freq.QuadPart) * 1000000000UL) + ((ctr.QuadPart % freq.QuadPart) * 1000000000UL / freq.QuadPart);

	return 0;
}

bool
Sys_MapFile(const char *path, bool write, void **ptr, uint64_t *size)
{
	return false;
}

void
Sys_UnmapFile(const void *ptr, uint64_t size)
{
	(void)ptr;
	(void)size;
}

const char *
Sys_Hostname(void)
{
/*	DWORD size = sizeof(_hostname);

	if (!_hostname[0])
		GetComputerNameA(_hostname, &size);

	return _hostname;*/
	return "Xbox";
}

const char *
Sys_Machine(void)
{
	return "x86";
}

const char *
Sys_CpuName(void)
{
	return "Xbox CPU";
}

int32_t
Sys_CpuFreq(void)
{
	return _cpuFreq;
}

int32_t
Sys_NumCpus(void)
{
	return 1;
}

uint64_t
Sys_TotalMemory(void)
{
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	return memStatus.dwTotalPhys;
}

uint64_t
Sys_FreeMemory(void)
{
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	return memStatus.dwAvailPhys;
}

const char *
Sys_OperatingSystem(void)
{
	return "Xbox Kernel";
}

const char *
Sys_OperatingSystemVersion(void)
{
	return _xboxKernelVersion;
}

enum MachineType
Sys_MachineType(void)
{
	return MT_XBOX;
}

uint32_t
Sys_Capabilities(void)
{
	return 0;
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

	MessageBoxA((HWND)E_screen, message, title, type);*/

	(void)title;
	(void)message;
	(void)icon;
}

bool
Sys_ProcessEvents(void)
{
	return true;
}

void *
Sys_LoadLibrary(const char *path)
{
	(void)path;

	return NULL;
}

void *
Sys_GetProcAddress(void *lib, const char *name)
{
	(void)lib;
	(void)name;

	return NULL;
}

void
Sys_UnloadLibrary(void *lib)
{
	(void)lib;

	NULL;
}

bool
Sys_InitPlatform(void)
{
	DWORD videoFlags;
	FILE *fp = NULL;
	_CalcCPUFreq();

	k32_ConditionVariableSize = sizeof(struct Win32CompatCV);
	k32_InitializeConditionVariable = win32Compat_InitializeConditionVariable;
	k32_SleepConditionVariableSRW = win32Compat_SleepConditionVariableSRW;
	k32_SleepConditionVariableCS = win32Compat_SleepConditionVariableCS;
	k32_WakeAllConditionVariable = win32Compat_WakeAllConditionVariable;
	k32_WakeConditionVariable = win32Compat_WakeConditionVariable;
	k32_DeleteConditionVariable = win32Compat_DeleteConditionVariable;

	snprintf(_xboxKernelVersion, sizeof(_xboxKernelVersion), "%u.%u.%u.%u",
		XboxKrnlVersion->VersionMajor, XboxKrnlVersion->VersionMinor,
		XboxKrnlVersion->Build, XboxKrnlVersion->Qfe);

	videoFlags = XGetVideoFlags();

fp = fopen("D:\\vid_dbgout.txt", "a");
fprintf(fp, "videoFlags: 0x%x\n", videoFlags);
fclose(fp);

	if (videoFlags & XC_VIDEO_FLAGS_HDTV_720p) {
	fp = fopen("D:\\vid_dbgout.txt", "a");
fprintf(fp, "720p\n");
fclose(fp);
	} else if (videoFlags & XC_VIDEO_FLAGS_HDTV_1080i) {
fp = fopen("D:\\vid_dbgout.txt", "a");
fprintf(fp, "1080i\n");
fclose(fp);
	} else if (videoFlags & XC_VIDEO_FLAGS_HDTV_480p) {
fp = fopen("D:\\vid_dbgout.txt", "a");
fprintf(fp, "480p\n");
fclose(fp);
	} else if (videoFlags & XC_VIDEO_FLAGS_WIDESCREEN) {
fp = fopen("D:\\vid_dbgout.txt", "a");
fprintf(fp, "widescreen\n");
fclose(fp);
	} else if (videoFlags & XC_VIDEO_FLAGS_LETTERBOX) {
fp = fopen("D:\\vid_dbgout.txt", "a");
fprintf(fp, "letterbox\n");
fclose(fp);
	} else if (videoFlags & XC_VIDEO_FLAGS_PAL_60Hz) {
fp = fopen("D:\\vid_dbgout.txt", "a");
fprintf(fp, "pal 60\n");
fclose(fp);
	}
		
	E_SetCVarU32("Engine_ScreenWidth", 640);
	E_SetCVarU32("Engine_ScreenHeight", 480);

	return true;
}

void
Sys_TermPlatform(void)
{
}

void *
Sys_AlignedAlloc(size_t size, size_t alignment)
{
	return _aligned_malloc(size, alignment);
}

void
Sys_AlignedFree(void *mem)
{
	_aligned_free(mem);
}

void
Sys_ZeroMemory(void *mem, size_t size)
{
#if WINVER >= 0x0501
	SecureZeroMemory(mem, size);
#else
	// This branch is taken only when compiling with an SDK older than Windows XP / Server 2003,
	// because SecureZeroMemory is inlined. See:
	// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/aa366877(v=vs.85)
	ZeroMemory(mem, size);
#endif
}

void
_CalcCPUFreq(void)
{
	DWORD winFSB, winRes;
	double cpuFSB, cpuRes, freq;

	cpuFSB = (double)__rdtsc();
	winFSB = GetTickCount();

	Sleep(300);

	cpuRes = (double)__rdtsc();
	winRes = GetTickCount();

	freq = cpuRes - cpuFSB;
	freq /= winRes - winFSB;

	_cpuFreq = (uint32_t)(freq / 1000);
}

void
__cpuid(int dst[4], int code)
{
	__asm {
		mov eax, code;
		cpuid;
		mov dst[0], eax
		mov dst[4], ebx
		mov dst[8], ecx
		mov dst[12], edx
	}
}

uint64_t
__rdtsc(void)
{
	unsigned long a, b;
	uint64_t ret;

	__asm {
		RDTSC
		mov [a], eax
		mov [b], edx
	}

	ret = b;
	ret *= 0x100000000;
	ret += a;

	return ret;
}

