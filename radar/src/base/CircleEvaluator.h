/**
 * CircleEvaluator generates a series of points which lie on the
 * perimeter of the circle. The circle is evaluated as an arc,
 * given a start and end point in degrees. Degrees are defined as 0=360 being
 * up, 90 degrees right, etc.
 */

#ifndef CircleEvaluator_h
#define CircleEvaluator_h

namespace ns
{

class CircleEvaluator  
{

public:

  CircleEvaluator();
  virtual ~CircleEvaluator();
  
  /** Sets the radius of the circle in physical units */
  void SetRadius(double radius);
  
  /** Defines the "coarseness" of the circle (degree spacing of GL_POINTS) */
  void SetDegreesPerPoint(double degreesPerPoint);
  
  /** Defines the start and end of the arc in degrees */
  void SetArcStartEnd(double startArc, double endArc);

  /** Defines the center of the circle in physical units */
  void SetOrigin(double x, double y);

  /** Defines dashed circle */
  void SetDashed(int nper100, float ratio);

  /** Calls the GL code to evaluate the circle (generates points) */
  void Evaluate();

protected:

  /** The origin (center) of the circle */
  double m_XOrigin, m_YOrigin;

  /** The radius of the arc/circle */
  double m_Radius;

  /** The start and end of the arc in degrees */
  double m_StartArcDegrees, m_EndArcDegrees;

  /** How many degrees to move before generating a new point */
  double m_DegreesPerPoint;

  /** How many dashes per 100 pixels diameter (set to 0 for no dashes) **/
  int m_Nper100;

  /** Ratio of dashed versus total space **/
  float m_Ratio;

};

  /* Function to draw a dashed line since in OpenGL ES the gl_line_stipple function is not available */
  void drawDashedLine(float x0, float y0, float x1, float y1, float nper100, float ratio);

} // end namespace ns

#endif
