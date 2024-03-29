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

  float sweepTime = 10.0f; // radar sweep period
  float minSweep = 52.0f;  // min sweep angle
  float maxSweep = 308.0f;  // max sweep angle
  float wxrSpread = 0.005f;  // radar beam spread
};

} // end namespace ns

#endif
