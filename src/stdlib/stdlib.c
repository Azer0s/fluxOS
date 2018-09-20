#include <stdlib.h>
#include <stdint.h>

static unsigned long int RAND_NEXT = 1;

int32_t rand(void)
{
    RAND_NEXT = RAND_NEXT * 1103515245 + 12345;
    return (uint32_t)(RAND_NEXT/65536) % 32768;
}

void srand(uint32_t seed)
{
    RAND_NEXT = seed;
}