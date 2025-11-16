#include <stdio.h>

#include "cpuid_util.h"

#define SUPPORTS_CAT_I(x, y) x##y
#define SUPPORTS_CAT(x, y) SUPPORTS_CAT_I(x, y)
#define SUPPORTS(x) printf("supports %s: %s\n", #x, SUPPORTS_CAT(cpuid_check_, x)() ? "yes" : "no");

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
