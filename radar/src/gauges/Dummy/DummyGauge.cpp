#include "Dummy/DummyGauge.h"

namespace ns
{

DummyGauge::DummyGauge()
{
  printf("DummyGauge constructed\n");
}

DummyGauge::~DummyGauge()
{
// Destruction handled by base class
}

void DummyGauge::Render()
{
// Rendering the dummy class does nothing
}

} // end namespace ns
