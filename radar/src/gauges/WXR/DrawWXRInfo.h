#ifndef DrawWXRInfo_h
#define DrawWXRInfo_h

#include "Gauge.h"
#include "WXR.h"

namespace ns
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

  float sweepAngle = 0.0f; // Current angle of the radar sweep
  float sweepSpeed = 1.0f; // Speed of the sweep (degrees per frame or per update)

};

} // end namespace ns

#endif
