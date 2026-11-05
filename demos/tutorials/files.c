/* This file lives in simulated RAM. View > Panels > Program files shows it.
 * Save file exports a copy only when you choose a destination. */
#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *file = fopen("result.txt", "w+");
    char line[32];
    if (!file)
        return 1;
    if (fputs("The answer is 42.\n", file) == EOF) {
        fclose(file);
        return 2;
    }
    rewind(file);
    if (!fgets(line, sizeof line, file)) {
        fclose(file);
        return 3;
    }
    int valid = strcmp(line, "The answer is 42.\n") == 0;
    if (fclose(file))
        return 4;
    puts(valid ? "Virtual file verified." : "Unexpected file contents.");
    return valid ? 0 : 5;
}
