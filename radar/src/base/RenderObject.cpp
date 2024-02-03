#include <stdio.h>
#include <iostream>
#include "RenderObject.h"

extern int verbosity;

namespace xpradar
{

RenderObject
::RenderObject()
{
  if (verbosity > 0) printf("RenderObject - constructing\n");
  
  // Initialize with safe default parameters

  m_UnitsPerPixel = 1.0;

  m_PhysicalPosition.x = 0;
  m_PhysicalPosition.y = 0;

  m_PhysicalSize.x = 0;
  m_PhysicalSize.y = 0;

  m_Scale.x = 1.0;
  m_Scale.y = 1.0;

  if (verbosity > 1) printf("RenderObject - constructed\n");
}

RenderObject
::~RenderObject()
{

}

void
RenderObject
::SetScale(double xScale, double yScale)
{
  // Set gauge scaling factors
  // Must be a double greater than 0

  if(xScale > 0)
    m_Scale.x = xScale;

  if(yScale > 0)
    m_Scale.y = yScale;
}

void
RenderObject
::SetSize(double xSize, double ySize)
{
//  printf("RenderObject::SetSize called\n");
  m_PhysicalSize.x = xSize;
  m_PhysicalSize.y = ySize;
}

void
RenderObject
::SetPosition(double xPos, double yPos)
{
  if (verbosity > 2)
    printf("RenderObject::SetPosition called - xpos %f, ypos %f\n", xPos, yPos);
  m_PhysicalPosition.x = xPos;
  m_PhysicalPosition.y = yPos;
}

void
RenderObject
::SetUnitsPerPixel(double unitsPerPixel)
{
  m_UnitsPerPixel = unitsPerPixel;
}


void
RenderObject
::SetArg(int arg)
{
  m_Arg = arg;
}
  
// Called by framework when a mouse click occurs
void
RenderObject
::HandleMouseButton(int button, int state, int x, int y)
{
  if( this->ClickTest(button, state, x, y) )
  {
    // Convert the click to an x/y position in render object physical coordinates
    double physX, physY;
    physX = (((double)x - (double)m_PixelPosition.x)/(double)m_PixelSize.x)*m_PhysicalSize.x;
    physY = (((double)y - (double)m_PixelPosition.y)/(double)m_PixelSize.y)*m_PhysicalSize.y;
    
    if(state==0)
      this->OnMouseDown(button, physX, physY);
    else
      this->OnMouseUp(button, physX, physY);
  }
}

// Called when a mouse "down" event occurs
void
RenderObject
::OnMouseDown(int button, double physicalX, double physicalY)
{
  // The default version of this doesn't do anything
  // Overload to provide specific functionality
  // cout << "RenderObject mouse down called\n";
}

// Called when a mouse "up" event occurs
void
RenderObject
::OnMouseUp(int button, double physicalX, double physicalY)
{
  // The default version of this doesn't do anything
  // Overload to provide specific functionality
}

} // end namespace xpradar
