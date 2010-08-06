#pragma once
#ifdef	_MSC_VER
typedef __int64		i64_time_t;
#else
typedef long long	i64_time_t;
#endif

typedef	i64_time_t	usec_t;
typedef	i64_time_t	cpuclock_t;

static inline usec_t		get_usec();			// get microseconds time stamp
static inline cpuclock_t	get_cpuclock();		// get CPU clocks stamp


#ifdef	__GNUC__

	#include <sys/time.h>

	static inline 
	usec_t	get_usec()
	{
		timeval usec_time;

		gettimeofday( &usec_time, NULL );

		return ( ( usec_t )( usec_time.tv_sec ) * 1000000 + usec_time.tv_usec );
	}


	static inline 
	cpuclock_t	get_cpuclock()
	{
	#ifdef __INTEL_COMPILER

		return ( ( cpuclock_t )__rdtsc() );

	#elif defined( __x86_64__ ) || defined( __i386__ )
		unsigned long		lo, hi;

		__asm__ __volatile__ ( "rdtsc" : "=a" (lo), "=d" (hi) );

		return ((cpuclock_t) hi << 32) | lo;
	#else 
		#error "unsuppoted platform by timing.h"
	#endif
	}

#elif defined(_WIN32)

	#include <windows.h>

	static __int64	perf_freq;
	static BOOL		init_freq = QueryPerformanceFrequency( (LARGE_INTEGER*)&perf_freq );

	static inline 
	usec_t	get_usec()
	{
		__int64	 counter;

		QueryPerformanceCounter( (LARGE_INTEGER*)&counter );

		return (usec_t)( ((double)counter / perf_freq) * 1000000.0 ); // convert to usec
	}

#if defined ( _AMD64_ ) && defined ( _MSC_VER )
#pragma intrinsic(__rdtsc)
#endif

	static inline 
	cpuclock_t	get_cpuclock()
	{
	#ifdef __INTEL_COMPILER

		return ( ( cpuclock_t )__rdtsc() );

	#elif defined( _X86_ )

		unsigned int		lo, hi;

		_asm { 
				rdtsc
				mov dword ptr lo, eax
				mov dword ptr hi, edx
		}

		return ( ( (cpuclock_t) hi << 32) | lo );
  #elif defined ( _AMD64_ ) && defined ( _MSC_VER )
    return ( ( cpuclock_t )__rdtsc() );
	#else 
		#error "unsuppoted platform by timing.h"
	#endif	
	}

#else

	#error "unsuppoted platform by timing.h"

#endif
