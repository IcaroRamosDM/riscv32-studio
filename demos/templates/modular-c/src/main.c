#include "../include/counter.h"
volatile unsigned result;

int main(void)
{
    result = add(20, 22);
    return 0;
}
