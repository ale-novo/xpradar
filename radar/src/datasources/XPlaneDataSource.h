#ifndef XPlaneDataSource_h
#define XPlaneDataSource_h

#include <string>

namespace OpenGC
{

using namespace std;

class XPlaneDataSource : public DataSource
{
public:
  XPlaneDataSource();
  virtual ~XPlaneDataSource();

  // transfer TCP/IP address and port from namelist
  void define_server(int port, string ip_address, int radardata);

  // The "money" function
  void OnIdle();
 
protected:
  
};

} // end namespace OpenGC

#endif
