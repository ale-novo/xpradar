/*
This file defines the class/structure of the UDP packet that sends
the simulation data created by FlightGear to the glass displays. It
is required to "sync" the data types contained in the packet
*/

#ifndef WXR_h
#define WXR_h

#include <Gauge.h>

namespace ns
{

class WXR : public Gauge  
{
public:

  WXR();
  virtual ~WXR();

  void Render();

  float GetMapRange(void) {return m_MapRange;}
  double GetMapCtrLon(void) {return m_MapCtrLon;}
  double GetMapCtrLat(void) {return m_MapCtrLat;}
  void SetMapCtrLon(double lon) {m_MapCtrLon=lon;}
  void SetMapCtrLat(double lat) {m_MapCtrLat=lat;}
  float GetMapHeading(void) {return m_MapHeading;}
 
 protected:

  // The font number
  int m_Font;

  float m_MapRange; // NAV Display Range in [nm]
  double m_MapCtrLon; // NAV Display Center Longitude [deg E]
  double m_MapCtrLat; // NAV Display Center Latitude [deg N]
  float m_MapHeading; // True Heading of MAP (=0 or N in Plan Mode)
  
};

} // end namespace ns

#endif
