#include <rv_atomic.h>
#include <stdio.h>

/* Single-hart demonstration. Step over LR and watch reserved()/reservation().
 * Editing counter between LR and SC cancels the reservation. */
volatile uint32_t counter = 40, previous, loaded, status, result;

int main(void)
{
    previous = rv_atomic_add(&counter, 2);
    loaded = rv_atomic_lr(&counter);
    status = rv_atomic_sc(&counter, 43);
    result = counter;
    printf("previous=%u loaded=%u status=%u result=%u\n",
        (unsigned)previous,
        (unsigned)loaded,
        (unsigned)status,
        (unsigned)result);
    return previous == 40 && loaded == 42 && status == 0 && result == 43 ? 0 : 1;
}
