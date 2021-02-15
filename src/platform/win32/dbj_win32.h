#pragma once

#ifdef _WIN32
//DBJ: we do the WIN10 BUILD only
#define  _WIN32_WINNT   0x0A00
#include <sdkddkver.h> // thus WINVER is set to 0xA00 too
#endif

#ifdef __clang__
#pragma clang system_header
#endif // __clang__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>

// https://stackoverflow.com/a/31335254/10870835
// struct timespec { long tv_sec; long tv_nsec; };    //header part
int clock_gettime(int dummy_, struct timespec* spec)      //C-file part
{
	__int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
	wintime -= 116444736000000000i64;  //1jan1601 to 1jan1970
	spec->tv_sec = wintime / 10000000i64;           //seconds
	spec->tv_nsec = wintime % 10000000i64 * 100;      //nano-seconds
	return 0;
}
// --------------------------------------------------------------------
// https://gist.github.com/Youka/4153f12cf2e17a77314c
/* Windows sleep in 100ns units */
inline BOOLEAN dbj_nanosleep(LONGLONG ns) {
	/* Declarations */
	HANDLE timer;	/* Timer handle */
	LARGE_INTEGER li;	/* Time defintion */
	/* Create timer */
	if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
		return FALSE;
	/* Set timer properties */
	li.QuadPart = -ns;
	if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
		CloseHandle(timer);
		return FALSE;
	}
	/* Start & wait for timer */
	WaitForSingleObject(timer, INFINITE);
	/* Clean resources */
	CloseHandle(timer);
	/* Slept without problems */
	return TRUE;
}

// https://stackoverflow.com/a/33389908/10870835
#define dbj_snprintf(buf,len, format,...) _snprintf_s(buf, len,len, format, __VA_ARGS__)
// --------------------------------------------------------------------------
// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/makepath-s-wmakepath-s?view=msvc-160
inline int
dbj_basename(const char path_buffer[_MAX_PATH],
	const int basename_len /*= _MAX_PATH*/,
	char basename_[basename_len]
)
{
	char drive[_MAX_DRIVE] = { 0 };
	char dir[_MAX_DIR] = { 0 };
	char fname[_MAX_FNAME] = { 0 };
	char ext[_MAX_EXT] = { 0 };
	errno_t err = 0;

	err = _splitpath_s(path_buffer, drive, _MAX_DRIVE, dir, _MAX_DIR, fname,
		_MAX_FNAME, ext, _MAX_EXT);
	if (err != 0)
	{
		perror("Error splitting the path");
		exit(1);
	}
	//printf("Path extracted with _splitpath_s:\n");
	//printf("   Drive: %s\n", drive);
	//printf("   Dir: %s\n", dir);
	//printf("   Filename: %s\n", fname);
	//printf("   Ext: %s\n", ext);

	_snprintf_s(basename_, _MAX_PATH, _MAX_PATH, "%s.%s", fname, ext);
}
// --------------------------------------------------------------------------

// https://stackoverflow.com/a/62371749/10870835

// Windows does not define the S_ISREG and S_ISDIR macros in stat.h, so we do.
// We have to define _CRT_INTERNAL_NONSTDC_NAMES 1 before #including sys/stat.h
// in order for Microsoft's stat.h to define names like S_IFMT, S_IFREG, and S_IFDIR,
// rather than just defining  _S_IFMT, _S_IFREG, and _S_IFDIR as it normally does.
#define _CRT_INTERNAL_NONSTDC_NAMES 1
#include <sys/stat.h>
#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif
#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif