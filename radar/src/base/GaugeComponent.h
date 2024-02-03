/**
 * GaugeComponents are the most "primitive" building block in the
 * OpenGC design scheme. Gauges are composed of a number of
 * GaugeComponents, each of which is capable of positioning and
 * drawing itself.
 *
 * By breaking a gauge into multiple GaugeComponents, recycling
 * of code is encouraged between different gauge designs.
 */

#ifndef GaugeComponent_h
#define GaugeComponent_h

#include "FontManager.h"
#include "DataSource.h"
#include "OrderedPair.h"
#include "RenderObject.h"

namespace OpenGC
{

class GaugeComponent: public RenderObject
{
public:
  GaugeComponent();
  virtual ~GaugeComponent();

  /** handle positioning and clipping */
  void setPosition ();
  void setClip ();
  void unsetClip ();

  /** Render the gauge component */
  virtual void Render();

  /** Return true if the click is inside the gauge component */
  bool ClickTest(int button, int state, int x, int y);
};

} // end namespace OpenGC

#endif

