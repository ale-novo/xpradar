#include <stdio.h>
#include <math.h>
#include "GLHeaders.h"
#include "CircleEvaluator.h"

namespace xpradar
{

CircleEvaluator
::CircleEvaluator()
{
  m_XOrigin = 0;
  m_YOrigin = 0;

  m_StartArcDegrees = 0;
  m_EndArcDegrees = 0;

  m_DegreesPerPoint = 10;

  m_Radius = 1;
}

CircleEvaluator
::~CircleEvaluator()
{

}

void
CircleEvaluator
::SetOrigin(double x, double y)
{
  m_XOrigin = x;
  m_YOrigin = y;
}

void
CircleEvaluator
::SetArcStartEnd(double startArc, double endArc)
{
  m_StartArcDegrees = startArc;
  m_EndArcDegrees = endArc;
}

void
CircleEvaluator
::SetDegreesPerPoint(double degreesPerPoint)
{
  m_DegreesPerPoint = degreesPerPoint;
}

void
CircleEvaluator
::Evaluate()
{
  double x;
  double y;
  double pi = 3.14159265;

  // We parametrically evaluate the circle
  // x = sin(t)
  // y = cos(t)
  // t goes from 0 to 2pi
  // 0 degrees = 0rad, 90 degrees = pi/2rad, etc.
  
  double startRad = m_StartArcDegrees / 180 * pi;
  double endRad = m_EndArcDegrees / 180 * pi;
  double radPerPoint = m_DegreesPerPoint / 180 * pi;
  if (startRad > endRad)
    endRad += 2*pi;
  double currentRad = startRad;

  do
  {
    x = m_Radius*sin(currentRad) + m_XOrigin;
    y = m_Radius*cos(currentRad) + m_YOrigin;
    glVertex2d(x,y);
    currentRad += radPerPoint;
  } while(currentRad < endRad);
  x = m_Radius*sin(endRad) + m_XOrigin;
  y = m_Radius*cos(endRad) + m_YOrigin;
  glVertex2d(x,y);
}

void
CircleEvaluator
::SetRadius(double radius)
{
  if (radius > 0)
    m_Radius = radius;
  else
    m_Radius = 1;
}

} // end namespace xpradar
