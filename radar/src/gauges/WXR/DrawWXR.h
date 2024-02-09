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

  int m_wxr_ncol;
  int m_wxr_nlin;

};

} // end namespace ns

#endif
