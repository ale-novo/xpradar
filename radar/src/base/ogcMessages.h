/**
 * Defines messages which can be triggered by mouse clicks, key presses, etc.
 */

#ifndef ogcMessages_h
#define ogcMessages_h

namespace OpenGC
{

enum Message
{
  /* Messages used by the virtual keypad in the test gauge*/
  MSG_VIRTUAL_KEYPAD_0,
  MSG_VIRTUAL_KEYPAD_1,
  MSG_VIRTUAL_KEYPAD_2,
  MSG_VIRTUAL_KEYPAD_3,
  MSG_VIRTUAL_KEYPAD_4,
  MSG_VIRTUAL_KEYPAD_5,
  MSG_VIRTUAL_KEYPAD_6,
  MSG_VIRTUAL_KEYPAD_7,
  MSG_VIRTUAL_KEYPAD_8,
  MSG_VIRTUAL_KEYPAD_9,

  /* Altimeter pressure increment/decrement by 1/100 inch */
  MSG_ALTIMETER_PRESSURE_INCREMENT,
  MSG_ALTIMETER_PRESSURE_DECREMENT,

  /* Nav zoom in and out */
  MSG_NAV_ZOOM_DECREASE,
  MSG_NAV_ZOOM_INCREASE
};

} // end namespace OpenGC

#endif
