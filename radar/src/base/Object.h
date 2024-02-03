/**
 * The base class for any object in the world that can send
 * and receive messages
 */

#ifndef Object_h
#define Object_h

#include <list>
#include "Messages.h"

namespace ns
{

class Object
{
public:
 
  Object();
  virtual ~Object();

  /** Called when a message is triggered somewhere */
  void OnRadarMessage(Message message, void *data);

  /** Send a message to all objects */
  void DispatchRadarMessage(Message message, void *data);

private:
  
  /** List of all objects in the world, used for global message passing */
  static std::list<Object*>* m_StaticObjectList;
  
  /** Iterator type for accessing the list of objects */
  typedef std::list<Object*>::iterator ObjectIteratorType;
};

} // end namespace ns

#endif
