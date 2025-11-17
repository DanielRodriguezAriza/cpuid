#include <stdio.h>

#include "../include/cpuid_util.h"

#define SUPPORTS_CAT_I(x, y) x##y
#define SUPPORTS_CAT(x, y) SUPPORTS_CAT_I(x, y)
#define SUPPORTS(x) printf(" - supports %12s: %s\n", #x, SUPPORTS_CAT(cpuid_check_, x)() ? "yes" : "no");

int main()
{
	cpuid_str_t cpuStr;
	int maxLeaf = cpuid_check_info(&cpuStr);
	printf("CPUID: CPU Info:\n");
	printf(" - Max leaf value for CPUID: %d\n", maxLeaf);
	printf(" - CPU Manufacturer ID str : %s\n", cpuStr.str);
	printf("\n");
	
	printf("CPUID: Supported features:\n");
    SUPPORTS(fpu);
	SUPPORTS(vme);
	SUPPORTS(de);
	SUPPORTS(pse);
	SUPPORTS(sse);
    SUPPORTS(sse2);
    SUPPORTS(sse3);
    SUPPORTS(bmi2);
    SUPPORTS(sha);
	SUPPORTS(avx512_bw);
    SUPPORTS(avx512_vbmi);
    SUPPORTS(sha512);
    SUPPORTS(mawau);
	printf("\n");
}
