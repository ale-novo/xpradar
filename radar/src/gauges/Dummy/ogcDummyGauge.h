/**
 * Dummy gauge used for initializing static ogcRenderObject members
 */

#ifndef ogcDummyGauge_h
#define ogcDummyGauge_h

#include "ogcGauge.h"

namespace OpenGC
{

class DummyGauge : public Gauge  
{
public:

  DummyGauge();
  virtual ~DummyGauge();

  void Render();

};

} // end namespace OpenGC

#endif
