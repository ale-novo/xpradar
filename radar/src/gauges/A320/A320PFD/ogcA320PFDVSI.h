/*=============================================================================

  This is the ogcA320PFDVSI.h header to ogcA320PFDVSI.cpp

  Created:
    Date:        2011-11-14
    Author:      Hans Jansen
    last change: 2020-02-04
    (see ogcSkeletonGauge.cpp for more details)

  Copyright (C) 2011-2020 Hans Jansen (hansjansen@users.sourceforge.net)
  and/or                  Reto Stöckli (stockli@users.sourceforge.net)

  This program is free software: you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  this program. If not, see <http://www.gnu.org/licenses/>. 

=============================================================================*/

/**
 * Airbus A320 style Primary Flight Display: the Vertical Speed Indicator
 */

#ifndef ogcA320PFDVSI_h
#define ogcA320PFDVSI_h

#include "ogcGaugeComponent.h"
#include "../ogcAirbusColors.h"

extern int verbosity;

namespace OpenGC {

  class A320PFDVSI : public GaugeComponent {

    public:
      A320PFDVSI ();
      virtual ~A320PFDVSI ();
       void Render();

    protected:
      double VSpeedToNeedle(double vspd);
      int m_Font;
      double m_NeedleCenter;
      double m_MaxNeedleDeflection;

  };

} // end namespace OpenGC

#endif
