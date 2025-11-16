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

int has_sse2(void)
{
    cpuid_t x;
    x.eax = 1;
    x.ecx = 0;
    cpuid_c(&x);
    return x.edx & (1 << 26);
}

int has_sse3(void)
{
    cpuid_t x;
    x.eax = 1;
    x.ecx = 0;
    cpuid_c(&x);
    return x.ecx & (1 << 0);
}

int has_bmi2(void)
{
    cpuid_t x;
    x.eax = 7;
    x.ecx = 0;
    cpuid_c(&x);
    return x.ebx & (1 << 8);
}

int has_sha(void)
{
    cpuid_t x;
    x.eax = 7;
    x.ecx = 0;
    cpuid_c(&x);
    return x.ebx & (1 << 29);
}

int has_avx512_bw(void)
{
	cpuid_t x;
	x.eax = 7;
	x.ecx = 0;
	cpuid_c(&x);
	return x.ebx & (1 << 30);
}

int has_avx512_vbmi(void)
{
	cpuid_t x;
	x.eax = 7;
	x.ecx = 0;
	cpuid_c(&x);
	return x.ecx & (1 << 1);
}

int has_sha512(void)
{
	cpuid_t x;
	x.eax = 7;
	x.ecx = 1;
	cpuid_c(&x);
	return x.eax & (1 << 0);
}

int has_mawau(void)
{
	cpuid_t x;
	x.eax = 7;
	x.ecx = 0;
	cpuid_c(&x);
	return x.ecx & (1 << 19);
}

#endif /* CPUID_UTIL_H */
