#ifndef ogcDrawWXRInfo_h
#define ogcDrawWXRInfo_h

#include "ogcGauge.h"
#include "ogcWXR.h"

namespace OpenGC
{

class DrawWXRInfo : public GaugeComponent
{
public:

  DrawWXRInfo();
  virtual ~DrawWXRInfo();

  void Render();

  void SetWXRGauge(WXR* WXRGauge) {m_WXRGauge = WXRGauge;}

protected:
  
  /** The font number provided to us by the font manager */
  int m_Font;

  /* Pointer to the calling Navigation Gauge */
  WXR* m_WXRGauge;

};

} // end namespace OpenGC

#endif
