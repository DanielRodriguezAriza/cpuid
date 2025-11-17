#ifndef CPUID_H
#define CPUID_H

/* C++ support */
#ifdef __cplusplus
	#define CPUID_EXTERN_C_BEGIN extern "C" {
	#define CPUID_EXTERN_C_END }
	#define CPUID_NOEXCEPT noexcept
#else
	#define CPUID_EXTERN_C_BEGIN
	#define CPUID_EXTERN_C_END
	#define CPUID_NOEXCEPT
#endif
#define CPUID_LINKAGE static inline

/* MSVC support */
#if defined(_MSC_VER) && (defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64))
	#define CPUID_VERSION_MSVC
	#include <intrin.h>
#endif

/* GNUC GCC support */
#if defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)
	#define CPUID_VERSION_GCC
#endif

/* LLVM Clang support */
#if defined(__clang__) || defined(__llvm__)
	#define CPUID_VERSION_LLVM
#endif

/* Check for stdint support */
#if defined(_MSC_VER) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
	#include <stdint.h>
	#define CPUID_U32_TYPE uint32_t
#endif

/* cpuid types */
#ifndef CPUID_U32_TYPE
	#define CPUID_U32_TYPE unsigned int
#endif

typedef CPUID_U32_TYPE cpuid_u32_t;

typedef struct cpuid_str_t {
	unsigned char str[13];
} cpuid_str_t;

// String lengths in raw
#define CPUID_STRLEN_MANUFACTURER (3 * 4)
#define CPUID_STRLEN_BRAND (4 * 4)

// String lengths with null terminator already accounted for
#define CPUID_STRLENZ_MANUFACTURER (CPUID_STRLEN_MANUFACTURER + 1)
#define CPUID_STRLENZ_BRAND (CPUID_STRLEN_BRAND + 1)

typedef unsigned char cpuid_char_t;

typedef struct cpuid_reg_t {
	union {
		cpuid_u32_t regs[4];
		struct {
			cpuid_u32_t eax, ebx, ecx, edx;
		};
		struct {
			cpuid_u32_t a, b, c, d;
		};
	};
} cpuid_reg_t;

#define CPUID_REG_IDX_EAX 0
#define CPUID_REG_IDX_EBX 1
#define CPUID_REG_IDX_ECX 2
#define CPUID_REG_IDX_EDX 3

CPUID_EXTERN_C_BEGIN

CPUID_LINKAGE void cpuid_call(cpuid_u32_t leaf, cpuid_u32_t subleaf, cpuid_reg_t *regs) CPUID_NOEXCEPT
{
#if defined(CPUID_VERSION_MSVC)
	__cpuidex(regs->regs, leaf, subleaf);
#elif defined(CPUID_VERSION_GCC) || defined(CPUID_VERSION_LLVM)
	__asm__ volatile(
		"cpuid"
		: "=a"(regs->regs[0]), "=b"(regs->regs[1]), "=c"(regs->regs[2]), "=d"(regs->regs[3])
		: "a"(leaf), "c"(subleaf)
	);
#else
	#error "Unsupported compiler: no CPUID intrinsic or inline assembly available or known to the implementation."
#endif
}

CPUID_EXTERN_C_END

#endif /* CPUID_H */
