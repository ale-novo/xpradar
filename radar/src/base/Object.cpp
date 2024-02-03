#include <stdio.h>
#include "Object.h"

namespace ns
{

Object
::Object()
{
  if(m_StaticObjectList == 0)
    m_StaticObjectList = new std::list<Object*>;
    
  // Add this object to the static list of all objects
  m_StaticObjectList->push_back(this);
}

Object
::~Object()
{
  // Remove this object from the list of objects that exist in the world
  ObjectIteratorType it;
  
  for (it = m_StaticObjectList->begin(); it != m_StaticObjectList->end(); ++it)
  {
    if ( (*it) == this )
    {
      m_StaticObjectList->remove(*it);
      break;
    }
  }
}

void
Object
::DispatchRadarMessage(Message message, void *data)
{
  // Dispatch the message to all objects
  ObjectIteratorType it;
  
  for (it = m_StaticObjectList->begin(); it != m_StaticObjectList->end(); ++it)
  {
    (*it)->OnRadarMessage(message, data);
  }
}

// Called when a message is triggered somewhere
void
Object
::OnRadarMessage(Message message, void *data)
{
  // The default version of this doesn't do anything
  // Overload to provide specific functionality
}

} // end namespace ns
