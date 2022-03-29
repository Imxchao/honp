#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tbhdr {
    int a:2;
    int b:2;
    char* s;
};

int main()
{
    int tob = 2312321;

    unsigned char *msg = (unsigned char*)&tob;
    int len = sizeof(msg);
    for(int i = 0; i < len; i++) {
        printf("%02x:", msg[i]);
    }

    printf("\b \n");

    struct tbhdr tb;

    tb.a = 1;
    tb.b = 3;

    tb.s = malloc(32*sizeof(char));

    strcpy(tb.s, "Hello");

    unsigned char *msg_1 = (unsigned char*)&tb;
    int len_1 = sizeof(tb);
    for (int i = 0; i < len_1; i++) {
        printf("%02x:", msg_1[i]);
    }
    printf("\b \n");

    printf("%s\n", (char*)(msg_1 + 7));
    return 0;
}
