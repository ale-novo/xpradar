/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org
  
  Module:  $RCSfile: opengc_data.hxx,v $

  Copyright (C) 2001-2 by:
    Original author:
      John Wojnaroski
    Contributors (in alphabetical order):

  Last modification:
    Date:      $Date: 2004/10/14 19:28:01 $
    Version:   $Revision: 1.1.1.1 $
    Author:    $Author: damion $
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

=========================================================================*/

/*
This file defines the class/structure of the UDP packet that sends
the simulation data created by FlightGear to the glass displays. It
is required to "sync" the data types contained in the packet

Modified 02/12/01 - Update engine structure for multi-engine models
                  - Added data preamble to id msg types
Modified 01/23/02 - Converted portions of the Engine and Gear accesssors to properties
                  - Removed data from navigation functions. OpenGC provides own
*/

#ifndef _OPENGC_DATA_HXX
#define _OPENGC_DATA_HXX

#ifndef __cplusplus
# error This library requires C++
#endif

namespace OpenGC
{

const int OGC_VERSION = 4;

class FGData {

public:

    // defines msg types and contents. The msg_content is used as a 'pointer' to
    // a predefined set of msg strings
 
    int	version_id;
    int	msg_type;
    int	msg_content;

    // position

    double	latitude;
    double	longitude;
    double	magvar;

    // flight parameters
	
    double	pitch;
    double	bank;
    double	heading;
    double	altitude;
    double	altitude_agl;  // this can also be the radar altimeter
    double  v_kcas;
    double	groundspeed;
    double	vvi;
    double  mach;
    double	v_keas;  // equivalent airspeed in knots

  // Data used by the FMC and autopilots

    double	phi_dot;
    double	theta_dot;
    double	psi_dot;

    double	alpha;
    double	alpha_dot;
    double	beta;
    double	beta_dot;

  // Control surface positions

    double	left_aileron;
    double  right_aileron;
    double	aileron_trim;
    double	elevator;
    double	elevator_trim;
    double	rudder;
    double	rudder_trim;
    double	flaps;
    double  flaps_cmd;

  // gear positions 0 = up and 1 = down  The 747 has 5 wheel bogey assemblies

    double	gear_nose;
    double	gear_left;
    double	gear_right;
    double  gear_left_rear;
    double  gear_right_rear;  

    // engine data

    double	rpm[4];  // this is for pistons, jets see below
    double 	n1_turbine[4];
    double	epr[4];
    double	egt[4];
    double 	n2_turbine[4];
    double	fuel_flow[4];
    double  man_pressure[4];
    double	oil_pressure[4];
    double	throttle[4];
    double	mixture[4];
    double	prop_advance[4];
   
    // Pressures and temperatures

    double	static_temperature;
    double	total_temperature;
    double	static_pressure;
    double	total_pressure;
    double	dynamic_pressure;
    
    // more environmental data
		double wind;
		double wind_dir;
};

} // end namespace OpenGC

#endif // _OPENGC_HXX
