// Required Library: https://github.com/microentropie/ManyButtons

/*
Here an example for ManyButtons Arduino library.
See readme.md file in library for details.
*/

/*
EXAMPLE 1:
Report (to Serial) when a button is pressed.
A notification function is used to get button events.
*/

// (1) define the Arduino digital pins reserved for buttons.
//     Connect buttons between the defined pin and GND
// Buttons          Uno pin: AVR Port:
#define BUTTON_LEFT_PIN   6  // PD6
#define BUTTON_RIGHT_PIN 12  // PB4
#define BUTTON_ESC_PIN    8  // PB0
#define BUTTON_ENTER_PIN  7  // PD7
#include <ManyButtons.h>
ManyButtons_PinsAre(BUTTON_LEFT_PIN, BUTTON_RIGHT_PIN, BUTTON_ESC_PIN, BUTTON_ENTER_PIN); 


void setup()
{
  Serial.begin(74880);
  Serial.println();
  Serial.println();
  Serial.println("ManyButtons_Example1n.ino");
  Serial.println("https://github.com/microentropie/ManyButtons");

  // (2) subscribe to the mbEVENT_RELEASE event:
  ManyButtons::Init(); // this is the default,
  // but you can even explicit the parameters;
  // ManyButtons::Init() is equivalent to:
  //ManyButtons::Init(mbEVENT_RELEASE, /*debounce time=*/120);
  ManyButtons::ReportEventFn = &ButtonEvent;
}



void loop()
{
  // (3) let the class debounce and detect events 
  ManyButtons::Check();
}

void ButtonEvent(byte event, int pin, unsigned long timestamp)
{
  // (4) an event has occurred, report it
  Serial.print("[");
  Serial.print(timestamp);
  Serial.print(" ms] pin: ");
  Serial.print(pin);
  Serial.print(" event: ");
  Serial.print(event);
  Serial.println();
}

/*

ManyButtons_Example1n.ino
https://github.com/microentropie/ManyButtons
[3292 ms] pin: 12 event: 2
[10800 ms] pin: 6 event: 2
[15466 ms] pin: 6 event: 2
[17214 ms] pin: 7 event: 2
[18846 ms] pin: 8 event: 2

*/
