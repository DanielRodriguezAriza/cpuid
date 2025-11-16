#ifndef CPUID_UTIL_H
#define CPUID_UTIL_H

#include "cpuid.h"

static inline int cpuid_check(cpuid_u32_t leaf, cpuid_u32_t subleaf, cpuid_u32_t regidx, cpuid_u32_t bit)
{
	cpuid_reg_t regs;
	cpuid_call(leaf, subleaf, &regs);
	return regs.regs[regidx] & (1 << bit);
}

int cpuid_check_sse(void) { return cpuid_check(1, 0, 3, 25); }
int cpuid_check_sse2(void) { return cpuid_check(1, 0, 3, 26); }
int cpuid_check_sse3(void) { return cpuid_check(1, 0, 2, 0); }
int cpuid_check_bmi2(void) { return cpuid_check(7, 0, 1, 8); }
int cpuid_check_sha(void) { return cpuid_check(7, 0, 1, 29); }
int cpuid_check_avx512_bw(void) { return cpuid_check(7, 0, 1, 30); }
int cpuid_check_avx512_vbmi(void) { return cpuid_check(7, 0, 2, 1); }
int cpuid_check_sha512(void) { return cpuid_check(7, 1, 0, 0); }
int cpuid_check_mawau(void) { return cpuid_check(7, 0, 2, 19); }

#endif /* CPUID_UTIL_H */
