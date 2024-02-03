/* handleclient.c is the header file to handleclient.h
*/

/* General Definitions */
#define MAXCONNECT 20         /* Maximum simultaneous TCP/IP connections to the server */

/* Client Socket Status */

/* The socket is instantiated, bound, and ready	to connect. */
#define status_Ready 1
/* The socket is attempting to connect, but has not yet.  Or
 * an incoming connection has come in. */
#define status_Connecting 2
/* The socket is connected and ready to send data. */
#define status_Connected 3 
/* The other side is disconnected, either because the connection
   failed, the connection was disconnected in a disorderly manner
   from either side, or because both sides issued an orderly
   disconnect. */
#define status_Disconnected 4 
/* An unknown error has happened; the socket is hosed. */
#define status_Error 5

#define TCPBUFSIZE      1000000   /* how much memory (bytes) do we need at maximum for a single transmission */

#define MARK_DATA       1000000
#define MARK_LINK       2000000
#define MARK_UNLINK     3000000
#define MARK_EOT        4000000
#define MARK_DISCONNECT 5000000

extern int socketStatus;

/* prototype functions */
void htonf (float *src, float *dst);
void ntohf (float *src, float *dst);
void receive_client(int clntSock);
void send_client(int clntSock);
void close_client(int clntSock);
