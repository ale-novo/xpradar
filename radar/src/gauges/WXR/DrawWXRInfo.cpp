#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>

#include "GaugeComponent.h"
#include "CircleEvaluator.h"
#include "WXR.h"
#include "DrawWXRInfo.h"

namespace ns
{

  DrawWXRInfo::DrawWXRInfo()
  {
    printf("DrawWXRInfo constructed\n");
    m_Font = m_pFontManager->LoadDefaultFont();
  }

  DrawWXRInfo::~DrawWXRInfo()
  {
    // Destruction handled by base class
  }

  void DrawWXRInfo::Render()
  {
    GaugeComponent::Render();

    float mapRange = m_WXRGauge->GetMapRange();
    float heading_map =  m_WXRGauge->GetMapHeading();
    
    char buffer[10];

    // define geometric stuff
    float fontSize = 4.0 * m_PhysicalSize.x / 150.0;
    float lineWidth = 3.0;

    // double dtor = 0.0174533; /* radians per degree */
    // double radeg = 57.2958;  /* degree per radians */

    // define ACF center position in relative coordinates
    float acf_x = 0.500;
    float acf_y = 0.000;
    float map_y_max = 0.950;
    float map_size = m_PhysicalSize.y*(map_y_max-acf_y);
    
    // What's the heading?
    float *heading_mag = link_dataref_flt("sim/flightmodel/position/mag_psi",-1);
    float *magnetic_variation = link_dataref_flt("sim/flightmodel/position/magnetic_variation",-1);

    // get a time
    float *elapsed = link_dataref_flt("sim/time/local_time_sec", -1);

    // convert to 20 sec periods
    float cycleTime = fmod(*elapsed, 20.0f);
    //printf("Cycle Time: %f seconds\n", cycleTime);

    // Mapping cycleTime to 0-360 range
    float mappedValue = (cycleTime / 20.0f) * 360.0f;
    //printf("Mapped Value: %f\n", mappedValue);


    if ((heading_map != FLT_MISS) && (*magnetic_variation != FLT_MISS)) {

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();

      // Translate the center of rotation to the origin
      glTranslatef(m_PhysicalSize.x/2, 0, 0);
  
      // Rotate around the z-axis, OpenGL expects angles in degrees
      glRotatef(mappedValue, 0, 0, 1);
  
      // Translate back to the original position
      glTranslatef(-m_PhysicalSize.x/2, 0, 0);

      // Set color to black
      glColor3ub(0,0,0);

      // Draw the triangle
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBegin(GL_POLYGON);
        // The vertices should be defined relative to the center of rotation
        glVertex2f(m_PhysicalSize.x/2,0); // This vertex is at the center of rotation
        glVertex2f(m_PhysicalSize.x*-0.5,m_PhysicalSize.y);
        glVertex2f(m_PhysicalSize.x*1.5,m_PhysicalSize.y);
      glEnd();

      glPopMatrix(); // Restore the original model view matrix

      glPushMatrix();

      /* Plot Something in red on the upper left corner */
      /*
      glColor3ub(255,0,0);
      m_pFontManager->SetSize( m_Font, fontSize, fontSize );
      snprintf(buffer, sizeof(buffer), "test");
      m_pFontManager->Print(0.05*m_PhysicalSize.x, 0.9*m_PhysicalSize.y, &buffer[0], m_Font);
      */

      /* Plot Heading in blue on the upper right corner */
      glColor3ub(100,100,255);
      m_pFontManager->SetSize( m_Font, fontSize, fontSize );
      snprintf(buffer, sizeof(buffer), "%03d", (int) lroundf(heading_map));
      m_pFontManager->Print(0.9*m_PhysicalSize.x, 0.9*m_PhysicalSize.y, &buffer[0], m_Font);
      
      // Shift center and rotate about heading
      glTranslatef(m_PhysicalSize.x*acf_x, m_PhysicalSize.y*acf_y, 0.0);
      glRotatef(heading_map + *magnetic_variation, 0, 0, 1);

      /* end of down-shifted coordinate system */
      glPopMatrix();

      /*--------- distance circles ------- */

      // plot range scale
      glPushMatrix();
      glColor3ub(255,255,255);
      
      glTranslatef(m_PhysicalSize.x*acf_x, m_PhysicalSize.y*acf_y, 0);

      m_pFontManager->SetSize( m_Font, 0.75*fontSize, 0.75*fontSize );
      for (int i=1;i<=4;i++) {
	float r = mapRange/4.0*(float) i;
	if (r >= 10.0) {
	  snprintf(buffer, sizeof(buffer), "%i", (int) r);
	} else {
	  snprintf(buffer, sizeof(buffer), "%2.1f", r);
	}
	if (r < 100) {
	  if (r >= 10.0) {
	    m_pFontManager->Print( -0.75*fontSize, map_size/4.0 * (float) i + map_size*0.005, &buffer[0], m_Font);
	  } else {
	    m_pFontManager->Print( -0.75*fontSize, map_size/4.0 * (float) i + map_size*0.005, &buffer[0], m_Font);
	  }
	} else {
	  m_pFontManager->Print( -1.0*fontSize, map_size/4.0 * (float) i + map_size*0.005, &buffer[0], m_Font);
	}
      }
      glPopMatrix();
    
      // Set up big circle for nav range
      glPushMatrix();
	
      glLineWidth(lineWidth);
      glTranslatef(0.0, 0.0, 0.0);
      CircleEvaluator bCircle;
      bCircle.SetDegreesPerPoint(2);

      /*
      glBegin(GL_LINES);
      glVertex2f(m_PhysicalSize.x/2,m_PhysicalSize.y);
      glVertex2f(m_PhysicalSize.x/2,m_PhysicalSize.y*acf_y);
      glEnd();
      */

      glEnable(GL_LINE_STIPPLE);
      glLineStipple( 4, 0x1F1F );

      for (int i=1;i<=4;i++) {
	bCircle.SetArcStartEnd(270,90);
	bCircle.SetRadius(map_size*0.25 * (float) i);
	bCircle.SetOrigin(m_PhysicalSize.x*acf_x, m_PhysicalSize.y*acf_y);
	glBegin(GL_LINE_STRIP);
	bCircle.Evaluate();
	glEnd();
      }

      glDisable(GL_LINE_STIPPLE);

      /* Remove Cone area outside of WXR image */
      glColor3ub(0,0,0);

      // Center
      /*
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBegin(GL_POLYGON);
      glVertex2f(m_PhysicalSize.x/2,0);
      glVertex2f(m_PhysicalSize.x*0.66,m_PhysicalSize.y);
      glVertex2f(m_PhysicalSize.x*0.33,m_PhysicalSize.y);
      glEnd();
      */

      // Lower Left Corner
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBegin(GL_POLYGON);
      glVertex2f(0,m_PhysicalSize.y/2);
      glVertex2f(0,m_PhysicalSize.y*acf_y);
      glVertex2f(m_PhysicalSize.x/2,m_PhysicalSize.y*acf_y);
      glEnd();
      
      // Lower Right Corner
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBegin(GL_POLYGON);
      glVertex2f(m_PhysicalSize.x,m_PhysicalSize.y/2);
      glVertex2f(m_PhysicalSize.x,m_PhysicalSize.y*acf_y);
      glVertex2f(m_PhysicalSize.x/2,m_PhysicalSize.y*acf_y);
      glEnd();

      glPopMatrix();   
	
    }
      
  }

} // end namespace ns
