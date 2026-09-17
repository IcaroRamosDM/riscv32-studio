#include <stdint.h>

volatile uint32_t input = 7;
volatile uint32_t result;

__attribute__((noinline)) static uint32_t calculate(uint32_t value)
{
  uint32_t scaled = value * 8u;
  uint32_t unused = value + 123u;
  (void)unused;
  uint32_t adjusted = scaled + 0u;
  return adjusted + 3u;
}

int main(void)
{
  result = calculate(input);
  return result == 59u ? 0 : 1;
}
