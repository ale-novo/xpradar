#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>

#include "GaugeComponent.h"
#include "CircleEvaluator.h"
#include "WXR/WXR.h"
#include "WXR/DrawWXR.h"

extern "C" {
  #include "wxrdata.h"
}

namespace ns
{
  DrawWXR::DrawWXR()
  {
    printf("DrawWXR constructed\n");
    
    m_Font = m_pFontManager->LoadDefaultFont();
    m_WXRGauge = NULL;
    m_wxr_ncol = 0;
    m_wxr_nlin = 0;
    wxr_image = NULL;

    // init wxr_image_l and r as black.
    int wxr_col = 1200;
    int wxr_lin = 1200;

    int i = 0;
    int j = 0;

    wxr_image_l = (unsigned char*)malloc(wxr_lin * wxr_col * 4 * sizeof(unsigned char));
    wxr_image_r = (unsigned char*)malloc(wxr_lin * wxr_col * 4 * sizeof(unsigned char));

    for (i = 0; i < wxr_lin; i++) {
      for (j = 0; j < wxr_col; j++) {
          wxr_image_l[i*4*wxr_col+j*4+0] = 0;
          wxr_image_l[i*4*wxr_col+j*4+1] = 0;
          wxr_image_l[i*4*wxr_col+j*4+2] = 0;
          wxr_image_l[i*4*wxr_col+j*4+3] = 255; /* Transparent */

          wxr_image_r[i*4*wxr_col+j*4+0] = 0;
          wxr_image_r[i*4*wxr_col+j*4+1] = 0;
          wxr_image_r[i*4*wxr_col+j*4+2] = 0;
          wxr_image_r[i*4*wxr_col+j*4+3] = 255; /* Transparent */
      }
    }

  }

  DrawWXR::~DrawWXR() // Destruction handled by base class
  {
  }
 
  void DrawWXR::Render()
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

    float halfSweepTime = sweepTime * 0.5;
    float quarterSweepTime = sweepTime * 0.25;
    float treeqSweepTime = sweepTime * 0.75;

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
	if (wxr_newdata == 1) {

          wxr_newdata_l = 1;
	  wxr_newdata_r = 1;

	  printf("Plotting New WXR Data\n");
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

// LEFT
        glPushMatrix();
        // SCISSOR half of the screen
        glEnable(GL_SCISSOR_TEST);

        glTranslatef(m_PhysicalSize.x*acf_x, m_PhysicalSize.y*acf_y, 0.0);

	if (wxr_init_l == 0) {
          glRotatef((int) lroundf(heading_map), 0, 0, 1);
          old_heading_l = heading_map;
	  old_range_l = mapRange;
	  wxr_init_l = 1;
        }

        if (cycleTime >= 0.0f && cycleTime < 1.0f) {
          if (countReverse_l == 0) {
            glRotatef((int) lroundf(heading_map), 0, 0, 1);
            old_heading_l = heading_map;
            old_range_l = mapRange;
            //printf("Update radar L to new heading\n");
            wxr_update_l = 1;

          } else {
            glRotatef((int) lroundf(old_heading_l), 0, 0, 1);
          }
          countReverse_l = countReverse_l + 1;

        } else {
          glRotatef((int) lroundf(old_heading_l), 0, 0, 1);
          countReverse_l = 0;
        }

/*
    float lnormalizedTime;
    float langle; // Angle in radians
    float lscaledPixelSizeX;

    float lminAngle = 0 * PI / 180;
    float lmaxAngle = 52 * PI / 180;

    if (cycleTime <= halfSweepTime) {
        lnormalizedTime = cycleTime / halfSweepTime;
        langle = lminAngle + (lmaxAngle - lminAngle) * lnormalizedTime;
    } else {
        lnormalizedTime = (cycleTime - halfSweepTime) / halfSweepTime;
        langle = lmaxAngle - (lmaxAngle - lminAngle) * lnormalizedTime;
    }
    printf("angle %f\n", langle);

    lscaledPixelSizeX = (cos(langle) + 1) / 2 * m_PixelSize.x;
    glScissor(0, 0, lscaledPixelSizeX, m_PixelSize.y); // reverse black sweep anim
*/

	//
        float tl;

        if (cycleTime <= halfSweepTime) {
          tl = cycleTime / halfSweepTime;
        } else {
          tl = 1.0f - ((cycleTime - halfSweepTime) / halfSweepTime);
        }

        int lscaledPixelSizeX = (int)(m_PixelSize.x * tl);

        glScissor(0, 0, lscaledPixelSizeX, m_PixelSize.y); // reverse black sweep anim

	//
	
        if ( wxr_update_l == 1 && wxr_image ) {
          if (wxr_newdata_l == 1) {

            if (wxr_image_l) free(wxr_image_l);
            wxr_image_l = NULL;
            wxr_image_l = (unsigned char*)malloc(m_wxr_nlin * m_wxr_ncol * 4 * sizeof(unsigned char));
            printf("Update WXR image LEFT\n");
 
            memcpy(wxr_image_l, wxr_image, m_wxr_nlin * m_wxr_ncol * 4 * sizeof(unsigned char));
            wxr_newdata_l = 0;
          }
          wxr_update_l = 0;
        }

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Remove border line
	GLfloat lcolor[4]={0,0,0,1};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, lcolor);
	
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA,
		      m_wxr_ncol,  m_wxr_nlin, 0, GL_RGBA,
		      GL_UNSIGNED_BYTE, wxr_image_l);

	float lscx = 0.5 * ((float) m_wxr_ncol) * mpplon / old_range_l * map_size * cos(M_PI / 180.0 * aircraftLat);
	float lscy = 0.5 * ((float) m_wxr_nlin) * mpplat / old_range_l * map_size;
	float ltx = (textureCenterLon - aircraftLon) * ((float) wxr_pixperlon) * mpplon / old_range_l * map_size *
	  cos(M_PI / 180.0 * aircraftLat);
	float lty = (textureCenterLat - aircraftLat) * ((float) wxr_pixperlat) * mpplat / old_range_l * map_size;
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBegin(GL_TRIANGLES);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-lscx+ltx,  lscy+lty);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-lscx+ltx, -lscy+lty);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( lscx+ltx,  lscy+lty);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( lscx+ltx,  lscy+lty);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-lscx+ltx, -lscy+lty);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( lscx+ltx, -lscy+lty);
	glEnd();

	glDisable (GL_TEXTURE_2D);
	glFlush();

	// end of down-shifted and rotated coordinate system
        glDisable(GL_SCISSOR_TEST);
	glPopMatrix();

// RIGHT

        glPushMatrix();
        // SCISSOR half of the screen
        glEnable(GL_SCISSOR_TEST);

        glTranslatef(m_PhysicalSize.x*acf_x, m_PhysicalSize.y*acf_y, 0.0);

	if (wxr_init_r == 0) {
          glRotatef((int) lroundf(heading_map), 0, 0, 1);
          old_heading_r = heading_map;
          old_range_r = mapRange;
	  wxr_init_r = 1;
        }

        if (cycleTime >= 5.0f && cycleTime < 6.0f) {
          if (countReverse_r == 0) {
            glRotatef((int) lroundf(heading_map), 0, 0, 1);
            old_heading_r = heading_map;
            old_range_r = mapRange;
            //printf("Update radar R to new heading\n");
            wxr_update_r = 1;

          } else {
            glRotatef((int) lroundf(old_heading_r), 0, 0, 1);
          }
          countReverse_r = countReverse_r + 1;

        } else {
          glRotatef((int) lroundf(old_heading_r), 0, 0, 1);
          countReverse_r = 0;
        }

/*
    float rnormalizedTime;
    float rangle; // Angle in radians
    float rscaledPixelSizeX;

    float rminAngle = 0 * PI / 180;
    float rmaxAngle = 52 * PI / 180;

    if (cycleTime <= halfSweepTime) {
        rnormalizedTime = cycleTime / halfSweepTime;
        rangle = rminAngle + (rmaxAngle - rminAngle) * rnormalizedTime;
    } else {
        rnormalizedTime = (cycleTime - halfSweepTime) / halfSweepTime;
        rangle = rmaxAngle - (rmaxAngle - rminAngle) * rnormalizedTime;
    }
    rscaledPixelSizeX = (cos(rangle) + 1) / 2 * m_PixelSize.x;
    glScissor(rscaledPixelSizeX, 0, m_PixelSize.x, m_PixelSize.y );
*/

	//
        float tr;

        if (cycleTime <= halfSweepTime) {
          tr = cycleTime / halfSweepTime;
        } else {
          tr = 1.0f - ((cycleTime - halfSweepTime) / halfSweepTime);
        }

        int rscaledPixelSizeX = (int)(m_PixelSize.x * tr);

        glScissor(rscaledPixelSizeX, 0, m_PixelSize.x, m_PixelSize.y ); //reverse black anim

	//
	
	if ( wxr_update_r == 1 && wxr_image ) {
          if (wxr_newdata_r == 1) {

            if (wxr_image_r) free(wxr_image_r);
            wxr_image_r = NULL;
            wxr_image_r = (unsigned char*)malloc(m_wxr_nlin * m_wxr_ncol * 4 * sizeof(unsigned char));
            printf("Update WXR image RIGHT\n");

	    memcpy(wxr_image_r, wxr_image, m_wxr_nlin * m_wxr_ncol * 4 * sizeof(unsigned char));
            wxr_newdata_r = 0;
          }
          wxr_update_r = 0;
        }
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Remove border line
	GLfloat rcolor[4]={0,0,0,1};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, rcolor);
	
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA,
		      m_wxr_ncol,  m_wxr_nlin, 0, GL_RGBA,
		      GL_UNSIGNED_BYTE, wxr_image_r);

	float rscx = 0.5 * ((float) m_wxr_ncol) * mpplon / old_range_r * map_size * cos(M_PI / 180.0 * aircraftLat);
	float rscy = 0.5 * ((float) m_wxr_nlin) * mpplat / old_range_r * map_size;
	float rtx = (textureCenterLon - aircraftLon) * ((float) wxr_pixperlon) * mpplon / old_range_r * map_size *
	  cos(M_PI / 180.0 * aircraftLat);
	float rty = (textureCenterLat - aircraftLat) * ((float) wxr_pixperlat) * mpplat / old_range_r * map_size;
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBegin(GL_TRIANGLES);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-rscx+rtx,  rscy+rty);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-rscx+rtx, -rscy+rty);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( rscx+rtx,  rscy+rty);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( rscx+rtx,  rscy+rty);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-rscx+rtx, -rscy+rty);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( rscx+rtx, -rscy+rty);
	glEnd();

	glDisable (GL_TEXTURE_2D);
	glFlush();
        glDisable(GL_SCISSOR_TEST);

	// end of down-shifted and rotated coordinate system
	glPopMatrix();
//

	// Cover radar image behind aircraft and beyond range of XX NM
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

//        glDisable(GL_SCISSOR_TEST);
	glPopMatrix();
	
      } // valid acf coordinates
    } // known heading and WXR data allocated
  }
} // end namespace ns
