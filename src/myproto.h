#ifndef _MYPROTO_
#define _MYPROTO_

#define VBIT 3 /* version number 3 bit */
#define TBIT 2 /* request type 2 bit */
#define DBIT 8 /* data length 8 bit */
#define SBIT 16 /* series number 16 bit*/

#define REQ_TYPE 0 /* request type */
#define RES_TYPE 1 /* response type */

typedef struct mymsg {
    u_int verno:VBIT;
    u_int req_type:TBIT;
    u_int data_len:DBIT;
    u_int series:SBIT;
} mymsg_t;

#endif /* _MYPROTO_ */
