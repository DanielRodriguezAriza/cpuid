#ifndef CPUID_UTIL_H
#define CPUID_UTIL_H

#include "cpuid.h"

CPUID_EXTERN_C_BEGIN

CPUID_LINKAGE int cpuid_check(cpuid_u32_t leaf, cpuid_u32_t subleaf, cpuid_u32_t regidx, cpuid_u32_t bit) CPUID_NOEXCEPT
{
	cpuid_reg_t regs;
	cpuid_call(leaf, subleaf, &regs);
	return regs.regs[regidx] & (1 << bit);
}

// Returns the largest value that can be passed through EAX to the CPUID instruction
// and sets the values of the registers EBX, EDX, ECX to a 12 char ASCII string with the manufacturer ID.
int cpuid_check_info(cpuid_str_t *buf) CPUID_NOEXCEPT
{
	cpuid_reg_t x;
	cpuid_call(0,0,&x);

	buf->str[12] = 0;
	// NOTE : Maybe rewrite this using memcpy instead of this UB ridden type punning... or use unions.
	// All to avoid including extra headers like string.h lol...
	*((cpuid_u32_t*)(buf->str) + 0) = x.ebx;
	*((cpuid_u32_t*)(buf->str) + 1) = x.edx;
	*((cpuid_u32_t*)(buf->str) + 2) = x.ecx;
	
	return x.eax;
}

int cpuid_check_sse         (void) CPUID_NOEXCEPT { return cpuid_check(1, 0, 3, 25); }
int cpuid_check_sse2        (void) CPUID_NOEXCEPT { return cpuid_check(1, 0, 3, 26); }
int cpuid_check_sse3        (void) CPUID_NOEXCEPT { return cpuid_check(1, 0, 2, 0); }
int cpuid_check_bmi2        (void) CPUID_NOEXCEPT { return cpuid_check(7, 0, 1, 8); }
int cpuid_check_sha         (void) CPUID_NOEXCEPT { return cpuid_check(7, 0, 1, 29); }
int cpuid_check_avx512_bw   (void) CPUID_NOEXCEPT { return cpuid_check(7, 0, 1, 30); }
int cpuid_check_avx512_vbmi (void) CPUID_NOEXCEPT { return cpuid_check(7, 0, 2, 1); }
int cpuid_check_sha512      (void) CPUID_NOEXCEPT { return cpuid_check(7, 1, 0, 0); }
int cpuid_check_mawau       (void) CPUID_NOEXCEPT { return cpuid_check(7, 0, 2, 19); }

CPUID_EXTERN_C_END

#endif /* CPUID_UTIL_H */
