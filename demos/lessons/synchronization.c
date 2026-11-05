#include <rv_hart.h>
volatile unsigned counter, finished;

int main(void)
{
    for (unsigned i = 0; i < 64; ++i) {
        counter = counter + 1;
    }
    __atomic_fetch_add(&finished, 1, __ATOMIC_RELEASE);
    if (rv_hart_id() == 0)
        while (__atomic_load_n(&finished, __ATOMIC_ACQUIRE) != rv_hart_count()) {
        }
    return 0;
}
