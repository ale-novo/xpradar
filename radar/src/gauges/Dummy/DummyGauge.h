/**
 * Dummy gauge used for initializing static RenderObject members
 */

#ifndef DummyGauge_h
#define DummyGauge_h

#include "Gauge.h"

namespace ns
{

class DummyGauge : public Gauge  
{
public:

  DummyGauge();
  virtual ~DummyGauge();

  void Render();

};

} // end namespace ns

#endif
