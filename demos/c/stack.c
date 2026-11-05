#include <stdint.h>
volatile uint32_t lowest_sp = UINT32_MAX, entry_sp, result;

static __attribute__((noinline)) uint32_t descend(uint32_t depth)
{
    volatile uint32_t local[4] = {depth, 2, 3, 4};
    uint32_t sp;
    __asm__ volatile("mv %0, sp" : "=r"(sp));
    if (sp < lowest_sp)
        lowest_sp = sp;
    uint32_t child = depth ? descend(depth - 1) : 0;
    return local[0] + child;
}

int main(void)
{
    __asm__ volatile("mv %0, sp" : "=r"(entry_sp));
    result = descend(4);
    return result == 10 && lowest_sp < entry_sp ? 0 : 1;
}
