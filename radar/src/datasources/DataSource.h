#ifndef DataSource_h
#define DataSource_h

// #include "AirframeDataContainer.h"
#include <string>

extern "C" {
#include "serverdata.h"
#include "udpdata.h"
}

namespace xpradar
{

using namespace std;

class DataSource  
{
public:
  DataSource();
  virtual ~DataSource();

  // transfer TCP/IP address and port from namelist
  virtual void define_server(int port, string ip, int radardata);

  /** Initialize the data to a default value*/
  void InitializeData();
  
  /** Called by the base AppObject after all the init parameters
    * have been complete. This should open the connection to the sim
    */
  virtual bool Open();

  /** Called by the render window during idle processing
    * This function is the one and only place where it
    * should acquire data from the sim
    */
  virtual void OnIdle();

  /** Get access to airframe data */
  //  AirframeDataContainer* GetAirframe() { return m_Airframe; }

  /* Determine Aircraft type */
  virtual void SetAcfType(int acfType) { m_AcfType = acfType; }
  virtual int GetAcfType() { return m_AcfType; }
  
protected:

  /** Data that describes the airframe (alt, heading, control surfaces, etc.) */
  //  AirframeDataContainer* m_Airframe;

  int m_AcfType;
  
};

} // end namespace xpradar

#endif

