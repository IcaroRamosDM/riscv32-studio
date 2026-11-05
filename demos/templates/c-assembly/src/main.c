int increment(int value);
volatile int result;

int main(void)
{
    result = increment(41);
    return 0;
}
