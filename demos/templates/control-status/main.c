#include <rv_csr.h>
volatile unsigned previous, result;

int main(void)
{
    rv_csr_write(mscratch, 40);
    previous = rv_csr_swap(mscratch, 42);
    result = rv_csr_read(mscratch);
    return previous == 40 && result == 42 ? 0 : 1;
}
