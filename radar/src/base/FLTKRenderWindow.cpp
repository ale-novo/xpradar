#include <stdio.h>
#include <math.h>

#include <FL/Fl.H>
#include "FLTKRenderWindow.h"

extern int verbosity;

namespace ns
{

FLTKRenderWindow
::FLTKRenderWindow(int X, int Y, int W, int H, const char *L) : Fl_Gl_Window(X, Y, W, H, L)
{
  if (verbosity > 0) printf("FLTKRenderWindow - constructing\n");
  //mode(FL_RGB | FL_DOUBLE | FL_DEPTH | FL_STENCIL);

  // Safe to enable rendering
  m_IsOKToRender = true;

  if (verbosity > 1) printf("FLTKRenderWindow - constructed\n");
}

FLTKRenderWindow
::~FLTKRenderWindow()
{
}

void
FLTKRenderWindow
::draw()
{
//  this->Render();
/*
  if (!valid()) {
    glViewport(0, 0, w(), h());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    valid(1);
  }

  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 1, 0xFF); // Pass stencil test always
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Replace stencil buffer with reference value where stencil test passes
  glStencilMask(0xFF); // Enable writing to the stencil buffer
*/
  this->Render();

//  glDisable(GL_STENCIL_TEST); 


}

int
FLTKRenderWindow
::handle(int event)
{
  switch(event) {
  case FL_PUSH:
    //... mouse down event ...
    this->CallBackMouseFunc(0, 0, Fl::event_x(), Fl::event_y() );
    return 1;
  case FL_DRAG:
    //... mouse moved while down event ...
    return 1;
  case FL_RELEASE:   
    //... mouse up event ...
    this->CallBackMouseFunc(0, 1, Fl::event_x(), Fl::event_y() );
    return 1;
  case FL_FOCUS :
  case FL_UNFOCUS :
    //... Return 1 if you want keyboard events, 0 otherwise
    return 1;
  case FL_KEYBOARD:
    //... keypress, key is in Fl::event_key(), ascii in Fl::event_text()
    //... Return 1 if you understand/use the keyboard event, 0 otherwise...
    return 1;
  case FL_SHORTCUT:
    //... shortcut, key is in Fl::event_key(), ascii in Fl::event_text()
    //... Return 1 if you understand/use the shortcut event, 0 otherwise...
    return 1;
  default:
    // pass other events to the base class...
    return Fl_Gl_Window::handle(event);
  }
}

void
FLTKRenderWindow
::Render()
{
  // Check to see if any FL window stuff has changed
  if( !valid() )
  {
    this->Resize( w(), h() );
  }
  
  // Call the base class render to do everything else
  RenderWindow::Render();
}

} // end namespace ns
