/**
 * Responsible for creating *one* global ogcAppObject, invoking
 * the app's Go() method, and cleaning up memory allocation
 * when FLTK kicks back by calling the CleanupApp() global
 * function. CleanupApp() is called either by the ANSI C
 * atexit callback mechanism or by a Win32 hook that intercepts
 * the WM_QUIT message prior to posting it to the FLTK window.
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <FL/Fl.H>
#include "ogcObject.h"
#include "ogcAppObject.h"
#include "ogcDataSource.h"
#include "ogcFontManager.h"

// The level of debug information
int verbosity; // 0: no debug, 1: some debug, 2: lots of debug

//-------Construct the one and only App Object-----
OpenGC::AppObject theApp;

// The update rate for the app's idle function
float appUpdateRate;

//------------Static RenderObject members...-----------
OpenGC::DataSource* OpenGC::RenderObject::m_pDataSource = 0;
OpenGC::FontManager* OpenGC::RenderObject::m_pFontManager = 0;
std::list<OpenGC::Object*>* OpenGC::Object::m_StaticObjectList = 0;

// Global idle function to handle app updates
void GlobalIdle(void *)
{
  theApp.IdleFunction();
  Fl::repeat_timeout(appUpdateRate, GlobalIdle);
}

void signal_handler(int sigraised)
{
  printf("Interrupted ... exiting \n");
  exit(0);
}

int main(int argc, char* argv[])
{

  /* evaluate command line arguments */
  argc--;
  if (argc < 1) {
    printf("Invalid number of arguments. Please only specify the initialization name. This is the prefix of one of the initialization file names \"*.ini\" found in the source subdirectory inidata/ or in the installation subdirectory share/. You can optionally give a second argument with the frame rate (per second).\n");
    exit (-1);
  }

  // Set up a signal handler so we can clean up when we're interrupted from the command line
  if (signal(SIGINT, signal_handler) == SIG_ERR) {
    printf("Could not establish new Interrupt signal handler.\n");
  }
  if (signal(SIGTERM, signal_handler) == SIG_ERR) {
    printf("Could not establish new Termination signal handler.\n");
  }

  printf ("=========== OpenGC - Starting up ==========\n");
    
  // Set the update rate in nominal seconds per frame
  if (argc == 1) {
    appUpdateRate = 1.0 / 50.0;
  } else {
    appUpdateRate = 1.0 / (float) atoi(argv[2]);
    printf("Started OpenGC with an update rate of %f [1/s]\n",(float) atoi(argv[2]));
  }

  // Register the idle function (which is a timeout to
  // avoid saturating the processor)
  Fl::add_timeout(appUpdateRate, GlobalIdle);

  // Start up the OGC application
  theApp.Go(argv[0],argv[1]);

  printf ("=========== OpenGC  -  Finished  ==========\n");
  return 0;
}

