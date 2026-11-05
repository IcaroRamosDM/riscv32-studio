#include <rv_csr.h>
volatile unsigned previous, result;
volatile uint64_t observed_cycles, observed_retired;

int main(void)
{
    rv_csr_write(mscratch, 40);
    previous = rv_csr_swap(mscratch, 42);
    result = rv_csr_read(mscratch);
    observed_cycles = rv_cycle();
    observed_retired = rv_instret();
    return previous == 40 && result == 42 ? 0 : 1;
}
