#ifndef DrawWXRL_h
#define DrawWXRL_h

#include "Gauge.h"
#include "WXR.h"

namespace ns
{

class DrawWXRL : public GaugeComponent
{
public:

  DrawWXRL();
  virtual ~DrawWXRL();
  
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

  int m_wxr_ncol;
  int m_wxr_nlin;

  float sweepTime = 5.0f; // radar sweep period
                           //
  int sweepReverse = 0; // radar sweep reverse 
  int countReverse = 0; // only reverse the first time
  int wxr_update = 0;
  int wxr_update_count = 0;
  int wxr_reverse = 0;
  int wxr_init = 0;
  float old_heading;
};

} // end namespace ns

#endif