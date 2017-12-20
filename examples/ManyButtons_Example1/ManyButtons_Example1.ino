// Required Library: https://github.com/microentropie/MultipleButtonsClass

/*
ManyButtons:
Can handle many buttons with one simple class.
You need to define the Arduino pins involved in DigitalPin[] array (see below in the example).
Each pin must be reserved for a single button.
You just need to connect each button to the relevant pin and ground.
Pull-up resistor is provided by the micro.
Class will handle debounce and detect when each button is pressed, released or long-pressed.
No interrupts are being used, you need to call the Check() method in the Arduino loop().
*/

/*
EXAMPLE 1:
Report (to Serial) when a button is pressed.
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

unsigned long loopTimer;

void setup()
{
  Serial.begin(74880);
  Serial.println();
  Serial.println();
  Serial.println("ManyButtons_Example1.ino");
  Serial.println("https://github.com/microentropie/ManyButtons");

  // (2) subscribe to the mbEVENT_RELEASE event:
  ManyButtons::Init(); // this is the default,
  // but you can even explicit the parameters;
  // ManyButtons::Init() is equivalent to:
  //ManyButtons::Init(mbEVENT_RELEASE, /*debounce time=*/120);

  loopTimer = millis();
}

void loop()
{
  // (3) let the class debounce and detect events 
  ManyButtons::Check();

  if (millis() - loopTimer < 600)
    return; // wait until the proper time comes
  loopTimer += 600; // prepare for next step

  // (4) check if any button has been pressed
  //     (or better released as we subscribed to the mbEVENT_RELEASE event)
  buttonStatus b = ManyButtons::getButtonEvent();
  if (b.event != mbEVENT_NO_EVENT)
  {
    // a button event has occurred, report it
    Serial.print("[");
    Serial.print(millis());
    Serial.print(" ms] pin: ");
    Serial.print(b.pin);
    Serial.print(" event: ");
    Serial.print(b.event);
    Serial.println();
  }
}

/*
*** Output example ***
 
ManyButtons_Example1.ino
https://github.com/microentropie/ManyButtons
[9001 ms] pin: 6 event: 2
[10801 ms] pin: 12 event: 2
[13201 ms] pin: 8 event: 2
[14401 ms] pin: 7 event: 2
[16201 ms] pin: 7 event: 2
 
*/
