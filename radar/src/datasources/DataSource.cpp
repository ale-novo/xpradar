#include "DataSource.h"
#include <stdio.h>

namespace ns
{

DataSource::DataSource()
{
  printf("DataSource constructed\n");

  //  m_Airframe = new AirframeDataContainer;
}

DataSource::~DataSource()
{
  //  delete m_Airframe;
}

// Initialize the data
void DataSource::InitializeData()
{
  //  m_Airframe->InitializeData();
}

void DataSource::OnIdle()
{
  // The default version does nothing
}

  void DataSource::define_server(int port, string ip_address, int radardata)
{
  printf("WRONG SOURCE");
  // The default version does nothing
}

bool DataSource::Open()
{
	// This is redefined in FGDatasource.cpp so as to delay opening the socket until all file and
	// command options have been tested for network parameters
  
  // The default version returns false, which doesn't necessarily mean anything
  return false;
}

} // end namespace ns
