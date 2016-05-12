#ifndef _ATOMIC_H_
#define _ATOMIC_H_

/// atomic defs
#ifdef _MSC_VER
#define atomic_inc32(counter) _InterlockedIncrement((volatile long*)&counter)
#define atomic_dec32(counter) _InterlockedDecrement((volatile long*)&counter)
#define atomic_add32(counter,i) _InterlockedExchangeAdd((volatile long*)&counter,i)
#define atomic_cmpxchg32(dest,exchg,cmp) _InterlockedCompareExchange((volatile long*)&dest,exchg,cmp)

#elif defined(__GNUC__)
#define atomic_inc32(counter)     \
    __asm__ __volatile__(      \
        "lock incl %0"         \
        : "+m"(counter)           \
        )
#define atomic_dec32(vval)     \
    __asm__ __volatile__(      \
        "lock decl %0"         \
        : "+m"(vval)           \
        )
#define atomic_add32(counter,i) \
    __asm__ __volatile__(       \
        "lock addl %1,%0"       \
        :"+m" (counter)         \
        :"ir" (i))
#define atomic_cmpxchg32(dest,exchg,cmp)   \
    __asm__ __volatile__(                  \
        "movl %2, %%eax\n\t"               \
        "lock cmpxchg %1, %0"              \
        : "+m"(dest)                       \
        : "r"(exchg),"r"(cmp)              \
        : "cc", "memory", "eax", "ecx"     \
        )
#endif

#endif
