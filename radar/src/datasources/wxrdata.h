/* This is the wxrdata.h header to the wxrdata.c code
*/

#define WXR_MISS -9999.0
#define MAXRADAR 50 /* maximum number of WXR pixels to retrieve in Type 2 */
#define WXR_UPSCALE 5  /* Upscaling Factor */

extern int wxr_type;
extern float wxr_lonmin;
extern float wxr_lonmax;
extern float wxr_latmin;
extern float wxr_latmax;

extern int wxr_pixperlon;
extern int wxr_pixperlat;
extern int wxr_ncol;
extern int wxr_nlin;

extern int wxr_phase; /* 0: getting bounds, 1: getting data */

extern unsigned char **wxr_data; /* storm level 0-100 */
extern int **wxr_height; /* top height of storm feet a.s.l */
extern int wxr_newdata;

/* prototype functions */

void init_wxr(int type, char server_ip[]);
void read_wxr(void);
void exit_wxr(void);
void nearest_uchar(unsigned char **data, unsigned char **newData, int width, int height, int newWidth, int newHeight);
void nearest_int(int **data, int **newData, int width, int height, int newWidth, int newHeight);
void bilinear_uchar(unsigned char **data, unsigned char **newData, int width, int height, int newWidth, int newHeight);
void bilinear_int(int **data, int **newData, int width, int height, int newWidth, int newHeight);
void bicubic_uchar(unsigned char **data, unsigned char **newData, int width, int height, int newWidth, int newHeight);
void bicubic_int(int **data, int **newData, int width, int height, int newWidth, int newHeight);
