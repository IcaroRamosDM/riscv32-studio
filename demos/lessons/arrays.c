int values[4] = {2, 4, 6, 8};
volatile int result;

int main(void)
{
    for (int i = 0; i < 3; i++)
        result += values[i];
    return 0;
}
