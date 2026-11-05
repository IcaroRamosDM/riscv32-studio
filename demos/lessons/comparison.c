volatile unsigned result;

static unsigned square(unsigned n)
{
    return n * n;
}

int main(void)
{
    unsigned sum = 0;
    for (unsigned i = 1; i < 10; ++i)
        sum += square(i);
    result = sum;
    return 0;
}
