#ifndef ogcNavRadioDataContainer_h
#define ogcNavRadioDataContainer_h

#include "ogcDataContainer.h"

namespace OpenGC
{

class NavRadioDataContainer : public DataContainer
{

  /** Frequency that the nav radio is tuned to */
  DataReadWriteMacro(Frequency, double);

  /** Omni bearing select */
  DataReadWriteMaco(OBS, double);

  /** Radial being received, if VOR */
  DataReadMacro(Radial, double);

public:
  
  void InitializeData();

  NavRadioDataContainer();
  virtual ~NavRadioDataContainer();

protected:

};

} // end namespace OpenGC
#endif
