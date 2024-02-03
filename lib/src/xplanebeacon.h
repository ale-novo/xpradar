/* This is the xplanebeacon.h header to the xplanebeacon.c code
*/

/* Parameters which are externally visible */
extern char XPlaneBeaconIP[30];
extern short unsigned int XPlaneBeaconPort;
extern int XPlaneBeaconSocket;

/* Prototype functions which are externally visible */
int initialize_beacon_client(int init_verbose);
void exit_beacon_client(void);

