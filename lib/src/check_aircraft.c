/* This is the check_aircraft.c code which determines aircraft based on tail number
   Feel free to add your own aircraft. Aircraft type will be available through the
   common.h shared variable acf_type for all xpusb codes
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <sys/types.h>

#include "iniparser.h"
#include "handleserver.h"
#include "serverdata.h"

#include "check_aircraft.h"

int acf_type;

void check_aircraft(void) {

  /* determine ACF by tail number */
  unsigned char *tailnum = link_dataref_byte_arr("sim/aircraft/view/acf_tailnum",  40, -1);

  if (tailnum) {
    // printf("ACF_TAILNUM: %s \n",tailnum);

    /* Add your own number > 3 for any of your favorite aircraft */
    
    if (strcmp((const char*) tailnum,"DO328")==0) {
      /* MG Dornier 328 */
      acf_type = 4;
    } else if (strcmp((const char*) tailnum,"ZB738")==0) {
      /* ZIBO MOD of B737-800 */
      acf_type = 3;
    } else if (strcmp((const char*) tailnum,"N816NN")==0) {
      /* Laminar B738-800 */
      acf_type = 2;
    } else if (strcmp((const char*) tailnum,"OY-GRL")==0) {
      /* Air Greenland Livery of x737 */
      acf_type = 1;
    } else if (strcmp((const char*) tailnum,"D-ATUC")==0) {
      /* Standard x737 */
      acf_type = 1;
    } else if (strcmp((const char*) tailnum,"")==0) {
      /* No Tail Number (yet?) */
      acf_type = -1;
    } else {
      /* Any other X-Plane Aircraft */
      acf_type = 0;
    }    
  } else {
    /* No Allocation for Tail Number Dataref */
    acf_type = -1;
  }
}
