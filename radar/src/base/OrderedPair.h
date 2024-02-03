/**
 * Defines an ordered pair structure templated over data type (int, double, etc.)
 * and associated mathematical operations. All members are public to allow easy
 * access
 */

#ifndef OrderedPair_h
#define OrderedPair_h

#include "OrderedPair.cpp"

namespace OpenGC
{

template<typename TDataType>
class OrderedPair
{

public:

  OrderedPair() {x=0; y=0;}
  ~OrderedPair() {};

  /** X coordinate */
  TDataType x;

  /** Y coordinate */
  TDataType y;
};

} // end namespace OpenGC

#endif
