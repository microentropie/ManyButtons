/*
Author:  Stefano Di Paolo
License: MIT, https://en.wikipedia.org/wiki/MIT_License
Date:  2017-12-18

Library: multiple-buttons handled by a Single class

Sources repository: https://github.com/microentropie/
*/

// inspired by:
// https://arduino.stackexchange.com/questions/9390/good-button-debouncing-statechange-library

#include <limits.h>

#include "ManyButtons.h"

// init default values of class static variables
byte ManyButtons::checkEvents = 0;
int ManyButtons::debounceTime = mbDefaultDebounceTime;
int ManyButtons::longPressTime = mbDefaultLongPressTime;
void(*ManyButtons::ReportEventFn)(byte event, int pin, unsigned long timestamp) = NULL;

signed char ManyButtons::currentButtonIndex = -1;
unsigned long ManyButtons::debounceCounter = 0UL;
byte ManyButtons::currentButtonEvent = mbEVENT_NO_EVENT;


// status definitions, for internal use only
#define BUTTON_IS_PRESSED 1
#define BUTTON_IS_NOT_PRESSED 2
#define BUTTON_IS_LONGPRESSED 4


// define basic parameters: events to which subscribe, debounce time, long-press time
void ManyButtons::Init(byte _checkEvents, int _debounceTime, int _longPressTime)
{
  ReportEventFn = NULL;

  checkEvents = _checkEvents;
  debounceTime = _debounceTime;
  longPressTime = _longPressTime;

  Clear();
}



// reset all events
void ManyButtons::Clear()
{
  for (int idx = 0; idx < numButtons; ++idx)
  {
    pinMode(DigitalPin[idx], INPUT_PULLUP);
    LastStatus[idx] = digitalRead(DigitalPin[idx]) ? BUTTON_IS_NOT_PRESSED : BUTTON_IS_PRESSED;
  }
  debounceCounter = millis();
  currentButtonIndex = -1;
  currentButtonEvent = mbEVENT_NO_EVENT;
}



#define ButtonPressMask (BUTTON_IS_NOT_PRESSED | BUTTON_IS_PRESSED)

// need to call this method as frequently as possible.
// this is required to solve all the debounce and event decoding of buttons
void ManyButtons::Check()
{
  unsigned long currMillis = millis();
  for (int idx = numButtons - 1; idx >= 0; --idx)
  {
    byte buttonStatus = digitalRead(DigitalPin[idx]) ? BUTTON_IS_NOT_PRESSED : BUTTON_IS_PRESSED;

    if (buttonStatus != (LastStatus[idx] & ButtonPressMask) && (currMillis - debounceCounter) >= debounceTime)
    {
      if (buttonStatus & BUTTON_IS_PRESSED)
      {
        // button pressed
        if (checkEvents & mbEVENT_PRESS)
        {
          // user asked to get event
          // report it to user
          currentButtonEvent = mbEVENT_PRESS;
          currentButtonIndex = idx;
          if (ReportEventFn) ReportEventFn(mbEVENT_PRESS, DigitalPin[idx], currMillis);
        }
      }
      if (buttonStatus == BUTTON_IS_NOT_PRESSED)
      {
        // button released
        if (checkEvents & mbEVENT_RELEASE)
        {
          // user asked to get event
          if (LastStatus[idx] & BUTTON_IS_LONGPRESSED)
          {
            // long press has been recorded on this button
            // must hide this release event to user
            // as LONGPRESS has already been reported
          }
          else
          {
            // report user the button has been released
            currentButtonEvent = mbEVENT_RELEASE;
            currentButtonIndex = idx;
            if (ReportEventFn) ReportEventFn(mbEVENT_RELEASE, DigitalPin[idx], currMillis);
          }
        }
      }
      debounceCounter = currMillis;
      LastStatus[idx] = buttonStatus;
    }
    else if ((checkEvents & mbEVENT_LONGPRESS) && buttonStatus == BUTTON_IS_PRESSED && buttonStatus == LastStatus[idx] && (currMillis - debounceCounter) >= longPressTime)
    {
      // long-press detected
      // NOTE: only enter once here because
      // buttonStatus is now BUTTON_IS_PRESSED
      // but LastStatus[idx] is going to be ORed with BUTTON_IS_LONGPRESSED
      currentButtonEvent = mbEVENT_LONGPRESS;
      currentButtonIndex = idx;
      LastStatus[idx] |= BUTTON_IS_LONGPRESSED;
      if (ReportEventFn) ReportEventFn(mbEVENT_LONGPRESS, DigitalPin[idx], currMillis);
    }
  }
}
#undef ButtonPressMask



// returns pin number and event of the pressed button.
// if no event has occurred
//   returns event = mbEVENT_NO_EVENT
//   in this case pin number is filled with the latest pressed pin
// if no button has ever been pressed returns pin=0
// NOTE: the button status can only be read once;
//       once read it is cleared.
//       If more buttons or more events on the same button occurs
//       between 2 calls of this method, they are lost.
//       Use ReportEventFn() if you want to catch them all.
buttonStatus ManyButtons::getButtonEvent()
{
  buttonStatus b;

  b.pin = (currentButtonIndex >= 0)? DigitalPin[currentButtonIndex] : 0;
  b.event = currentButtonEvent;
#ifdef WANT_TIMESTAMP
  b.timestamp = debounceCounter;
#endif //WANT_TIMESTAMP
  currentButtonEvent = mbEVENT_NO_EVENT;
  //currentButtonIndex = -1;
  return b;
}



// returns 0 if no button is being held pressed now.
// returns the pin number if a button is currently being pressed.
// NOTE: useful for auto increase/decrease of variables (see example 3).
// This method return if a button is pressed, regardless of the subscribed events.
byte ManyButtons::getPressedButton()
{
  for (int idx = 0; idx < numButtons; ++idx)
    if (LastStatus[idx] & BUTTON_IS_PRESSED)
      return DigitalPin[idx];

  return 0; // no button is pressed
}
