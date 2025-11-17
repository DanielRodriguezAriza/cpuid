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
int cpuid_check_info(cpuid_char_t *buf) CPUID_NOEXCEPT
{
	cpuid_reg_t x;
	cpuid_call(0,0,&x);

	buf[12] = 0;
	// NOTE : Maybe rewrite this using memcpy instead of this UB ridden type punning... or use unions.
	// All to avoid including extra headers like string.h lol...
	*((cpuid_u32_t*)(buf) + 0) = x.ebx;
	*((cpuid_u32_t*)(buf) + 1) = x.edx;
	*((cpuid_u32_t*)(buf) + 2) = x.ecx;
	
	return x.eax;
}

// Returns true on success and false on failure.
// Failure happens when the CPU does not have this feature.
// Think of the return value as a "has brand string feature" flag.
int cpuid_brand_string(cpuid_char_t *buf) CPUID_NOEXCEPT
{
	cpuid_reg_t x, y, z;
	
	// Check if the feature is present on the CPU
	cpuid_call(0x80000000, 0, &x);
	if(x.eax < 0x80000004)
		return 0;
	
	// Store all of the bytes from the CPU brand string
	cpuid_call(0x80000002, 0, &x);
	cpuid_call(0x80000003, 0, &y);
	cpuid_call(0x80000004, 0, &z);
	
	// Store the data into the buf string
	// NOTE : Again, this would be easier with memcpy, but I don't want to force include string.h yet,
	// so I'll think of adding some optional flag for that later on or just including it and calling it a day...
	buf[CPUID_STRLEN_BUF_BRAND - 1] = 0;
	*(((cpuid_u32_t*)(buf)) + 0) = x.eax;
	*(((cpuid_u32_t*)(buf)) + 1) = x.ebx;
	*(((cpuid_u32_t*)(buf)) + 2) = x.ecx;
	*(((cpuid_u32_t*)(buf)) + 3) = x.edx;
	*(((cpuid_u32_t*)(buf)) + 4) = y.eax;
	*(((cpuid_u32_t*)(buf)) + 5) = y.ebx;
	*(((cpuid_u32_t*)(buf)) + 6) = y.ecx;
	*(((cpuid_u32_t*)(buf)) + 7) = y.edx;
	*(((cpuid_u32_t*)(buf)) + 8) = z.eax;
	*(((cpuid_u32_t*)(buf)) + 9) = z.ebx;
	*(((cpuid_u32_t*)(buf)) + 10) = z.ecx;
	*(((cpuid_u32_t*)(buf)) + 11) = z.edx;
	
	return 1;
}

int cpuid_check_fpu         (void) CPUID_NOEXCEPT { return cpuid_check(1, 0, 3, 0); } // On board x87 FPU
int cpuid_check_vme         (void) CPUID_NOEXCEPT { return cpuid_check(1, 0, 3, 1); }
int cpuid_check_de          (void) CPUID_NOEXCEPT { return cpuid_check(1, 0, 3, 2); }
int cpuid_check_pse         (void) CPUID_NOEXCEPT { return cpuid_check(1, 0, 3, 3); } // Page size extension with 4MB pages.
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
