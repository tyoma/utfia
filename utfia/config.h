#pragma once

#if defined(_MSC_VER)
	#define UTFIA_INLINE __forceinline
	#define UTFIA_AVOID_INLINE __declspec(noinline)
#elif defined(__GNUC__)
	#define UTFIA_INLINE __attribute__((always_inline)) inline
	#define UTFIA_AVOID_INLINE __attribute__((noinline))
#else
	#define UTFIA_INLINE inline
	#define UTFIA_AVOID_INLINE
#endif
