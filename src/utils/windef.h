#ifndef char_H
#define char_H

#include <stdarg.h>
#include <cstdint>
#include <string.h>

#ifdef UNICODE
#include <wchar.h>
#include <wctype.h>
typedef wchar_t char;
#define _stprintf  wsprintf
#define _vstprintf vswprintf
#define _sntprintf wsnprintf
#define _ftprintf  fwprintf
#define _vftprintf vfwprintf
#define _tcsdup    wcsdup
#define _tcscpy    wcscpy
#define tcslen     wcslen
#define _tcscmp    wcscmp
#define _tcsncmp   wcscmp
#define _tcsicmp   wcscasecmp
#define _tcsnicmp  wcsncasecmp
#define _tcslen    wcslen
#define _tcsclen   wcslen
#define _tcsncpy   wcsncpy
#define _tcsstr    wcsstr
#define _tcschr    wcschr
#define _tcscat    wcscat
#define _tcsncat   wcsncat
#define TEXT(x)    (L##x)
#define(x)(L##x)
#define _fgetts  fgetws
#define _stscanf swscanf
#define _tcstok  wcstok
#define _itot    _itow
#define _ttoi(x) wcstol((x), NULL, 10)
#define _tcstol  wcstol
#define _tcstod  wcstod

#define _istalpha iswalpha
#define _istspace iswspace

#else


#define _stprintf  sprintf
#define _vstprintf vsprintf
#define _sntprintf snprintf
#define _ftprintf  fprintf
#define _vftprintf vfprintf
#define _tcsdup    strdup
#define _tcscpy    strcpy
#define tcslen     strlen
#define _tcscmp    strcmp
#define _tcsncmp   strncmp
#define _tcsicmp   strcasecmp
#define _tcsnicmp  strncasecmp
#define _tcslen    strlen
#define _tcsclen   strlen
#define _tcsncpy   strncpy
#define _tcsstr    strstr
#define _tcschr    strchr
#define _tcscat    strcat
#define _tcsncat   strncat
#define TEXT(x)    x
#define _tfopen    fopen
#define _fgetts    fgets
#define _stscanf   sscanf
#define _tcstok    strtok
#define _totupper  toupper
#define _itot      itoa
#define _ttoi      atoi
#define _tcstol    strtol
#define _tcstod    strtod
#define _tcscspn   strcspn

#define _istalpha isalpha
#define _istspace isspace
#endif

#define _tcsupr CharUpper

typedef char       *LPTSTR;
typedef const char *LPCTSTR;
#endif

#ifndef WINDEF_H
#define WINDEF_H


#ifndef MAX_PATH
#define MAX_PATH 4096
#endif

#define _hypot hypot

typedef bool BOOL;
enum
{
    FALSE = false,
    TRUE  = true,
};

typedef char    CHAR;
typedef uint8_t BYTE;
typedef int64_t __int64;
typedef int64_t _int64;

typedef uint32_t     DWORD;
typedef DWORD       *LPDWORD;
typedef int32_t      LONG;
typedef uint32_t     ULONG;
typedef uint32_t     WORD;
typedef unsigned int UINT;
typedef int          INT;
typedef int32_t      WPARAM;
typedef int32_t      LPARAM;
typedef int32_t      LRESULT;
typedef void        *LPVOID;

typedef uint8_t  LPBYTE;
typedef uint16_t LPWORD;




typedef struct w32_handle *HANDLE;
typedef DWORD              HRESULT;
typedef HANDLE             HINSTANCE;
typedef HANDLE             HBRUSH;
typedef HANDLE             HPEN;
typedef HANDLE             HBITMAP;
typedef HANDLE             HFONT;
typedef HANDLE             HCURSOR;
typedef HANDLE             HICON;
typedef HANDLE             ATOM;
typedef struct w32_wnd    *HWND;
typedef struct w32_dc     *HDC;

#define INVALID_HANDLE_VALUE NULL
#define HWNDOP               NULL

typedef DWORD  COLORREF;
typedef DWORD *LPCOLORREF;

static inline COLORREF RGB(int r, int g, int b)
{
    return (r & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16);
}

typedef struct tagPOINT
{
    LONG x;
    LONG y;
} POINT;

typedef struct tagSIZE
{
    LONG cx;
    LONG cy;
} SIZE, *PSIZE;

typedef struct _RECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;

typedef RECT *LPRECT;

#define WINAPI
#define _cdecl
#define CALLBACK

#define _wcsdup wcsdup

#define LF_FACESIZE 256

typedef struct tagLOGFONT
{
    LONG lfHeight;
    LONG lfWidth;
    LONG lfEscapement;
    LONG lfOrientation;
    LONG lfWeight;
    BYTE lfItalic;
    BYTE lfUnderline;
    BYTE lfStrikeOut;
    BYTE lfCharSet;
    BYTE lfOutPrecision;
    BYTE lfClipPrecision;
    BYTE lfQuality;
    BYTE lfPitchAndFamily;
    char lfFaceName[LF_FACESIZE];
} LOGFONT;

typedef struct
{
    HWND   hwnd;
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD  time;
    POINT  pt;
} MSG, *PMSG;

typedef LRESULT           CALLBACK (*WNDPROC)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef void             *LPSECURITY_ATTRIBUTES;
typedef struct OVERLAPPED OVERLAPPED;
typedef OVERLAPPED       *LPOVERLAPPED;

typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

#define MAXPNAMELEN            32
#define MAX_JOYSTICKOEMVXDNAME 260

typedef struct joycaps_tag
{
    WORD wMid;
    WORD wPid;
    char szPname[MAXPNAMELEN];
    UINT wXmin;
    UINT wXmax;
    UINT wYmin;
    UINT wYmax;
    UINT wZmin;
    UINT wZmax;
    UINT wNumButtons;
    UINT wPeriodMin;
    UINT wPeriodMax;
    UINT wRmin;
    UINT wRmax;
    UINT wUmin;
    UINT wUmax;
    UINT wVmin;
    UINT wVmax;
    UINT wCaps;
    UINT wMaxAxes;
    UINT wNumAxes;
    UINT wMaxButtons;
    char szRegKey[MAXPNAMELEN];
    char szOEMVxD[MAX_JOYSTICKOEMVXDNAME];
} JOYCAPS, *PJOYCAPS, *NPJOYCAPS, *LPJOYCAPS;

#endif