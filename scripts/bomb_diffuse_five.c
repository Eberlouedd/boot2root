#include <stdio.h>
#include <string.h>

int main(void)
{
    const char table[] = "isrveawhobpnutfg";
    const char target[] = "giants";

    char params[7];

    for (int i = 0; i < 6; i++) {

        for (int c = 32; c <= 126; c++) {

            if (table[c & 0xf] == target[i]) {
                params[i] = c;
                break;
            }
        }
    }

    params[6] = '\0';

    printf("Solution : %s\n", params);

    return 0;
}