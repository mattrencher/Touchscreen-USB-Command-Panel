#include "HID-Project.h"
#include <Nextion.h>          // Nextion Library
#include <SoftwareSerial.h>   // Library for Software Serial Port
//#include <Keyboard.h>         // Keyboard Emulation
#include <Bounce2.h>
#include <Rotary.h>

Rotary rotary1 = Rotary(0,1);
Rotary rotary2 = Rotary(2,3);

int btn1Pin = 7;  // Button 1;
int btn2Pin = 8;  // Button 2;

Bounce btn1 = Bounce(btn1Pin, 10);
Bounce btn2 = Bounce(btn2Pin, 10);

// Nextion pins to TX, RX
SoftwareSerial HMISerial(9, 10);

// Declare all objects from the Nextion Display
// (page, ID, Object name)
NexHotspot m0 = NexHotspot(0, 2, "m0"); 
NexHotspot m1 = NexHotspot(0, 3, "m1");  
NexHotspot m2 = NexHotspot(0, 4, "m2");  
NexHotspot m3 = NexHotspot(0, 5, "m3"); 
NexHotspot m4 = NexHotspot(0, 9, "m4"); 
NexHotspot m5 = NexHotspot(0, 8, "m5"); 
NexPicture m6 = NexPicture(0, 7, "m6");  
NexPicture m7 = NexPicture(0, 6, "m7");  

// All possible touch screen events get added to any array
NexTouch *nextion_touch_events[] = 
{
  &m0,  // Page 0 HotSpot
  &m1,  
  &m2,  
  &m3,  
  &m4, 
  &m5,
  &m6,
  &m7, 
  NULL  // End string
};

//  Callbacks for page 0 events
void m0PushCallback(void *ptr)
{
   Keyboard.press(HID_KEYBOARD_F13);
   delay(250);
   Keyboard.releaseAll();
   Serial.println("On/Off");
}

void m1PushCallback(void *ptr)
{
   Keyboard.press(HID_KEYBOARD_F14);
   delay(250);
   Keyboard.releaseAll();
   Serial.println("Clip");
}

void m2PushCallback(void *ptr)
{
   Keyboard.press(HID_KEYBOARD_F15);
   delay(250);
   Keyboard.releaseAll();
   Serial.println("Record");
}

void m3PushCallback(void *ptr)
{
   Keyboard.press(HID_KEYBOARD_F16);
   delay(250);
   Keyboard.releaseAll();
   Serial.println("Mute");
}

void m4PushCallback(void *ptr)
{
  Keyboard.press(HID_KEYBOARD_F17);
  delay(250);
  Keyboard.releaseAll();
  Serial.println("Stream");
}

void m5PushCallback(void *ptr)
{
  Keyboard.press(HID_KEYBOARD_F18);
  delay(250);
  Keyboard.releaseAll();
  Serial.println("Airhorn");
}

void m6PushCallback(void *ptr)
{
  Keyboard.press(HID_KEYBOARD_F19); 
  delay(250);
  Keyboard.releaseAll();
  Serial.println("Gunshots");
}

void m7PushCallback(void *ptr)
{
  Keyboard.press(HID_KEYBOARD_F20);
  delay(250);
  Keyboard.releaseAll();
  Serial.println("Scene");
}

void rot1pushBtn()
{
  Consumer.write(MEDIA_PLAY_PAUSE); 
  delay(30);
  Serial.println("Rotary 1 Button Pressed, Play/Pause");
}
void rot1CCW()
{
  Consumer.write(MEDIA_VOLUME_DOWN);
  delay(30);
  Serial.println("Rotary 1 CCW; Volume - ");
}
void rot1CW()
{
  Consumer.write(MEDIA_VOLUME_UP); 
  delay(30);
  Serial.println("Rotary 1 CW; Volume + ");
}
void rot2pushBtn()
{
  Keyboard.press(KEY_F20);
  delay(30);
  Serial.println("Rotary 2 Pressed");
}
void rot2CCW()
{
  Keyboard.press(KEY_F21);
  delay(30);
  Serial.println("Rotary 2 CCW");
}
void rot2CW()
{
  Keyboard.press(KEY_F22);
  delay(30);
  Serial.println("Rotary 2 CW");
}

void setup() {
  pinMode (btn1Pin, INPUT_PULLUP);
  pinMode (btn2Pin, INPUT_PULLUP);
  
  HMISerial.begin(9600);  // Start Software Serial at 9600bauds
  nexInit();  // Nextion Display initalize

// Link the touchscreen events to their relative functions in the code
// attachPush for press events or attachPop for release events  
  m0.attachPush(m0PushCallback);
  m1.attachPush(m1PushCallback); 
  m2.attachPush(m2PushCallback); 
  m3.attachPush(m3PushCallback);  
  m4.attachPush(m4PushCallback);  
  m5.attachPush(m5PushCallback); 
  m6.attachPush(m6PushCallback);  
  m7.attachPush(m7PushCallback);  

  Serial.begin(9600);
  //  Keyboard.begin();  // Start Keyboard emulation
  Consumer.begin();  // Consumer Keyboard from HID-Project
}

void loop() {
//  delay(10);
  // Check for touch screen events
  nexLoop(nextion_touch_events);
  
  // Encoders check for any rotations
  unsigned char result1 = rotary1.process();
  if (result1 == DIR_CW) {
    rot1CW();
  } else if (result1 == DIR_CCW) {
    rot1CCW();
  }
  unsigned char result2 = rotary2.process();
  if (result2 == DIR_CW) {
    rot2CW();
  } else if (result2 == DIR_CCW) {
    rot2CCW();
  }
  
  // Check the Encoder Buttons
  btn1.update();
  btn2.update();
  
  if (btn1.fallingEdge()) {
    rot1pushBtn();
  }
  if (btn2.fallingEdge()) {
    rot2pushBtn();
  }
}
