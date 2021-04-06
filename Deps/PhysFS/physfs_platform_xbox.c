/*
 * Xbox support routines for PhysicsFS.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Alexandru Naiman based on physfs_platform_windows.c.
 */

#define __PHYSICSFS_INTERNAL__
#include "physfs_platforms.h"

#ifdef PHYSFS_PLATFORM_XBOX

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <xtl.h>

#include <errno.h>
#include <ctype.h>
#include <time.h>

#include "physfs_internal.h"

/*
 * Users without the platform SDK don't have this defined.  The original docs
 *  for SetFilePointer() just said to compare with 0xFFFFFFFF, so this should
 *  work as desired.
 */
#define PHYSFS_INVALID_SET_FILE_POINTER  0xFFFFFFFF

/* just in case... */
#define PHYSFS_INVALID_FILE_ATTRIBUTES   0xFFFFFFFF

/* Not defined before the Vista SDK. */
#define PHYSFS_FILE_ATTRIBUTE_REPARSE_POINT 0x400
#define PHYSFS_IO_REPARSE_TAG_SYMLINK    0xA000000C

/* Some older APIs aren't in WinRT (only the "Ex" version, etc).
   Since non-WinRT might not have the "Ex" version, we tapdance to use
   the perfectly-fine-and-available-even-on-Win95 API on non-WinRT targets. */

static BOOL winSetFilePointer(HANDLE h, const PHYSFS_sint64 pos,
                              PHYSFS_sint64 *_newpos, const DWORD whence)
{
    const LONG low = (LONG) (pos & 0xFFFFFFFF);
    LONG high = (LONG) ((pos >> 32) & 0xFFFFFFFF);
    const DWORD rc = SetFilePointer(h, low, &high, whence);
    /* 0xFFFFFFFF could be valid, so you have to check GetLastError too! */
    if (_newpos)
        *_newpos = ((PHYSFS_sint64) rc) | (((PHYSFS_sint64) high) << 32);
    if ((rc == PHYSFS_INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR))
        return FALSE;
    return TRUE;
} /* winSetFilePointer */

static PHYSFS_ErrorCode errcodeFromWinApiError(const DWORD err)
{
    /*
     * win32 error codes are sort of a tricky thing; Microsoft intentionally
     *  doesn't list which ones a given API might trigger, there are several
     *  with overlapping and unclear meanings...and there's 16 thousand of
     *  them in Windows 7. It looks like the ones we care about are in the
     *  first 500, but I can't say this list is perfect; we might miss
     *  important values or misinterpret others.
     *
     * Don't treat this list as anything other than a work in progress.
     */
    switch (err)
    {
        case ERROR_SUCCESS: return PHYSFS_ERR_OK;
        case ERROR_ACCESS_DENIED: return PHYSFS_ERR_PERMISSION;
        case ERROR_NETWORK_ACCESS_DENIED: return PHYSFS_ERR_PERMISSION;
        case ERROR_NOT_READY: return PHYSFS_ERR_IO;
        case ERROR_CRC: return PHYSFS_ERR_IO;
        case ERROR_SEEK: return PHYSFS_ERR_IO;
        case ERROR_SECTOR_NOT_FOUND: return PHYSFS_ERR_IO;
        case ERROR_NOT_DOS_DISK: return PHYSFS_ERR_IO;
        case ERROR_WRITE_FAULT: return PHYSFS_ERR_IO;
        case ERROR_READ_FAULT: return PHYSFS_ERR_IO;
        case ERROR_DEV_NOT_EXIST: return PHYSFS_ERR_IO;
        case ERROR_BUFFER_OVERFLOW: return PHYSFS_ERR_BAD_FILENAME;
        case ERROR_INVALID_NAME: return PHYSFS_ERR_BAD_FILENAME;
        case ERROR_BAD_PATHNAME: return PHYSFS_ERR_BAD_FILENAME;
        case ERROR_DIRECTORY: return PHYSFS_ERR_BAD_FILENAME;
        case ERROR_FILE_NOT_FOUND: return PHYSFS_ERR_NOT_FOUND;
        case ERROR_PATH_NOT_FOUND: return PHYSFS_ERR_NOT_FOUND;
        case ERROR_INVALID_DRIVE: return PHYSFS_ERR_NOT_FOUND;
        case ERROR_HANDLE_DISK_FULL: return PHYSFS_ERR_NO_SPACE;
        case ERROR_DISK_FULL: return PHYSFS_ERR_NO_SPACE;
        case ERROR_WRITE_PROTECT: return PHYSFS_ERR_READ_ONLY;
        case ERROR_LOCK_VIOLATION: return PHYSFS_ERR_BUSY;
        case ERROR_SHARING_VIOLATION: return PHYSFS_ERR_BUSY;
        case ERROR_CURRENT_DIRECTORY: return PHYSFS_ERR_BUSY;
        case ERROR_DRIVE_LOCKED: return PHYSFS_ERR_BUSY;
        case ERROR_PATH_BUSY: return PHYSFS_ERR_BUSY;
        case ERROR_BUSY: return PHYSFS_ERR_BUSY;
        case ERROR_NOT_ENOUGH_MEMORY: return PHYSFS_ERR_OUT_OF_MEMORY;
        case ERROR_OUTOFMEMORY: return PHYSFS_ERR_OUT_OF_MEMORY;
        case ERROR_DIR_NOT_EMPTY: return PHYSFS_ERR_DIR_NOT_EMPTY;
        default: return PHYSFS_ERR_OS_ERROR;
    } /* switch */
} /* errcodeFromWinApiError */

static inline PHYSFS_ErrorCode errcodeFromWinApi(void)
{
    return errcodeFromWinApiError(GetLastError());
} /* errcodeFromWinApi */

void __PHYSFS_platformDetectAvailableCDs(PHYSFS_StringCallback cb, void *data)
{
    //detectAvailableCDs(cb, data);
} /* __PHYSFS_platformDetectAvailableCDs */

char *__PHYSFS_platformCalcBaseDir(const char *argv0)
{
	char *retval = allocator.Malloc(4);
    snprintf(retval, 4, "D:\\");
    return retval;
} /* __PHYSFS_platformCalcBaseDir */


char *__PHYSFS_platformCalcPrefDir(const char *org, const char *app)
{
	char *retval = allocator.Malloc(256);
    snprintf(retval, 256, "D:\\AppData\\%s\\", app);

    return retval;
} /* __PHYSFS_platformCalcPrefDir */


char *__PHYSFS_platformCalcUserDir(void)
{
    char *retval = allocator.Malloc(4);
    snprintf(retval, 4, "D:\\");

    return retval;
} /* __PHYSFS_platformCalcUserDir */


int __PHYSFS_platformInit(void)
{
    return 1;  /* It's all good */
} /* __PHYSFS_platformInit */


void __PHYSFS_platformDeinit(void)
{
} /* __PHYSFS_platformDeinit */


void *__PHYSFS_platformGetThreadID(void)
{
    return ( (void *) ((size_t) GetCurrentThreadId()) );
} /* __PHYSFS_platformGetThreadID */


PHYSFS_EnumerateCallbackResult __PHYSFS_platformEnumerate(const char *dirname,
                               PHYSFS_EnumerateCallback callback,
                               const char *origdir, void *callbackdata)
{
    PHYSFS_EnumerateCallbackResult retval = PHYSFS_ENUM_OK;
    HANDLE dir = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ent;
    size_t len = strlen(dirname);
    char *searchPath = NULL;

    /* Allocate a new string for path, maybe '\\', "*", and NULL terminator */
    searchPath = (char *) __PHYSFS_smallAlloc(len + 3);
    BAIL_IF(!searchPath, PHYSFS_ERR_OUT_OF_MEMORY, PHYSFS_ENUM_ERROR);

    /* Copy current dirname */
    strcpy(searchPath, dirname);

    /* if there's no '\\' at the end of the path, stick one in there. */
    if (searchPath[len - 1] != '\\')
    {
        searchPath[len++] = '\\';
        searchPath[len] = '\0';
    } /* if */

    /* Append the "*" to the end of the string */
    strcat(searchPath, "*");

    dir = FindFirstFile(searchPath, &ent);
    __PHYSFS_smallFree(searchPath);
    BAIL_IF(dir==INVALID_HANDLE_VALUE, errcodeFromWinApi(), PHYSFS_ENUM_ERROR);

    do
    {
        const CHAR *fn = ent.cFileName;

        if (fn[0] == '.')  /* ignore "." and ".." */
        {
            if ((fn[1] == '\0') || ((fn[1] == '.') && (fn[2] == '\0')))
                continue;
        } /* if */

        retval = callback(callbackdata, origdir, fn);
        if (retval == PHYSFS_ENUM_ERROR)
            PHYSFS_setErrorCode(PHYSFS_ERR_APP_CALLBACK);
    } while ((retval == PHYSFS_ENUM_OK) && (FindNextFile(dir, &ent) != 0));

    FindClose(dir);

    return retval;
} /* __PHYSFS_platformEnumerate */


int __PHYSFS_platformMkDir(const char *path)
{
    const DWORD rc = CreateDirectory(path, NULL);
    BAIL_IF(rc == 0, errcodeFromWinApi(), 0);
    return 1;
} /* __PHYSFS_platformMkDir */


static HANDLE doOpen(const char *fname, DWORD mode, DWORD creation)
{
    HANDLE fileh;
	const DWORD share = FILE_SHARE_READ | FILE_SHARE_WRITE;

    fileh = CreateFile(fname, mode, share, NULL, creation,
                       FILE_ATTRIBUTE_NORMAL, NULL);

    if (fileh == INVALID_HANDLE_VALUE)
        BAIL(errcodeFromWinApi(), INVALID_HANDLE_VALUE);

    return fileh;
} /* doOpen */


void *__PHYSFS_platformOpenRead(const char *filename)
{
    HANDLE h = doOpen(filename, GENERIC_READ, OPEN_EXISTING);
    return (h == INVALID_HANDLE_VALUE) ? NULL : (void *) h;
} /* __PHYSFS_platformOpenRead */


void *__PHYSFS_platformOpenWrite(const char *filename)
{
    HANDLE h = doOpen(filename, GENERIC_WRITE, CREATE_ALWAYS);
    return (h == INVALID_HANDLE_VALUE) ? NULL : (void *) h;
} /* __PHYSFS_platformOpenWrite */


void *__PHYSFS_platformOpenAppend(const char *filename)
{
    HANDLE h = doOpen(filename, GENERIC_WRITE, OPEN_ALWAYS);
    BAIL_IF_ERRPASS(h == INVALID_HANDLE_VALUE, NULL);

    if (!winSetFilePointer(h, 0, NULL, FILE_END))
    {
        const PHYSFS_ErrorCode err = errcodeFromWinApi();
        CloseHandle(h);
        BAIL(err, NULL);
    } /* if */

    return (void *) h;
} /* __PHYSFS_platformOpenAppend */


PHYSFS_sint64 __PHYSFS_platformRead(void *opaque, void *buf, PHYSFS_uint64 len)
{
    HANDLE h = (HANDLE) opaque;
    PHYSFS_sint64 totalRead = 0;

    if (!__PHYSFS_ui64FitsAddressSpace(len))
        BAIL(PHYSFS_ERR_INVALID_ARGUMENT, -1);

    while (len > 0)
    {
        const DWORD thislen = (len > 0xFFFFFFFF) ? 0xFFFFFFFF : (DWORD) len;
        DWORD numRead = 0;
        if (!ReadFile(h, buf, thislen, &numRead, NULL))
            BAIL(errcodeFromWinApi(), -1);
        len -= (PHYSFS_uint64) numRead;
        totalRead += (PHYSFS_sint64) numRead;
        if (numRead != thislen)
            break;
    } /* while */

    return totalRead;
} /* __PHYSFS_platformRead */


PHYSFS_sint64 __PHYSFS_platformWrite(void *opaque, const void *buffer,
                                     PHYSFS_uint64 len)
{
    HANDLE h = (HANDLE) opaque;
    PHYSFS_sint64 totalWritten = 0;

    if (!__PHYSFS_ui64FitsAddressSpace(len))
        BAIL(PHYSFS_ERR_INVALID_ARGUMENT, -1);

    while (len > 0)
    {
        const DWORD thislen = (len > 0xFFFFFFFF) ? 0xFFFFFFFF : (DWORD) len;
        DWORD numWritten = 0;
        if (!WriteFile(h, buffer, thislen, &numWritten, NULL))
            BAIL(errcodeFromWinApi(), -1);
        len -= (PHYSFS_uint64) numWritten;
        totalWritten += (PHYSFS_sint64) numWritten;
        if (numWritten != thislen)
            break;
    } /* while */

    return totalWritten;
} /* __PHYSFS_platformWrite */


int __PHYSFS_platformSeek(void *opaque, PHYSFS_uint64 pos)
{
    HANDLE h = (HANDLE) opaque;
    const PHYSFS_sint64 spos = (PHYSFS_sint64) pos;
    BAIL_IF(!winSetFilePointer(h,spos,NULL,FILE_BEGIN), errcodeFromWinApi(), 0);
    return 1;  /* No error occured */
} /* __PHYSFS_platformSeek */


PHYSFS_sint64 __PHYSFS_platformTell(void *opaque)
{
    HANDLE h = (HANDLE) opaque;
    PHYSFS_sint64 pos = 0;
    BAIL_IF(!winSetFilePointer(h,0,&pos,FILE_CURRENT), errcodeFromWinApi(), -1);
    return pos;
} /* __PHYSFS_platformTell */


PHYSFS_sint64 __PHYSFS_platformFileLength(void *opaque)
{
    HANDLE h = (HANDLE) opaque;
    DWORD high = 0;
    const DWORD rc = GetFileSize(h, &high);
    if ((rc == PHYSFS_INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR))
        return -1;
    return (PHYSFS_sint64) ((((PHYSFS_uint64) high) << 32) | rc);
} /* __PHYSFS_platformFileLength */


int __PHYSFS_platformFlush(void *opaque)
{
    HANDLE h = (HANDLE) opaque;
    BAIL_IF(!FlushFileBuffers(h), errcodeFromWinApi(), 0);
    return 1;
} /* __PHYSFS_platformFlush */


void __PHYSFS_platformClose(void *opaque)
{
    HANDLE h = (HANDLE) opaque;
    (void) CloseHandle(h); /* ignore errors. You should have flushed! */
} /* __PHYSFS_platformClose */

int __PHYSFS_platformDelete(const char *path)
{
	WIN32_FILE_ATTRIBUTE_DATA info;
    if (!GetFileAttributesEx(path, GetFileExInfoStandard, &info))
        BAIL(errcodeFromWinApi(), 0);
    else
    {
        const int isdir = (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
        const BOOL rc = isdir ? RemoveDirectory(path) : DeleteFile(path);
        BAIL_IF(!rc, errcodeFromWinApi(), 0);
    } /* else */
    return 1;   /* if you made it here, it worked. */
} /* __PHYSFS_platformDelete */


void *__PHYSFS_platformCreateMutex(void)
{
    LPCRITICAL_SECTION lpcs;
    lpcs = (LPCRITICAL_SECTION) allocator.Malloc(sizeof (CRITICAL_SECTION));
    BAIL_IF(!lpcs, PHYSFS_ERR_OUT_OF_MEMORY, NULL);

	InitializeCriticalSection(lpcs);

    return lpcs;
} /* __PHYSFS_platformCreateMutex */


void __PHYSFS_platformDestroyMutex(void *mutex)
{
    DeleteCriticalSection((LPCRITICAL_SECTION) mutex);
    allocator.Free(mutex);
} /* __PHYSFS_platformDestroyMutex */


int __PHYSFS_platformGrabMutex(void *mutex)
{
    EnterCriticalSection((LPCRITICAL_SECTION) mutex);
    return 1;
} /* __PHYSFS_platformGrabMutex */


void __PHYSFS_platformReleaseMutex(void *mutex)
{
    LeaveCriticalSection((LPCRITICAL_SECTION) mutex);
} /* __PHYSFS_platformReleaseMutex */


static PHYSFS_sint64 FileTimeToPhysfsTime(const FILETIME *ft)
{
    SYSTEMTIME st_utc;
    PHYSFS_sint64 retval;
    struct tm tm;

    BAIL_IF(!FileTimeToSystemTime(ft, &st_utc), errcodeFromWinApi(), -1);

    /* Convert to a format that mktime() can grok... */
    tm.tm_sec = st_utc.wSecond;
    tm.tm_min = st_utc.wMinute;
    tm.tm_hour = st_utc.wHour;
    tm.tm_mday = st_utc.wDay;
    tm.tm_mon = st_utc.wMonth - 1;
    tm.tm_year = st_utc.wYear - 1900;
    tm.tm_wday = -1 /*st_localtz.wDayOfWeek*/;
    tm.tm_yday = -1;
    tm.tm_isdst = -1;

    /* Convert to a format PhysicsFS can grok... */
    retval = (PHYSFS_sint64) mktime(&tm);
    BAIL_IF(retval == -1, PHYSFS_ERR_OS_ERROR, -1);
    return retval;
} /* FileTimeToPhysfsTime */

int __PHYSFS_platformStat(const char *filename, PHYSFS_Stat *st, const int follow)
{
    WIN32_FILE_ATTRIBUTE_DATA winstat;
    DWORD err = 0;
    BOOL rc = 0;

    rc = GetFileAttributesEx(filename, GetFileExInfoStandard, &winstat);

    if (!rc)
        err = GetLastError();

    BAIL_IF(!rc, errcodeFromWinApiError(err), 0);

    st->modtime = FileTimeToPhysfsTime(&winstat.ftLastWriteTime);
    st->accesstime = FileTimeToPhysfsTime(&winstat.ftLastAccessTime);
    st->createtime = FileTimeToPhysfsTime(&winstat.ftCreationTime);

    if (winstat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        st->filetype = PHYSFS_FILETYPE_DIRECTORY;
        st->filesize = 0;
    } /* else if */

    else
    {
        st->filetype = PHYSFS_FILETYPE_REGULAR;
        st->filesize = (((PHYSFS_uint64) winstat.nFileSizeHigh) << 32) | winstat.nFileSizeLow;
    } /* else */

    st->readonly = ((winstat.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);

    return 1;
} /* __PHYSFS_platformStat */

#endif  /* PHYSFS_PLATFORM_XBOX */

/* end of physfs_platform_xbox.c ... */


