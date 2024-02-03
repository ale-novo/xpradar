/**
 * Render windows are the high-level window management object.
 * Although this class does not contain any virtual functions, it assumes
 * that a proper rendering context exists PRIOR to any of the GL code
 * executing. Creating a device context is left as an excercise for the
 * reader.
 *
 * NOTE that bool m_IsOKToRender is set to false in the constructor of
 * this class and essentially disables rendering until enabled in a
 * derived class (to help prevent accidental OpenGL shenanigans).
 */

#ifndef RenderWindow_h
#define RenderWindow_h

#include <list>
#include "OrderedPair.h"
#include "Gauge.h"

namespace xpradar
{

class RenderWindow
{
public:

  /** Iterator type for traversing the list of gauges */
  typedef std::list<Gauge*>::iterator GaugeIteratorType;
  
  /** Constructor is responsible for initing a few variables */
  RenderWindow();

  ~RenderWindow();
  
  /** Call to reset projection on resize. */
  void Resize(int x, int y);
  
  /** Initialize GL parameters. */
  void SetupDisplay();

  /** Render the entire window. */
  void Render();

  /** Add a gauge to the window */
  void AddGauge(Gauge* pGauge);

  /** Iterates through the gauges and invokes the render method */
  void RenderGauges();

  /** Check to see if an extension exists */
  //  GLboolean RenderWindow::CheckExtension( char *extName );
  GLboolean CheckExtension( char *extName );

  /** ASCII character callback */
  void CallBackKeyboardFunc(unsigned char key, int x, int y);

  /** Special (non-ASCII) keystroke callback */
  void CallBackSpecialFunc(int key, int x, int y);

  /**  Mouse callback */
  void CallBackMouseFunc(int button, int state, int x, int y);

  /** Returns the mm/pixel for this render window */
  double GetUnitsPerPixel() { return m_UnitsPerPixel; }

  /** Specify whether or not to smooth lines */
  void SetSmoothLines(bool flag);

  /** Send a message to member gauges, possibly take some local action */
  void OnMessage(Message message, void* data);
  
protected:

  /** Shows the frame rate */
  void ShowFrameRate();

  /** Window height and width in pixels */
  OrderedPair<unsigned int> m_WindowSize;

  /**
   * Physical size of a pixel (assumed to be square) in mm
   * for the purpose of drawing gauges in approximately
   * the correct physical size on the monitor. It's also correct to
   * refer to this as the dot pitch of the monitor in mm.
   */
  double m_UnitsPerPixel;

  /** Parameters for the orthographic projection */
  float m_OrthoParams[6];
  
  /** All of the gauges */
  std::list<Gauge*> m_GaugeList;

  /** The number of gauges used in the display */
  int m_NumGauges;

  /** Whether or not to anti-alias lines */
  bool m_SmoothLines;
  
  /** True if the render window has been initialized */
  bool m_DisplayIsInitialized;

  /** True if it's safe to render things */
  bool m_IsOKToRender;

  /** The font handle provided to us by the font manager */
  int m_Font;
};

} // end namespace xpradar

#endif

