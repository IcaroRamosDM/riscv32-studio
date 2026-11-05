#include <stdint.h>

typedef struct Sample {
    int32_t value;
    int16_t readings[3];
    struct Sample *next;
} Sample;

int main(void)
{
    Sample sample = {42, {-7, 10, 20}, 0};
    sample.next = &sample;
    Sample *pointer = &sample;
    int32_t matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    /* Pause here: Explore sample, matrix, and pointer in Variables / stack.
       Watch: sample.readings[0] + matrix[1][2], pointer->next->value. */
    __asm__ volatile("ebreak" : : "r"(pointer), "r"(matrix));
    return sample.value == 42 ? 0 : 1;
}
