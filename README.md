ManyButtons
===========

* Author: Stefano Di Paolo (<http://github.com/microentropie>)
* Copyright (C) 2017 Stefano Di Paolo.
* Released under the MIT license.

A small class that can handle multiple buttons: just one single class and one single instance.
The result is a low memory usage.
Handles debouncing and the following events: button press, button release, button long-press.
Each pin must be reserved for a single button.
You just need to connect each button to the relevant pin and ground.
Pull-up resistor is provided by the micro.
No interrupts are being used, you need to call the Check() method in the Arduino loop().
Code is Arduino-portable so should work in any Arduino compatible platform.
Tested on Arduino Uno.

Requirements
------------
* An [Arduino](http://arduino.cc/)
* Some buttons

Installation
------------
Download the ZIP archive (<https://github.com/microentropie/ManyButtons> Press "Clone or download", then "Download ZIP"),
open the Arduino IDE and choose Sketch -> Include Library -> Add .ZIP Library... and select the downloaded file.

Code Examples
-------------
Library contains 3 examples:
* [EXAMPLE 1](./examples/ManyButtons_Example1/ManyButtons_Example1.ino):
  Report (to Serial) when a button is pressed.
* [EXAMPLE 1n](./examples/ManyButtons_Example1n/ManyButtons_Example1n.ino):
  Report (to Serial) when a button is pressed.
  A notification function is used to get button events. 
* [EXAMPLE 2](./examples/ManyButtons_Example2/ManyButtons_Example2.ino):
  Let's imagine we have a volume control.
  With the action on 2 buttons we will set the volume.
  Code implements three different behaviours:
  * when the relevant button is pressed volume is incremented / decremented
  * holding the button down will keep on incrementing / decrementing
  * holding the button down for a long time (1.6") will speed-up volume increment / decrement

Documentation
-------------
**ManyButtons_PinsAre()**
defines the Arduino Pins where buttons are connected to.
Each button must be connected to the pin and ground, no resistor is required.
Arduino pin numbers must be specified; example:
`ManyButtons_PinsAre(11,3);`
Can use defines, cannot use variables.
NOTE: class is static, no need to instantiate it, but `ManyButtons_PinsAre()` with at least one pin defined is required.

**`void Init()`**
define basic parameters: events to which subscribe, debounce time, long-press time.

**`void Clear()`**
reset all events.

**`void Check()`**
need to call this method as frequently as possible.
this is required to solve all the debounce and event decoding of buttons

**`buttonStatus getButtonEvent()`**
returns pin number and event of the pressed button.
If no event has occurred
  returns event = mbEVENT_NO_EVENT
  in this case pin number is filled with the latest pressed pin
If no button has ever been pressed returns pin=0
NOTE: the button status can only be read once;
      once read it is cleared.
      If more buttons or more events on the same button occurs
      between 2 calls of this method, they are lost.
      Use ReportEventFn() if you want to catch them all.

**`byte getPressedButton()`**
returns 0 if no button is being held pressed now.
returns the pin number if a button is currently being pressed.
NOTE: useful for auto increase/decrease of variables (see example 3).
This method return if a button is pressed, regardless of the subscribed events.

**`ReportEventFn`**
Callback function that, if defined, will be called whenever any event occurs.
See 'ManyButtons_Example1n' for an example.


License
-------
License: [MIT](https://en.wikipedia.org/wiki/MIT_License)
