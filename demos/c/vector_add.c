#include <stdint.h>
void add_vectors(const uint32_t *left, const uint32_t *right, uint32_t *out, unsigned count);
uint32_t left[17], right[17], out[17];
volatile unsigned result;

int main(void)
{
    for (unsigned i = 0; i < 17; ++i) {
        left[i] = i + 1;
        right[i] = 2 * i;
    }
    add_vectors(left, right, out, 17);
    for (unsigned i = 0; i < 17; ++i) {
        if (out[i] != 3 * i + 1)
            return 1;
        result += out[i];
    }
    return result == 425 ? 0 : 2;
}
