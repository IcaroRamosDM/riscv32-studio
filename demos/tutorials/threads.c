/* C17 hosted threads: main runs once; the second hart runs the new thread.
 * Joining completes synchronization before main reads the shared result. */
#include <stdatomic.h>
#include <stdio.h>
#include <threads.h>

static atomic_uint counter;

static int increment(void *unused)
{
    (void)unused;
    for (unsigned i = 0; i < 100; ++i)
        atomic_fetch_add_explicit(&counter, 1, memory_order_relaxed);
    return 7;
}

int main(void)
{
    thrd_t worker;
    int status;
    if (thrd_create(&worker, increment, NULL) != thrd_success)
        return 1;
    for (unsigned i = 0; i < 100; ++i)
        atomic_fetch_add_explicit(&counter, 1, memory_order_relaxed);
    if (thrd_join(worker, &status) != thrd_success)
        return 2;
    unsigned value = atomic_load(&counter);
    printf("counter = %u; thread = %d\n", value, status);
    return value == 200 && status == 7 ? 0 : 3;
}
