#ifndef DrawWXR_h
#define DrawWXR_h

#include "Gauge.h"
#include "WXR.h"

namespace ns
{

class DrawWXR : public GaugeComponent
{
public:

  DrawWXR();
  virtual ~DrawWXR();
  
  void Render();
  
  void SetWXRGauge(WXR* WXRGauge) {m_WXRGauge = WXRGauge;}
  
protected:
  
  /** The font number provided to us by the font manager */
  int m_Font;

  /* Pointer to the calling Navigation Gauge */
  WXR* m_WXRGauge;
  
  /* RGBA Array for later OpenGL Rendering
     Use single array instead of 3D array since it has to be contiguous memory */
  unsigned char *wxr_image;
  unsigned char *wxr_image_l;
  unsigned char *wxr_image_r;

  int m_wxr_ncol;
  int m_wxr_nlin;

  float sweepTime = 10.0f; // radar sweep period
                           //
  int countReverse_l = 0; // only reverse the first time
  int countReverse_r = 0;
  int wxr_update_l = 0; // update wxr image only at the start of sweep
  int wxr_update_r = 0; // update wxr image only at the start of sweep

  int wxr_init_l = 0; //
  int wxr_init_r = 0;
  int wxr_newdata_l = 0;
  int wxr_newdata_r = 0;
  float old_heading_l; // save old heading
  float old_heading_r;
};

} // end namespace ns

#endif
