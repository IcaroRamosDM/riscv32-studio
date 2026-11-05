volatile int left = -21, right = 3;
volatile int product, quotient, remainder;

int main(void)
{
    product = left * right;
    quotient = left / right;
    remainder = left % right;
    return 0;
}
