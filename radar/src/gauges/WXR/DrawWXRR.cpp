#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>

#include "GaugeComponent.h"
#include "CircleEvaluator.h"
#include "WXR/WXR.h"
#include "WXR/DrawWXRR.h"

extern "C" {
  #include "wxrdata.h"
}

namespace ns
{
  DrawWXRR::DrawWXRR()
  {
    printf("DrawWXRR constructed\n");
    
    m_Font = m_pFontManager->LoadDefaultFont();
    m_WXRGauge = NULL;
    m_wxr_ncol = 0;
    m_wxr_nlin = 0;
    wxr_image = NULL;
  }

  DrawWXRR::~DrawWXRR() // Destruction handled by base class
  {
  }
 
  void DrawWXRR::Render()
  {
    GaugeComponent::Render();

    float mapRange = m_WXRGauge->GetMapRange();
 
    // define geometric stuff
    float fontSize = 4.0 * m_PhysicalSize.x / 150.0;

    int i;
    int j;
    
    // define ACF center position in relative coordinates: bottom center
    float acf_x = 0.500;
    float acf_y = 0.000;
    float map_y_max = 0.950;
    float map_size = m_PhysicalSize.y*(map_y_max-acf_y);

    // Get information on what dynamic information we display on NAV MAP
 
    // Where is the aircraft?
    double aircraftLon = m_WXRGauge->GetMapCtrLon();
    double aircraftLat = m_WXRGauge->GetMapCtrLat();

    // What's the heading?
    float heading_map =  m_WXRGauge->GetMapHeading();
     // What's the altitude? (feet) --> needed to calculate effect of tilting the radar image
    float *pressure_altitude = link_dataref_flt("sim/flightmodel/misc/h_ind",0);
  
    /* Sample Datarefs for controlling WXR gain and tilt */
    float *wxr_gain = link_dataref_flt("xpserver/wxr_gain",-2); /* Gain should go from 0.1 .. 2.0 */
    float *wxr_tilt = link_dataref_flt("xpserver/wxr_tilt",-2); /* Tilt in degrees up/down : not implemented yet */

    // get a time
    float *elapsed = link_dataref_flt("sim/time/local_time_sec", -1);

    // convert to periods
    float cycleTime = fmod(*elapsed, sweepTime);
    //printf("Cycle Time: %f seconds\n", cycleTime);

    // The input coordinates are in lon/lat, so we have to rotate against true heading
    // despite the NAV display is showing mag heading
    if ((heading_map != FLT_MISS) && (wxr_data)) {
      // Shift center and rotate about heading
      glMatrixMode(GL_MODELVIEW);

      /* valid coordinates and full radar image received */
      if ((aircraftLon >= -180.0) && (aircraftLon <= 180.0) &&
	  (aircraftLat >= -90.0) && (aircraftLat <= 90.0)) {
	
	/* Copy WXR data into GL Texture Array */
	float textureCenterLon = (float) wxr_lonmin + (float) wxr_ncol / (float) wxr_pixperlon * 0.5;
	float textureCenterLat = (float) wxr_latmin + (float) wxr_nlin / (float) wxr_pixperlat * 0.5;

	/* miles per radar pixel. Each pixel .
	   Each degree lat is 111 km apart and each mile is 1.852 km */
	float mpplon =  111.0 / (float) wxr_pixperlon / 1.852;
	float mpplat =  111.0 / (float) wxr_pixperlat / 1.852;

	/* free WXR array and recreate it if we have new WXR data */
	if (wxr_newdata == 1 && wxr_update == 1) {
	  printf("Plotting New WXR Data in NAV Display\n");
	  m_wxr_ncol = wxr_ncol;
	  m_wxr_nlin = wxr_nlin;
	  if (wxr_image) free(wxr_image);
	  wxr_image = NULL;
	  wxr_image = (unsigned char*)malloc(m_wxr_nlin * m_wxr_ncol * 4 * sizeof(unsigned char));

	  float gain = *wxr_gain;
	  if (gain == FLT_MISS) gain = 1.0;
	  if (gain < 0.1) gain = 0.1;
	  if (gain > 2.0) gain = 2.0;
	
	  /* copy temporary WXR array to WXR array */
	  /* TODO: OPENGL Transparency not working */
	  /* TODO: Only create image if data has changed */

	  for (i = 0; i < m_wxr_nlin; i++) {
	    for (j = 0; j < m_wxr_ncol; j++) {
	      
	      if (wxr_data[i][j]*gain == 0) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Transparent */
	      } else if (wxr_data[i][j]*gain <= 10) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Transparent */
	      } else if (wxr_data[i][j]*gain <= 20) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Transparent */
	      } else if (wxr_data[i][j]*gain <= 30) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Transparent */
	      } else if (wxr_data[i][j]*gain <= 40) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Non-Transparent */
	      } else if (wxr_data[i][j]*gain <= 50) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Non-Transparent */
	      } else if (wxr_data[i][j]*gain <= 60) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Non-Transparent */
	      } else if (wxr_data[i][j]*gain <= 70) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Non-Transparent */
	      } else if (wxr_data[i][j]*gain <= 80) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Non-Transparent */
	      } else if (wxr_data[i][j]*gain <= 90) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Non-Transparent */
	      } else {
		/* Magenta for Turbulence, not implemented in X-Plane, but take the highest level */
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 200;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 45;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 200;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; /* Non-Transparent */
	      }
	    }
	  }
	  wxr_newdata = 0;
	}
	wxr_update = 0;

        glPushMatrix();

        // SCISSOR half of the screen
        glEnable(GL_SCISSOR_TEST);

        glTranslatef(m_PhysicalSize.x*acf_x, m_PhysicalSize.y*acf_y, 0.0);

	if (wxr_init == 0) {
          glRotatef((int) lroundf(heading_map), 0, 0, 1);
          old_heading = heading_map;
	  wxr_init = 1;
	  //printf("rotate init\n");
        }

        if (cycleTime >= 0.0f && cycleTime < 1.0f) {
          if (countReverse == 0) {

            wxr_reverse = 1 - wxr_reverse;

            if ( wxr_update_count == 1) {
              glRotatef((int) lroundf(heading_map), 0, 0, 1);
              old_heading = heading_map;
              printf("Update radar to new heading\n");
              wxr_update_count = 0;
              wxr_update = 1;

            } else {
              wxr_update_count = wxr_update_count + 1;
              glRotatef((int) lroundf(old_heading), 0, 0, 1);
	    }

          } else {
            glRotatef((int) lroundf(old_heading), 0, 0, 1);
          }
          countReverse = countReverse + 1;

        } else {
          glRotatef((int) lroundf(old_heading), 0, 0, 1);
          countReverse = 0;
	  //printf("rotate old heading\n");
        }

        float t;
        //if (wxr_reverse == 0) { //reverse radar update point
        if (wxr_reverse == 1) {
            t = cycleTime / 5.0f;
        } else {
            t = 1.0f - (cycleTime / 5.0f);
        }

        int scaledPixelSizeX = (int)(m_PixelSize.x * t);

        //glScissor(0, 0, scaledPixelSizeX, m_PixelSize.y); // reverse black sweep anim
        glScissor(scaledPixelSizeX, 0, m_PixelSize.x, m_PixelSize.y );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Remove border line */
	GLfloat color[4]={0,0,0,1};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA,
		      m_wxr_ncol,  m_wxr_nlin, 0, GL_RGBA,
		      GL_UNSIGNED_BYTE, wxr_image);

	float scx = 0.5 * ((float) m_wxr_ncol) * mpplon / mapRange * map_size * cos(M_PI / 180.0 * aircraftLat);
	float scy = 0.5 * ((float) m_wxr_nlin) * mpplat / mapRange * map_size;
	float tx = (textureCenterLon - aircraftLon) * ((float) wxr_pixperlon) * mpplon / mapRange * map_size *
	  cos(M_PI / 180.0 * aircraftLat);
	float ty = (textureCenterLat - aircraftLat) * ((float) wxr_pixperlat) * mpplat / mapRange * map_size;
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBegin(GL_TRIANGLES);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-scx+tx,  scy+ty);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-scx+tx, -scy+ty);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( scx+tx,  scy+ty);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( scx+tx,  scy+ty);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-scx+tx, -scy+ty);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( scx+tx, -scy+ty);
	glEnd();

	glDisable (GL_TEXTURE_2D);
	glFlush();

	/* end of down-shifted and rotated coordinate system */
	glPopMatrix();

	/* Cover radar image behind aircraft and beyond range of XX NM*/

	glPushMatrix();

	glColor3ub(0,0,0);
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(0,m_PhysicalSize.y*acf_y);
	glVertex2f(m_PhysicalSize.x,m_PhysicalSize.y*acf_y);
	glVertex2f(m_PhysicalSize.x,0);
	glEnd();
	
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2f(0,m_PhysicalSize.y);
	glVertex2f(0,m_PhysicalSize.y*map_y_max);
	glVertex2f(m_PhysicalSize.x,m_PhysicalSize.y*map_y_max);
	glVertex2f(m_PhysicalSize.x,m_PhysicalSize.y);
	glEnd();
	
	//-------------------Rounded NAV Gauge WXR Limit ------------------
	// The WXR image is blacked off on the top of the NAV gauge
	// The overlays are essentially the
	// remainder of a circle subtracted from a square, and are formed
	// by fanning out triangles from a point just off each corner
	// to an arc descrbing the curved portion of the art. horiz.

	CircleEvaluator aCircle;
	aCircle.SetRadius(m_PhysicalSize.y*(map_y_max-acf_y));
	
	// Upper Left quarter
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0,m_PhysicalSize.y*map_y_max);
	aCircle.SetOrigin(m_PhysicalSize.x*acf_x,m_PhysicalSize.y*acf_y);
	aCircle.SetArcStartEnd(270,360);
	aCircle.SetDegreesPerPoint(10);
	aCircle.Evaluate();
	glEnd();

	// Upper Right quarter
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(m_PhysicalSize.x,m_PhysicalSize.y*map_y_max);
	aCircle.SetOrigin(m_PhysicalSize.x*acf_x,m_PhysicalSize.y*acf_y);
	aCircle.SetArcStartEnd(0,90);
	aCircle.SetDegreesPerPoint(10);
	aCircle.Evaluate();
	glEnd();

	// The WXR Image is a Cone, so cut triangular areas away from ACF
	
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

        glDisable(GL_SCISSOR_TEST);
	glPopMatrix();
	
      } // valid acf coordinates
    } // known heading and WXR data allocated
  }
} // end namespace ns
