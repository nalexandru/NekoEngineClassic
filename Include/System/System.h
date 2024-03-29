#ifndef _SYS_SYSTEM_H_
#define _SYS_SYSTEM_H_

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <Engine/Types.h>

enum MsgBoxIcon
{
	MSG_ICON_NONE = 0,
	MSG_ICON_INFO = 1,
	MSG_ICON_WARN = 2,
	MSG_ICON_ERROR = 3
};

enum MachineType
{
	MT_PC,
	MT_XBOX,
	MT_PS3,
	MT_WII,
	MT_XBOX_360
};

enum SystemCapabilityFlags
{
	SC_MMIO = 0x01
};

bool Sys_Init(void);
void Sys_Term(void);

bool Sys_InitDbgOut(void);
void Sys_DbgOut(int color, const char *module, const char *severity, const char *text);
void Sys_TermDbgOut(void);

uint64_t Sys_Time(void);

bool Sys_MapFile(const char *path, bool write, void **ptr, uint64_t *size);
void Sys_UnmapFile(const void *ptr, uint64_t size);

const char *Sys_Hostname(void);
const char *Sys_Machine(void);
const char *Sys_CpuName(void);
int32_t Sys_CpuFreq(void);
int32_t Sys_NumCpus(void);

uint64_t Sys_TotalMemory(void);
uint64_t Sys_FreeMemory(void);

const char *Sys_OperatingSystem(void);
const char *Sys_OperatingSystemVersion(void);

enum MachineType Sys_MachineType(void);
uint32_t Sys_Capabilities(void);

bool Sys_ScreenVisible(void);
void Sys_MessageBox(const char *title, const char *message, int icon);
bool Sys_ProcessEvents(void);

void *Sys_LoadLibrary(const char *path);
void *Sys_GetProcAddress(void *lib, const char *name);
void Sys_UnloadLibrary(void *lib);

// Compatibility shivs
void *reallocarray(void *ptr, size_t nmemb, size_t size);

int getopt(int nargc, char *const nargv[], const char *ostr);

extern int opterr, optind, optopt, optreset;
extern char *optarg;

#ifdef SNPRINTF_COMPAT
int snprintf(char *, size_t, const char *, /*args*/ ...);
#endif

#ifdef WCSDUP_COMPAT
static inline wchar_t *wcsdup(const wchar_t *str)
{
	size_t len = wcslen(str) * sizeof(*str);
	wchar_t *copy = (wchar_t *)calloc(1, len + sizeof(*str));
	memcpy(copy, str, len);
	return copy;
}
#endif

#ifdef STRTOULL_COMPAT
static inline uint64_t strtoull(const char *str, char **endptr, int base)
{
	return strtoul(str, endptr, base);
}
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1700)

#if !defined(_XBOX360)
#	define vswprintf _vsnwprintf
#	define swprintf _snwprintf
#endif

#define strtoll strtol

static inline float strtof(const char *str, const char **endptr)
{
	const char *end = str;
	bool dot = false;

	if (endptr) {
		for (end; *end; ++end) {
			if (*end > 0x2F && *end < 0x3A)	// 0x30 - 0x39 ASCII digits
				continue;

			if (!dot && *end == '.') {
				dot = true;
				continue;
			}

			break;
		}

		*endptr = end;
	}

	return (float)atof(str);
}
#endif

#endif /* _SYS_SYSTEM_H_ */
