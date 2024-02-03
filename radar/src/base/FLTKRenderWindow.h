/**
 * FLTK implementation of the OpenGC render window
 */

#ifndef FLTKRenderWindow_h
#define FLTKRenderWindow_h

#include <FL/Fl_Gl_Window.H>
#include "RenderWindow.h"

namespace OpenGC
{

class FLTKRenderWindow : public Fl_Gl_Window, public RenderWindow
{
public:
  
  /** Constructor follows FLTK syntax */
  FLTKRenderWindow(int X, int Y, int W, int H, const char *L = 0);

  /** Yawn, a destructor */
  ~FLTKRenderWindow();
  
  /** FL rendering callback */
  void draw();
  
  /**  FL event handling callback */
  int handle(int);

  /**  Render the entire window */
  void Render();
};

} // end namespace OpenGC

#endif
