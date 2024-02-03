/* This is the udpdata.h header to the udpdata.c code
*/

extern int udpSendBufferLen;
extern int udpRecvBufferLen;
extern char *udpSendBuffer;
extern char *udpRecvBuffer;
extern int udpReadLeft;                      /* counter of bytes to read from receive thread */

/* prototype functions */
void allocate_udpdata(int sendlen,int recvlen);
void get_udpdata(char *data, int len);
void deallocate_udpdata();
