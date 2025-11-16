#include <stdio.h>

// TODO : Clean this up into separate header and stuff for easier usage in the future.

#if defined(_MSC_VER) && (defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64))

	#include <intrin.h>

	void cpuid1(int *regs, int level)
	{
		__cpuid(regs, level);
	}
	
	void cpuid2(int *regs, int level, int sublevel)
	{
		__cpuidex(regs, level, sublevel);
	}

#else
	
	void cpuid1(int *regs, int level)
	{
		__asm__ volatile(
			"cpuid"
			: "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
			: "a"(level)
		);
	}
	
	void cpuid2(int *regs, int level, int sublevel)
	{
		__asm__ volatile(
			"cpuid"
			: "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
			: "a"(level), "c"(sublevel)
		);
	}
	
#endif

typedef struct {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
} cpuid_t;

void cpuid_c(cpuid_t *x)
{
	cpuid2((int*)x, x->eax, x->ecx);
}

int has_sse(void)
{
    cpuid_t x;
    x.eax = 1;
    x.ecx = 0;
    cpuid_c(&x);
    return x.edx & (1 << 25);
}

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


#define SUPPORTS_CAT_I(x, y) x##y
#define SUPPORTS_CAT(x, y) SUPPORTS_CAT_I(x, y)
#define SUPPORTS(x) printf("supports %s: %s\n", #x, SUPPORTS_CAT(has_, x)() ? "yes" : "no");

int main()
{
    SUPPORTS(sse);
    SUPPORTS(sse2);
    SUPPORTS(sse3);
    SUPPORTS(bmi2);
    SUPPORTS(sha);
	SUPPORTS(avx512_bw);
    SUPPORTS(avx512_vbmi);
    SUPPORTS(sha512);
    SUPPORTS(mawau);
}
