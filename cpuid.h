#ifndef CPUID_H
#define CPUID_H

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

typedef union cpuid_reg_t {
	cpuid_u32_t a, b, c, d;
	cpuid_u32_t eax, ebx, ecx, edx;
	cpuid_u32_t regs[4];
} cpuid_reg_t;

#define CPUID_REG_IDX_EAX 0
#define CPUID_REG_IDX_EBX 1
#define CPUID_REG_IDX_ECX 2
#define CPUID_REG_IDX_EDX 3

static inline void cpuid_call(cpuid_u32_t leaf, cpuid_u32_t subleaf, cpuid_reg_t *registers)
{
#if defined(CPUID_VERSION_MSVC)
	__cpuidex(regs->regs, leaf, subleaf);
#elif defined(CPUID_VERSION_GCC) || defined(CPUID_VERSION_LLVM)
	__asm__ volatile(
		"cpuid"
		: "=a"(regs->eax), "=b"(regs->ebx), "=c"(regs->ecx), "=d"(regs->edx)
		: "a"(leaf), "c"(subleaf)
	);
#else
	#error "Unsupported compiler: no CPUID intrinsic or inline assembly available or known to the implementation."
#endif
}

#endif /* CPUID_H */
