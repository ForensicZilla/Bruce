#include "globals.h"
#include <USB.h>
#include "core/display.h"
#include "clicker.h"
#include <BleMouse.h>
#include "core/mykeyboard.h"

#ifdef USB_as_HID
#include <USBHIDMouse.h>
USBHIDMouse Mouse;
#endif
BleMouse bleMouse;

unsigned long prevMillisec = 0;
unsigned long currMillisec = 0;
int clickCount = 0,delayValueInt;

void clicker_setup(){
  String delayValue = keyboard("100",4,"Delay between click(ms)");
  delayValueInt = atoi(delayValue.c_str());

  options = {
    #ifdef USB_as_HID
    {"USB", [=]() { usbClickerSetup();}},
    #endif
    {"BLE", [=]() { bleClickerSetup();}},
  };
  delay(200);
  loopOptions(options, true);
  delay(200);
}

#ifdef USB_as_HID
void usbClickerSetup(){
  USB.begin();
  Mouse.begin();

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0,0);
  tft.print("CPS");

  for(;;){
    Mouse.click(MOUSE_LEFT);
    clickCount++;
    currMillisec = millis();
    if (currMillisec - prevMillisec >= 1000) {
      Serial.println(clickCount);
      displaySomething(String(clickCount));
      clickCount = 0;
      prevMillisec = currMillisec;
    }
    delay(delayValueInt);
  }
}
#endif

void bleClickerSetup(){
  bleMouse.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0,0);
  displaySomething("Waiting to pair");

  for(;;){
     if(bleMouse.isConnected()) {
       bleMouse.click(MOUSE_LEFT);
       clickCount++;
       currMillisec = millis();
       if (currMillisec - prevMillisec >= 1000) {
         Serial.println(clickCount);
         displaySomething(String(clickCount));
         clickCount = 0;
         prevMillisec = currMillisec;
       }
     }
     delay(delayValueInt);
  }
}