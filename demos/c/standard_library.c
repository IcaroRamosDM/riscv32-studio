#include <errno.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile double result;

int main(void)
{
    setlocale(LC_ALL, "");
    double number = 0;
    puts("C17 standard library");
    printf("Decimal number (use .): ");
    if (scanf("%lf", &number) != 1) {
        fputs("Invalid number.\n", stderr);
        return 1;
    }
    result = sqrt(fabs(number));
    char record[160];
    snprintf(record, sizeof record, "number=%.6f; sqrt(abs(number))=%.6f\n", number, result);
    char *copy = malloc(strlen(record) + 1);
    if (!copy) {
        perror("malloc");
        return 2;
    }
    strcpy(copy, record);
    FILE *file = fopen("result.txt", "w+");
    if (!file) {
        perror("fopen");
        free(copy);
        return 3;
    }
    fputs(copy, file);
    rewind(file);
    if (!fgets(record, sizeof record, file)) {
        perror("fgets");
        fclose(file);
        free(copy);
        return 4;
    }
    fclose(file);
    free(copy);
    puts("Read from virtual file result.txt:");
    fputs(record, stdout);
    puts("View > Program files shows the file and lets you save it on the notebook.");
    return 0;
}
