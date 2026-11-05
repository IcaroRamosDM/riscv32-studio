/* Run, then type Ada and Enter, followed by 7 and Enter in Terminal.
 * Width limits leave space for the name's terminator and bound the integer. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char name[32];
    char number[5];
    printf("Your name: ");
    fflush(stdout);
    if (scanf("%31s", name) != 1) {
        puts("Input ended before the name.");
        return 1;
    }
    printf("Years of experience (0-999): ");
    fflush(stdout);
    if (scanf("%4s", number) != 1 || strlen(number) > 3 ||
        strspn(number, "0123456789") != strlen(number)) {
        puts("Expected an integer.");
        return 2;
    }
    unsigned years = (unsigned)strtoul(number, NULL, 10);
    printf("Hello, %s! Next year: %u.\n", name, years + 1);
    return 0;
}
