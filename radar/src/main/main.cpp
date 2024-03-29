/**
 * Responsible for creating *one* global AppObject, invoking
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
#include "GLHeaders.h"
#include "Object.h"
#include "AppObject.h"
#include "DataSource.h"
#include "FontManager.h"

// The level of debug information
int verbosity; // 0: no debug, 1: some debug, 2: lots of debug

//-------Construct the one and only App Object-----
ns::AppObject theApp;

// The update rate for the app's idle function
float appUpdateRate;

//------------Static RenderObject members...-----------
ns::DataSource* ns::RenderObject::m_pDataSource = 0;
ns::FontManager* ns::RenderObject::m_pFontManager = 0;
std::list<ns::Object*>* ns::Object::m_StaticObjectList = 0;

// Global idle function to handle app updates
void GlobalIdle(void *)
{
  theApp.IdleFunction();
  Fl::repeat_timeout(appUpdateRate, GlobalIdle);
}

/* this routine prints the GNU license information */
void print_license(void)
{
}

void signal_handler(int sigraised)
{
  printf("Interrupted ... exiting \n");
  exit(0);
}

/*
void GLAPIENTRY MessageCallback( GLenum source,
				 GLenum type,
				 GLuint id,
				 GLenum severity,
				 GLsizei length,
				 const GLchar* message,
				 const void* userParam )
{
   fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
	    }*/

int main(int argc, char* argv[])
{
  /* print License terms */
  print_license();

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

  // Enable OpenGL Debugging (or not)
  //glEnable              ( GL_DEBUG_OUTPUT );
  //glDebugMessageCallback( MessageCallback, 0 );
  
  printf ("=========== Starting up ==========\n");
    
  // Set the update rate in nominal seconds per frame
  if (argc == 1) {
    appUpdateRate = 1.0 / 50.0;
  } else {
    appUpdateRate = 1.0 / (float) atoi(argv[2]);
    printf("Started with an update rate of %f [1/s]\n",(float) atoi(argv[2]));
  }

  // Register the idle function (which is a timeout to
  // avoid saturating the processor)
  Fl::add_timeout(appUpdateRate, GlobalIdle);

  // Start up the application
  theApp.Go(argv[0],argv[1]);

  printf ("=========== Finished  ==========\n");
  return 0;
}

