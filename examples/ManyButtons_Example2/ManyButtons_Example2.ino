// Required Library: https://github.com/microentropie/ManyButtons

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
EXAMPLE 2:
Let's imagne we have a volume control.
With the action of 2 buttons we will set the volume.
Code implements three different behaviours:
* when the relevant button is pressed volume is incremented / decremented
* holding the button down will keep on incrementing / decrementing
* holding the button down for a long time (1.6") will speed-up volume increment / decrement
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

// volume can go from 0 to 100, default 50:
int volume = 50;

unsigned long loopTimer;

void setup()
{
  Serial.begin(74880);
  Serial.println();
  Serial.println();
  Serial.println("ManyButtons_Example2.ino");
  Serial.println("https://github.com/microentropie/ManyButtons");

  // (2) subscribe to the mbEVENT_RELEASE and mbEVENT_LONGPRESS events:
  ManyButtons::Init(mbEVENT_RELEASE | mbEVENT_LONGPRESS);

  loopTimer = millis();
}

bool longPress = false;

void loop()
{
  // (3) let the class debounce and detect events 
  ManyButtons::Check();

  if (millis() - loopTimer < 600)
    return; // wait until the proper auto-repeat time comes
  loopTimer += 600; // prepare for auto-repeat step

  // (4) check if any button has been pressed
  //     (or better released as we subscribed
  //      to the mbEVENT_RELEASE and mbEVENT_LONGPRESS events)
  buttonStatus b = ManyButtons::getButtonEvent();
  if (b.event != mbEVENT_NO_EVENT)
  {
    // a button has been pressed, the call reports it
    // event can be mbEVENT_PRESS or mbEVENT_LONGPRESS
    Serial.print("[");
    Serial.print(millis());
    Serial.print(" ms] pin: ");
    Serial.print(b.pin);
    Serial.print(" event: ");
    Serial.print(b.event);
    Serial.println();

    longPress = (b.event == mbEVENT_LONGPRESS);
  }
  else
  {
    // no button events
    // (5) check if the button is being held pressed
    b.pin = ManyButtons::getPressedButton();
  }
  if (b.pin == BUTTON_RIGHT_PIN)
  {
    volume += longPress ? 10 : 1; // + pressed, increment volume (quicker if longPress)
    if (volume > 100) volume = 100; // max volume 100
  }
  else if (b.pin == BUTTON_LEFT_PIN)
  {
    volume -= longPress ? 10 : 1; // - pressed, decrement volume (quicker if longPress)
    if (volume < 0) volume = 0; // min volume 0
  }
  
  if (b.pin > 0)
  {
    // report new volume value
    Serial.print("volume=");
    Serial.print(volume);
    Serial.println();
  }
}

/*
ManyButtons_Example2.ino
https://github.com/microentropie/ManyButtons
[4801 ms] pin: 12 event: 2
volume=51
[7801 ms] pin: 12 event: 2
volume=52
[9601 ms] pin: 12 event: 2
volume=53
[10801 ms] pin: 12 event: 2
volume=54
volume=55
volume=56
volume=57
[14401 ms] pin: 12 event: 4
volume=67
volume=77
volume=87
*/
