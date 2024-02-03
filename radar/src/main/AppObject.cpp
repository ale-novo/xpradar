#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

//--------GL Stuff---------
#include "GLHeaders.h"

//---------Base Stuff----------
#include <FL/Fl.H>

extern "C" {
#include "dictionary.h"
#include "iniparser.h"
#include "wxrdata.h"
}

#include "AppObject.h"
#include "RenderObject.h"

//--------Data Sources---------
#include "XPlaneDataSource.h"

//-----------Gauges------------
#include "WXR/WXR.h"
#include "Dummy/DummyGauge.h"

extern int verbosity;

extern "C" char clientname[100];

namespace ns
{

  AppObject::AppObject()
  {
    /*if (verbosity > 0)*/ printf("AppObject - constructing\n");
  
    // Make sure all the pointers are nulled out
    m_pFontManager = 0;
    m_pRenderWindow = 0;
    m_pDataSource = 0;
    verbosity = 0;

    m_InitState = 0;
    
    // Run a frame-rate test when loading
    m_FrameTest = false;

    /* reset frame rate */
    for (int i=0;i<NFPS;i++) {
      m_FPSArray[i] = FLT_MISS;
    }
    m_FPS = FLT_MISS;
    m_FPSIndex = 0;

    /*if (verbosity > 1)*/ printf("AppObject - constructed\n");
  }

  AppObject::~AppObject()
  {
    // The way is laid out, the destructor for the App isn't actually called,
    // but if it did, we'd want to do a little cleanup
    this->Cleanup();
  }
  
  int AppObject::Go(char* programPath, char* iniFileName)
  {
    char iniFile[255];
    char cwd[200];
    char *pch;
    
    /* check if we are in the source code directory or in the binary installation path */
    if (strncmp("/",programPath,1)==0) {
      /* we have full path starting code */
      printf("Starting with full program path \n");
      /* remove program name from path */
#ifdef WIN
      pch = strrchr(programPath,'\\');
#else
      pch = strrchr(programPath,'/');
#endif
      *pch = '\0';
      printf("%s\n",programPath);
#ifdef WIN
      pch = strstr (programPath,"\\bin");
#else
      pch = strstr (programPath,"/bin");
#endif
      if (pch == NULL) {
	snprintf (iniFile, sizeof(iniFile), "%s/../../inidata/%s.ini",programPath,iniFileName);
	snprintf (m_FontPath, sizeof(m_FontPath), "%s/../../fonts/",programPath);
      } else {
	snprintf (iniFile, sizeof(iniFile), "%s/../share/xpradar/%s.ini",programPath,iniFileName);
	snprintf (m_FontPath, sizeof(m_FontPath), "%s/../share/xpradar/",programPath);
      }
    } else {
      /* assume we start from source or bin dir */
      printf("Starting from bin/ or src/ directory \n");
      if (getcwd(cwd, sizeof(cwd)) == NULL) return -2;
#ifdef WIN
      pch = strstr (cwd,"\\bin");
#else
      pch = strstr (cwd,"/bin");
#endif
      if (pch == NULL) {
	snprintf (iniFile, sizeof(iniFile), "../../inidata/%s.ini", iniFileName);
	snprintf (m_FontPath, sizeof(m_FontPath), "../../fonts/");
      } else {
	snprintf (iniFile, sizeof(iniFile), "../share/xpradar/%s.ini", iniFileName);
	snprintf (m_FontPath, sizeof(m_FontPath), "../share/xpradar/");
      }
    }

    // Process the initialization file, if this fails, abort!
    if( !this->DoFileInitialization(iniFile) )
      {
	return 0;
      }

    printf ("AppObject - Initialization complete. Starting main loop.\n");
    if (verbosity > 0) printf ("====================\n");

    m_InitState = 1;

    // Now it's time to enter the event loop
    Fl::run();

    // This return value doesn't mean anything
    return 1;
  }

  void
  AppObject
  ::IdleFunction()
  {

    // Check the frame rate if the user wants to
    if(m_FrameTest) {
      this->CheckFrameRate();
      m_pRenderWindow->SetFPS(m_FPS);
    }
     
    // Every time we loop we grab some new data and re-render the window
    // Only render if things have changed from X-Plane or during init, so
    // all datarefs have been safely received
    m_pDataSource->OnIdle();
    //printf("%i \n",numreceived);
    if ((numreceived > 0) || (m_InitState <= 100) ||
	(wxr_newdata == 1) || (m_FrameTest)) {
  
       //printf("%i %i \n",numreceived,m_InitState);
      m_pRenderWindow->redraw();
    
      Fl::flush();
    }
    if (m_InitState <= 100) m_InitState++;
  }

  bool AppObject::IntermediateInitialization()
  {
    // Initialize the static members of RenderObject
    DummyGauge dummyRenderObject;
    dummyRenderObject.SetDataSource(m_pDataSource);
    dummyRenderObject.SetFontManager(m_pFontManager);
  
    if (verbosity > 1) printf("AppObject - Finished creating the dummy render object\n");

    return true;
  }

  void AppObject::Cleanup()
  {  
    if (verbosity > 0) printf("AppObject - Cleaning up\n");


    if(m_pRenderWindow != 0)
      {
	if (verbosity > 1) printf("AppObject - Deleting render window\n");
	delete m_pRenderWindow;
	m_pRenderWindow = 0;
      }
 
    if (m_pDataSource != 0)
      {
	if (verbosity > 1) printf("AppObject - Deleting data source\n");
	delete m_pDataSource;
	m_pDataSource = 0;
      }
  
    if(m_pFontManager != 0)
      {
	if (verbosity > 1) printf("AppObject - Deleting font manager\n");
	delete m_pFontManager;
	m_pFontManager = 0;
      }
   
    if (verbosity > 1) printf("AppObject - Finished memory cleanup\n");
  }

  bool AppObject::DoFileInitialization(char* iniFile)
  {
    char temp_string[50];
    dictionary *ini;
    int default_verbosity = 0;
    char default_server_ip[] = "";
    int default_server_port = 8091;
    char default_data_source[] = "X-Plane";
    char default_xplane_path[] = "";
    char default_client_name[] = "xpradar";
    int default_customdata = 0; // do not read from X-Plane's "Custom Data" directory by default
    int default_radardata = 0; // do not read from X-Plane's UDP radar data by default
    char default_dem_path[] = "";
    char default_gshhg_path[] = "";

    printf("AppObject - Starting initialization with %s\n", iniFile);

    // parse the *.ini file
    ini = iniparser_load(iniFile);
    if (ini == NULL) {
      printf("AppObject - Error opening initialization file %s\n", iniFile);
      return false;
    } else {
      // verbosity: 0: no debug, 1: some debug, 2: lots of debug
      verbosity = iniparser_getint(ini,"General:Verbosity", default_verbosity); 

      /* client name for the server */
      strncpy(clientname,iniparser_getstring(ini,"General:Name", default_client_name),sizeof(clientname));

      // path to X-Plane installation
      strcpy(m_XPlanePath,iniparser_getstring(ini,"General:XPlanePath",default_xplane_path));

      // whether to use X-Plane's Custom Data path
      m_customdata = iniparser_getint(ini,"General:CustomData", default_customdata);

      // whether to use X-Plane's UDP-based WXR radar data
      // 0: do not receive WXR data
      // 1: turn on control pad under network and put IP address of this computer here
      // 2: use regular UDP data stream of x-plane
      m_radardata = iniparser_getint(ini,"General:RadarData", default_radardata);

      // path to GLOBE DEM Files for Terrain Rendering in NAV Display. Will also be needed for VSI
      strcpy(m_DEMPath,iniparser_getstring(ini,"General:DEMPath",default_dem_path));

      // path to GSHHG Files for shoreline/lake Rendering in NAV Display.
      strcpy(m_GSHHGPath,iniparser_getstring(ini,"General:GSHHGPath",default_gshhg_path));

      // Set up font manager
      m_pFontManager = new FontManager();

      // Specify smoothed fonts
      m_pFontManager->SetSmooth(true);
      //m_pFontManager->SetSmooth(false);
    
      if (verbosity > 0) printf("AppObject - Font path %s\n", m_FontPath);
      m_pFontManager->SetFontPath(m_FontPath);
     
      // Set up the network data
      strcpy(m_ip_address,iniparser_getstring(ini,"network:ServerIP", default_server_ip));
      m_port = iniparser_getint(ini,"network:ServerPort", default_server_port);

      // Set up the data source
      if (verbosity > 2) printf("AppObject - Setting up the Data Source\n");
      strcpy(temp_string,iniparser_getstring(ini,"network:DataSource", default_data_source));
      // Currently only the X-Plane Data Source is supported
      if( strcmp(temp_string, "X-Plane")==0 )
	m_pDataSource = new XPlaneDataSource();

      // Now initialize the data source
      m_pDataSource->define_server(m_port,m_ip_address,m_radardata);

      // Setup the render window
      int initX = iniparser_getint(ini,"Window:xpos", 0);
      int initY= iniparser_getint(ini,"Window:ypos", 0);
      int width = iniparser_getint(ini,"Window:width", 1000);
      int height = iniparser_getint(ini,"Window:height", 750);
      int decoration = iniparser_getint(ini, "Window:undecorated", 0);
      int frameratetest = iniparser_getint(ini,"Window:frametest", 0);
      if (verbosity > 1) printf("AppObject - ready to create render window\n");

      // Create the new render window
      m_pRenderWindow = new FLTKRenderWindow(initX, initY, width, height, "RADAR");
      m_pRenderWindow->resizable(m_pRenderWindow);
      m_pRenderWindow->mode(FL_RGB | FL_DOUBLE | FL_MULTISAMPLE);
      //m_pRenderWindow->mode(FL_RGB | FL_MULTISAMPLE);
      //m_pRenderWindow->mode(FL_RGB | FL_DOUBLE);

      
      m_pRenderWindow->border(!decoration);
      //==================================================================================================//
      //  Temporary solution to produce a borderless (undecorated) window                               ==//
      //==================================================================================================//
      m_pRenderWindow->border(!decoration);         // <-- TODO: make this into an ini parameter!  ==//
      //==================================================================================================//
      
      // Are we doing a frame rate test?
      m_FrameTest = (bool)frameratetest;

      if (verbosity > 0) printf("AppObject - ready to show render window\n");

      // We need to go ahead and show the window so that an OpenGL device context exists
      // once we start loading fonts
      m_pRenderWindow->show();

      // Now we can do the intermediate init stuff
      this->IntermediateInitialization();

      // Force the render window to update to obtain a valid device context
      m_pRenderWindow->redraw();
      Fl::flush();

      // Ready with the window; now set up the gauge(s)
      int gaugeDefined = 0;
      int i = 1;
      char gaugeKey[50];
      char *gaugeName;
      float xpos, ypos, xscale, yscale;
      int gaugeArg;
    
      while (0==0) {
	snprintf (gaugeKey, sizeof(gaugeKey), "Gauge:%i:Name",i);
	gaugeName = iniparser_getstring (ini, gaugeKey, (char *) "");
	if (strcmp (gaugeName, "") == 0) break;
	if (verbosity > 0) printf ("Gauge %i: Name, value: %s\n", i, gaugeName);
	snprintf (gaugeKey, sizeof(gaugeKey), "Gauge:%i:xPos", i);
	xpos = iniparser_getdouble (ini, gaugeKey, 0);
	if (verbosity > 0) printf ("Gauge %i: xpos, value: %f\n", i, xpos);
	snprintf (gaugeKey, sizeof(gaugeKey), "Gauge:%i:yPos", i);
	ypos = iniparser_getdouble (ini, gaugeKey, 0);
	if (verbosity > 0) printf ("Gauge %i: ypos, value: %f\n", i, ypos);
	snprintf (gaugeKey, sizeof(gaugeKey), "Gauge:%i:xscale", i);
	xscale = iniparser_getdouble (ini, gaugeKey, 0);
	if (verbosity > 0) printf ("Gauge %i: xscale, value: %f\n", i, xscale);
	snprintf (gaugeKey, sizeof(gaugeKey), "Gauge:%i:yscale", i);
	yscale = iniparser_getdouble (ini, gaugeKey, 0);
	if (verbosity > 0) printf ("Gauge %i: yscale, value: %f\n", i, yscale);
	snprintf (gaugeKey, sizeof(gaugeKey), "Gauge:%i:arg", i);
	gaugeArg = iniparser_getint (ini, gaugeKey, 0);
	if (verbosity > 0) printf ("Gauge %i: arg, value: %i\n", i, gaugeArg);
	gaugeDefined = 1;
	i++;
	CreateGauge (gaugeName, xpos, ypos, xscale, yscale, gaugeArg);
      }
      if (! gaugeDefined) {
	printf ("No gauges defined!\n");
	iniparser_freedict(ini);
	return false;
      }
      if (verbosity > 0) printf ("%i gauges defined\n", i - 1);
      iniparser_freedict(ini);
      return true;
    }

    return true;
  }

  // Create a gauge
  void AppObject::CreateGauge(char* name, float xPos, float yPos, float xScale, float yScale, int arg)
  {
    // A pointer to reference the gauge we're creating
    Gauge* pGauge = 0;

    if (verbosity > 0) printf ("AppObject - Gauge %s, xp %f, yp %f, xs %f, ys %f, (arg %i)\n", name, xPos, yPos, xScale, yScale, arg);

    if (strcmp(name, "WXR")==0) pGauge = new WXR();
    else {
      printf("Gauge %s not defined in CreateGauge. Aborting\n",name);
      exit(-1);
    }
    pGauge->SetUnitsPerPixel(m_pRenderWindow->GetUnitsPerPixel());
    pGauge->SetPosition(xPos, yPos);
    pGauge->SetScale(xScale, yScale);
    pGauge->SetArg(arg);
    pGauge->SetFPS(FLT_MISS);
    m_pRenderWindow->AddGauge(pGauge);
  }

// calculate frame rate from timing and with running mean function
  void AppObject::CheckFrameRate()
  {

    float fps;

    fps = m_FPSArray[m_FPSIndex];

    /* evaluate timer */
    if (fps == FLT_MISS) {
      fps = 0.0; // temporary value
      gettimeofday(&m_start,NULL); // start count
    } else {
      gettimeofday(&m_end,NULL); // end count
      fps = 1./(m_end.tv_sec - m_start.tv_sec + (m_end.tv_usec - m_start.tv_usec)/1000000.0);
      gettimeofday(&m_start,NULL); // start count
      
    }

    m_FPSArray[m_FPSIndex] = fps;
    m_FPSIndex += 1;
    if (m_FPSIndex == NFPS) m_FPSIndex = 0;

    /* store running mean of fps */
    fps = 0.0;
    for (int i=0;i<NFPS;i++) {
      fps += m_FPSArray[i];
    }
    fps /= (float) NFPS;
    m_FPS = fps;
    //printf("fps: %i \n",(int) round(fps));
  }
  
} // end namespace ns
