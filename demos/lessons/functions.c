volatile int result;

int square(int n)
{
    return n + n;
}

int main(void)
{
    result = square(9);
    return 0;
}
