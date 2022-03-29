#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>


#include "myproto.h"

void error_die(char *err)
{
    perror(err);
    exit(EXIT_FAILURE);
}


void send_message(void);

int series_no = 0;
int main(void)
{
    send_message();
    
    return 0;
}

void send_message(void)
{

    srand(clock());
    int body_length = rand() % 255;
    
    mymsg_t hmsg;
    hmsg.verno = 7;
    hmsg.req_type = REQ_TYPE;
    hmsg.data_len = body_length;
    hmsg.series = series_no++;

    int send_length = VBIT + TBIT + DBIT + SBIT + body_length;
    char *send_msg = malloc(send_length * sizeof(char));
    int send_count = 0;
    char *rand_msg;


    char *hmsg_bytes = (char*)&hmsg;
    printf("sizeof mymsg: %ld\n", sizeof(hmsg));

    int hmsg_bytes_len = sizeof(hmsg);
    for (int i = 0; i < hmsg_bytes_len; i++) {
        *(send_msg + send_count++) = *(hmsg_bytes + i);
    }
    
    if (body_length > 32) {
        int rand_msg_len = body_length - 32;
        rand_msg = malloc(rand_msg_len * sizeof(char));
            
        /* if length greater than 32 put random character */
        for (int i = 0; i < rand_msg_len; i++) {
            *(rand_msg + i) = 65 + rand() / (RAND_MAX/(90 - 65 + 1) + 1);
        }
    }

    char *welcome_msg;
    if (body_length > 0) {
        welcome_msg = malloc(32 * sizeof(char));
        char *welcome_str = "Hello Server, This is Client A.";
        for (int i = 0; i < strlen(welcome_str); i++) {
            *(welcome_msg + i) = *(welcome_str + i);
        }

        printf("welcome_msg size: %ld\n", strlen(welcome_msg));

        for (int i = 0; i < 32; i++){
            if (i % 16 == 0) printf("\n");
            printf("%02x:", *(welcome_msg + i));
        }
        printf("\b \n");
    }

    free(send_msg);
    free(rand_msg);
    free(welcome_msg);
}
