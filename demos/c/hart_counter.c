#include "rv_hart.h"
#include "rv_atomic.h"
volatile unsigned counter, arrived, result;
volatile unsigned contributions[4];

int main(void)
{
    const unsigned id = rv_hart_id(), count = rv_hart_count();
    for (unsigned i = 0; i < 100; ++i) {
        __atomic_fetch_add(&counter, 1, __ATOMIC_SEQ_CST);
        ++contributions[id];
    }
    __atomic_fetch_add(&arrived, 1, __ATOMIC_SEQ_CST);
    while (__atomic_load_n(&arrived, __ATOMIC_SEQ_CST) != count) {
    }
    if (id == 0)
        result = counter;
    return counter == 100 * count ? 0 : 1;
}
