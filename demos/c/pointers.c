#include <stdint.h>
uint32_t values[3] = {10, 20, 30};
volatile uint32_t result;

int main(void)
{
    uint32_t *p = &values[1];
    *p = 42;
    unsigned char *bytes = (unsigned char *)p;
    bytes[1] = 1;
    result = *p;
    return result == 298 && values[0] == 10 && values[2] == 30 ? 0 : 1;
}
