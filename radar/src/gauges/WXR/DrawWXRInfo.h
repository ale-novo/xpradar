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

  float sweepTime = 5.0f; // radar sweep period
  float minSweep = 55.0f;  // min sweep angle
  float maxSweep = 305.0f;  // max sweep angle
  float wxrSpread = 0.01f;  // radar beam spread
			   //
  int sweepReverse = 0; // radar sweep reverse 
  int countReverse = 0; // only reverse the first time
};

} // end namespace ns

#endif
