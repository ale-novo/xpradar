/**
 * The base class for any object in the OpenGC world that can send
 * and receive messages
 */

#ifndef Object_h
#define Object_h

#include <list>
#include "Messages.h"

namespace OpenGC
{

class Object
{
public:
 
  Object();
  virtual ~Object();

  /** Called when a message is triggered somewhere */
  void OnOpenGCMessage(Message message, void *data);

  /** Send a message to all OpenGC objects */
  void DispatchOpenGCMessage(Message message, void *data);

private:
  
  /** List of all objects in the OpenGC world, used for global message passing */
  static std::list<Object*>* m_StaticObjectList;
  
  /** Iterator type for accessing the list of OpenGC objects */
  typedef std::list<Object*>::iterator ObjectIteratorType;
};

} // end namespace OpenGC

#endif
