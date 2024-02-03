/* This is the xplanewxr.h header to the xplanewxr.c code
*/

#ifdef WIN
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

/* UDP CLIENT PARAMETERS */
extern char wxrServerIP[30];
extern int wxrServerPort;
extern int wxrSocket;

/* thread parameters */
extern pthread_t wxr_poll_thread;                /* read thread */
extern int wxr_poll_thread_exit_code;            /* read thread exit code */
extern pthread_mutex_t wxr_exit_cond_lock;

extern struct sockaddr_in wxrServerAddr;     /* Server address structure */
extern struct sockaddr_in wxrClientAddr;     /* Client address structure */

extern int wxrSendBufferLen;
extern int wxrRecvBufferLen;
extern char *wxrSendBuffer;
extern char *wxrRecvBuffer;
extern int wxrReadLeft;                      /* counter of bytes to read from receive thread */
extern int wxr_is_xp12;   /* WXR data is from XP12 instead from XP11 */

/* Prototype functions */
int init_wxr_server(void);
int init_wxr_client(void);
int init_wxr_receive();
void *wxr_poll_thread_main();
void exit_wxr_server(void);
void exit_wxr_client(void);
int send_wxr_to_server(void);
int recv_wxr_from_server(void);
int send_wxr_to_client(char client_ip[], int client_port, char data[], int len);
int recv_wxr_from_client(void);

void allocate_wxrdata(int sendlen,int recvlen);
void get_wxrdata(char *data, int len);
void deallocate_wxrdata();
