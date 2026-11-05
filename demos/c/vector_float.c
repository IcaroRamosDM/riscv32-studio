#include <stdint.h>
/* C plus Assembly: 17 values force a shorter final vector group. */
uint32_t vector_float_sum(const float *left, const float *right, float *out, unsigned count);
float left[17], right[17], out[17];
volatile float result;
volatile uint32_t result_bits;

int main(void)
{
    for (unsigned i = 0; i < 17; ++i) {
        left[i] = (float)i;
        right[i] = 0.5f;
    }

    union {
        uint32_t bits;
        float value;
    } total;

    total.bits = vector_float_sum(left, right, out, 17);
    result_bits = total.bits;
    result = total.value;
    for (unsigned i = 0; i < 17; ++i)
        if (out[i] != (float)i * 2.0f + 0.5f)
            return 1;
    return result == 280.5f ? 0 : 2;
}
