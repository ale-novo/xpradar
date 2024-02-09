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

#define DEG_TO_RAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)

#include <cmath>
#include <cstdlib>

#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0)

unsigned char* get_lines_from_angles(unsigned char* wxr_image, int m_wxr_nlin, int m_wxr_ncol, int center_row, int center_col, int angle) {
    int max_length = sqrt(m_wxr_nlin * m_wxr_nlin + m_wxr_ncol * m_wxr_ncol); // Max possible length from center
    // Allocating for 3 lines, each max_length long, with 4 bytes per pixel (RGBA), times 3 for three angles
    unsigned char* lines = (unsigned char*)malloc(max_length * 4 * 3 * sizeof(unsigned char));

    // Process three angles: angle - 1, angle, angle + 1
    for (int a = -1; a <= 1; a++) {
        float angle_rad = DEG_TO_RAD(angle + a);

        float step_row = -cos(angle_rad); // Negative because image coordinates go down as row number increases
        float step_col = sin(angle_rad);

        for (int i = 0; i < max_length; i++) {
            int row = round(center_row + i * step_row);
            int col = round(center_col + i * step_col);

            // Boundary check to stop if we reach the edge of the image
            if (row < 0 || col < 0 || row >= m_wxr_nlin || col >= m_wxr_ncol) break;

            for (int j = 0; j < 4; j++) {
                // Calculating index: Each line stored next to each other for the same pixel (i)
                // Adjusting index to pack data for three angles side by side
                int index = (i * 4 * 3) + (a + 1) * 4 + j; // (a + 1) to shift from 0-2 for -1, 0, +1 angles
                lines[index] = wxr_image[(row * m_wxr_ncol + col) * 4 + j];
            }
        }
    }

    return lines;
}

float calculateAircraftOriginalColumn(double aircraftLon, float textureCenterLon, int wxr_pixperlon, int m_wxr_ncol) {
    float diff_lon_pixels = (aircraftLon - textureCenterLon) * wxr_pixperlon;
    float orig_col = (m_wxr_ncol / 2.0) + diff_lon_pixels;
    return orig_col;
}

// Function to calculate the original line of the aircraft in the wxr image
float calculateAircraftOriginalLine(double aircraftLat, float textureCenterLat, int wxr_pixperlat, int m_wxr_nlin) {
    float diff_lat_pixels = (aircraftLat - textureCenterLat) * wxr_pixperlat;
    float orig_line = (m_wxr_nlin / 2.0) + diff_lat_pixels;
    return orig_line;
}

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
          wxr_image_l[i*4*wxr_col+j*4+3] = 255; // Transparent 

          wxr_image_r[i*4*wxr_col+j*4+0] = 0;
          wxr_image_r[i*4*wxr_col+j*4+1] = 0;
          wxr_image_r[i*4*wxr_col+j*4+2] = 0;
          wxr_image_r[i*4*wxr_col+j*4+3] = 255; // Transparent 
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
  
    // Sample Datarefs for controlling WXR gain and tilt 
    float *wxr_gain = link_dataref_flt("radar/wxr/gain",-2); // Gain should go from 0.1 .. 2.0
    float *wxr_tilt = link_dataref_flt("radar/wxr/tilt",-2); // Tilt in degrees up/down : not implemented yet

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

      // valid coordinates and full radar image received
      if ((aircraftLon >= -180.0) && (aircraftLon <= 180.0) &&
	  (aircraftLat >= -90.0) && (aircraftLat <= 90.0)) {
	
	// Copy WXR data into GL Texture Array
	float textureCenterLon = (float) wxr_lonmin + (float) wxr_ncol / (float) wxr_pixperlon * 0.5;
	float textureCenterLat = (float) wxr_latmin + (float) wxr_nlin / (float) wxr_pixperlat * 0.5;

	// miles per radar pixel. Each pixel .
	//  Each degree lat is 111 km apart and each mile is 1.852 km
	float mpplon =  111.0 / (float) wxr_pixperlon / 1.852;
	float mpplat =  111.0 / (float) wxr_pixperlat / 1.852;

	// free WXR array and recreate it if we have new WXR data
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
	
	  // copy temporary WXR array to WXR array
	  // TODO: OPENGL Transparency not working
	  // TODO: Only create image if data has changed

	  for (i = 0; i < m_wxr_nlin; i++) {
	    for (j = 0; j < m_wxr_ncol; j++) {
	      
	      if (wxr_data[i][j]*gain == 0) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Transparent
	      } else if (wxr_data[i][j]*gain <= 10) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Transparent
	      } else if (wxr_data[i][j]*gain <= 20) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Transparent
	      } else if (wxr_data[i][j]*gain <= 30) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Transparent
	      } else if (wxr_data[i][j]*gain <= 40) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Non-Transparent
	      } else if (wxr_data[i][j]*gain <= 50) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Non-Transparent
	      } else if (wxr_data[i][j]*gain <= 60) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Non-Transparent
	      } else if (wxr_data[i][j]*gain <= 70) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Non-Transparent
	      } else if (wxr_data[i][j]*gain <= 80) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Non-Transparent
	      } else if (wxr_data[i][j]*gain <= 90) {
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 250;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 0;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Non-Transparent
	      } else {
		// Magenta for Turbulence, not implemented in X-Plane, but take the highest level
		wxr_image[i*4*m_wxr_ncol+j*4+0] = 200;
		wxr_image[i*4*m_wxr_ncol+j*4+1] = 45;
		wxr_image[i*4*m_wxr_ncol+j*4+2] = 200;
		wxr_image[i*4*m_wxr_ncol+j*4+3] = 255; // Non-Transparent
	      }
	    }
	  }
	  wxr_newdata = 0;
	}

// LEFT
        glPushMatrix();

        glTranslatef(m_PhysicalSize.x*acf_x, m_PhysicalSize.y*acf_y, 0.0);
        //glRotatef((int) lroundf(heading_map), 0, 0, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Remove border line
	GLfloat lcolor[4]={0,0,0,1};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, lcolor);
	
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA,
		      m_wxr_ncol,  m_wxr_nlin, 0, GL_RGBA,
		      GL_UNSIGNED_BYTE, wxr_image);

	float lscx = 0.5 * ((float) m_wxr_ncol) * mpplon / mapRange * map_size * cos(M_PI / 180.0 * aircraftLat);
	float lscy = 0.5 * ((float) m_wxr_nlin) * mpplat / mapRange * map_size;
	float ltx = (textureCenterLon - aircraftLon) * ((float) wxr_pixperlon) * mpplon / mapRange * map_size *
	  cos(M_PI / 180.0 * aircraftLat);
	float lty = (textureCenterLat - aircraftLat) * ((float) wxr_pixperlat) * mpplat / mapRange * map_size;
/*	
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
//
	// end of down-shifted and rotated coordinate system
	glPopMatrix();
*/

//glPushMatrix();

float originalColumn = calculateAircraftOriginalColumn(aircraftLon, textureCenterLon, wxr_pixperlon, m_wxr_ncol);
float originalLine = calculateAircraftOriginalLine(aircraftLat, textureCenterLat, wxr_pixperlat, m_wxr_nlin);

int angle = heading_map; // Example angle
unsigned char* line_at_angle = get_lines_from_angles(wxr_image, m_wxr_nlin, m_wxr_ncol, originalLine, originalColumn, angle);

int max_length = sqrt(m_wxr_nlin * m_wxr_nlin + m_wxr_ncol * m_wxr_ncol);
for (int i = 0; i < max_length; i++) {
    if (line_at_angle[i * 4] == 0 && line_at_angle[i * 4 + 1] == 0 && line_at_angle[i * 4 + 2] == 0 && line_at_angle[i * 4 + 3] == 0) {
       break; // Assuming a series of 0s indicate the end or unused part of the array
    }
    printf("Pixel %d: R=%d, G=%d, B=%d, A=%d\n", i, line_at_angle[i * 4], line_at_angle[i * 4 + 1], line_at_angle[i * 4 + 2], line_at_angle[i * 4 + 3]);
}

// OpenGL setup code remains the same
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_wxr_ncol, 3, 0, GL_RGBA, GL_UNSIGNED_BYTE, line_at_angle);
glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

float vertex_x = 0;
float vertex_y = 0; // Starting at the bottom of the screen

float end_vertex_x = vertex_x; // No change in x, line goes straight up
float end_vertex_y = m_PhysicalSize.y * 0.9;

glBegin(GL_TRIANGLES);
glTexCoord2f(0.0f, 0.0f); glVertex2f(vertex_x - 1, vertex_y);
glTexCoord2f(0.0f, 1.0f); glVertex2f(vertex_x - 1,  end_vertex_y);
glTexCoord2f(1.0f, 1.0f); glVertex2f(vertex_x +  1, end_vertex_y);

glTexCoord2f(0.0f, 0.0f); glVertex2f(vertex_x - 1, vertex_y);
glTexCoord2f(1.0f, 0.0f); glVertex2f(vertex_x + 1, vertex_y);
glTexCoord2f(1.0f, 1.0f); glVertex2f(vertex_x + 1, end_vertex_y);
glEnd();

glDisable (GL_TEXTURE_2D);
glFlush();



/*
// Begin rendering the line
glBegin(GL_LINES);

// Calculate the starting vertex for the line based on the aircraft's position on the screen
float vertex_x = 0;
float vertex_y = 0; // Starting at the bottom of the screen

// Calculate the ending vertex for the line, pointing straight north
// Assuming `max_length` scales directly with your map scaling factors
float end_vertex_x = vertex_x; // No change in x, line goes straight up
//float end_vertex_y = max_length * lscy; // Use the scaling factor to determine the length
float end_vertex_y = m_PhysicalSize.y * 0.9;

// Set vertices for the line
glVertex2f(vertex_x, vertex_y);
glVertex2f(end_vertex_x, end_vertex_y);

// End the line rendering
glEnd();

// Clean up
glDisable(GL_TEXTURE_2D);
glFlush();
*/

glPopMatrix();
	
	
	//
/*
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
	
*/
      } // valid acf coordinates
    } // known heading and WXR data allocated
  }
} // end namespace ns
