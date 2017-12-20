#ifndef _ManyButtons__h_
#define _ManyButtons__h_

/*
// Must put the following code inside the sketch (example with 4 buttons) - begin
#include <ManyButtons.h>
#define BUTTON_LEFT_PIN   6  // PD6
#define BUTTON_RIGHT_PIN 12  // PB4
#define BUTTON_ESC_PIN    8  // PB0
#define BUTTON_ENTER_PIN  7  // PD7
ManyButtons_PinsAre(BUTTON_LEFT_PIN, BUTTON_RIGHT_PIN, BUTTON_ESC_PIN, BUTTON_ENTER_PIN);
// Must put the following code inside the sketch (example with 4 buttons) - end
//
// NOTE:
// This approach greatly reduces the amount of RAM involved.
// Pins must be constants (i.e. numbers) not variables.
// A great way to parametrize the pins is to use defines as in the above code.
// If you absolutely need to make use of variables for pin number definitions,
// cannot use this library !.
*/

#include <Arduino.h>

// Events to which user can subscribe, bit mapped
#define mbEVENT_NO_EVENT 0
#define mbEVENT_PRESS 1
#define mbEVENT_RELEASE 2
#define mbEVENT_LONGPRESS 4


// default timings:
#define mbDefaultDebounceTime 120
#define mbDefaultLongPressTime 1600

// if the following define is enabled
// button events report the timestamp at which the event occurred
//#define WANT_TIMESTAMP

struct buttonStatus
{
  signed char pin; // button digital pin
  byte event;
#ifdef WANT_TIMESTAMP
  unsigned long timestamp;
#endif //WANT_TIMESTAMP
};



class ManyButtons
{
public:
  static void Init(byte checkEvents = mbEVENT_RELEASE, int debounceTime = mbDefaultDebounceTime, int longPressTime = mbDefaultLongPressTime);
  static void Clear();
  static void Check();
  static buttonStatus getButtonEvent();
  static byte getPressedButton();
  static void(*ReportEventFn)(byte event, int pin, unsigned long timestamp);


private:
  ManyButtons() {};      // inhibit class instantiation
  static const byte numButtons;
  static const byte DigitalPin[];
  static byte LastStatus[];
  static byte checkEvents;
  static int debounceTime;
  static int longPressTime;

  static unsigned long debounceCounter;
  static signed char currentButtonIndex;
  static byte currentButtonEvent;
};

#define PP_NARG(...) (sizeof((int[]){__VA_ARGS__})/sizeof(int))

#define ManyButtons_PinsAre(...) \
const byte ManyButtons::numButtons = PP_NARG(__VA_ARGS__); \
const byte ManyButtons::DigitalPin[ManyButtons::numButtons] = { __VA_ARGS__ }; \
byte ManyButtons::LastStatus[ManyButtons::numButtons]


#endif //_ManyButtons__h_
