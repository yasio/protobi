#include <time.h>
#include "oslib.h"

#if defined (_WIN32)
#include <Windows.h>

/// the implemention of atoll on win32 for visual studio 2012 or before
#if _MSC_VER <= 1700
extern long long atoll(const char* p)  
{  
    int minus = 0;  
    long long value = 0;  
    if (*p == '-')  
    {  
        minus ++;  
        p ++;  
    }  
    while (*p >= '0' && *p <= '9')  
    {  
        value *= 10;  
        value += *p - '0';  
        p ++;  
    }  
    return minus ? 0 - value : value;  
}
#endif

/// the implemention of gettimeofday on win32
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)   
#define EPOCHFILETIME  11644473600000000Ui64   
#else   
#define EPOCHFILETIME  11644473600000000ULL   
#endif
extern int  
    gettimeofday (struct timeval *tv, struct timezone2 *tz)  
{  
    FILETIME ft;  
    LARGE_INTEGER li;  
    __int64 t;  
    static int tzflag;  

    if (tv)  
    {  
        GetSystemTimeAsFileTime (&ft);  
        li.LowPart = ft.dwLowDateTime;  
        li.HighPart = ft.dwHighDateTime;  
        t = li.QuadPart;      /* In 100-nanosecond intervals */  
        t -= EPOCHFILETIME;   /* Offset to the Epoch time */  
        t /= 10;          /* In microseconds */  
        tv->tv_sec = (long) (t / 1000000);  
        tv->tv_usec = (long) (t % 1000000);  
    }  

    if (tz)  
    {  
        if (!tzflag)  
        {  
            tzset ();  

            tzflag++;  
        }  
        tz->tz_minuteswest = _timezone / 60;  
        tz->tz_dsttime = _daylight;  
    }  

    return 0;  
}  /* windows */

#elif defined(__linux)
#include <sys/time.h>
#endif

// comman
long long atoll_s(const char* p, int n)
{
	int minus = 0;
	long long value = 0;
	if (*p == '-')
	{
		minus++;
		p++;
		--n;
	}
	while (*p >= '0' && *p <= '9' && n-- > 0)
	{
		value *= 10;
		value += *p - '0';
		p++;
	}
	if (minus)
		value = -value;
	return value;
}

int atoi_s(const char* p, int n)
{
	int minus = 0;
	int value = 0;
	if (*p == '-')
	{
		minus++;
		p++;
		--n;
	}
	while (*p >= '0' && *p <= '9' && n-- > 0)
	{
		value *= 10;
		value += *p - '0';
		p++;
	}
	if (minus)
		value = -value;
	return value;
}

long long millitime(void)
{
	return microtime() / 1000;
}

long long microtime(void)
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}
