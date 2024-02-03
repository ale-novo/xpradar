/* This is the xpserver.h header file to xpserver.c
*/

#ifndef XPSERVER_EXPORT
#ifdef WIN
#define XPSERVER_EXPORT __declspec(dllexport)
#else
#define XPSERVER_EXPORT __attribute__((visibility("default")))
#endif
#endif

/* TCP/IP Server Definitions */
#define MAXPENDING 5         /* Maximum outstanding connection requests */
#define BLOCKING 0
#define NON_BLOCKING 1

/* TCP/IP server/client definitions */
struct sockaddr_in echoServAddr;    /* Local address */
struct sockaddr_in echoClntAddr;    /* Client address */
int servSock;                       /* Socket descriptor for listening server */
int servSockMax;                    /* Maximum Socket descriptor (depends on number of clients) */
int nConnect;                       /* number of concurrent sockets connected */
fd_set socketSetMaster;             /* Master file descriptor list */
fd_set socketSetRead;               /* temporary file descriptor list */
int clntSockArray[MAXCONNECT];      /* client socket array */           
char clntAddrArray[MAXCONNECT][16]; /* client IP Address array */

/* dynamic data structures for all clients */
clientdata_struct *clientdataArray[MAXCONNECT];
int numallocArray[MAXCONNECT];
int numlinkArray[MAXCONNECT];
char clientnameArray[MAXCONNECT][100];

/* window control */
XPLMWindowID	xpserverWindow = NULL;
int             xpserverCloseWindow = 0;
