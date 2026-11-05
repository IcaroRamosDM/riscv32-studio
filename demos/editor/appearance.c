#include <stdint.h>
// Comments and line numbers
volatile int result;

int main(void)
{
    const char *text = "RISC-V";
    result = 20 + 22;
    return text[0];
}
