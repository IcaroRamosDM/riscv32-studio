#include "rv_firmware.h"
#include "rv_hart.h"
volatile unsigned timer_count[4], software_count[4], ready[4];

__attribute__((interrupt("machine"), aligned(4))) void handler(void)
{
    unsigned hart = rv_hart_id();
    uintptr_t cause = rv_csr_read(mcause);
    if (cause == (RV_MCAUSE_INTERRUPT | 7u)) {
        ++timer_count[hart];
        rv_timer_at(UINT64_MAX);
        rv_csr_clear(mie, 128);
    } else if (cause == (RV_MCAUSE_INTERRUPT | 3u)) {
        ++software_count[hart];
        rv_software_interrupt(0);
        rv_csr_clear(mie, 8);
    } else
        rv_firmware_exit(1);
}

int main(void)
{
    unsigned hart = rv_hart_id(), count = rv_hart_count();
    rv_csr_write(mtvec, (uintptr_t)handler);
    rv_timer_after(400 + hart * 200);
    rv_csr_set(mie, 136);
    rv_csr_set(mstatus, 8);
    __atomic_store_n(&ready[hart], 1, __ATOMIC_RELEASE);
    unsigned next = (hart + 1) % count;
    while (!__atomic_load_n(&ready[next], __ATOMIC_ACQUIRE))
        rv_wait_for_interrupt();
    if (rv_hart_software_interrupt(next, 1) != 0)
        return 2;
    while (!timer_count[hart] || !software_count[hart])
        rv_wait_for_interrupt();
    return timer_count[hart] == 1 && software_count[hart] == 1 ? 0 : 3;
}
