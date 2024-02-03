/* This is the handleserver.h header to the handleserver.c code
*/

#define MARK_DATA       1000000
#define MARK_LINK       2000000
#define MARK_UNLINK     3000000
#define MARK_EOT        4000000
#define MARK_DISCONNECT 5000000

/* The socket is connected to XPServer and ready to send/receive data. */
#define status_Connected 1
/* The socket is instantiated, bound, and ready	to connect. */
#define status_Ready 2  
/* The socket is not ready */
#define status_Init 3
/* An unknown error has happened; the socket is doomed. */
#define status_Error 4

/* TCP/IP CLIENT PARAMETERS */
/* Global variable declaration */
extern char XPlaneServerIP[30];
extern short unsigned int XPlaneServerPort;

/* TCP/IP PROTOTYPE FUNCTIONS */

/* Prototype functions for Network communication */
int initialize_tcpip_client(int init_verbose);
int create_tcpip_socket(void);
void exit_tcpip_client(void);
int check_xpserver(void);
int send_xpserver(void);
int receive_xpserver(void);
