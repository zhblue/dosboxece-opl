/**
 * For DosBoxECE-OPL OPL2Audio serial pass-through
 *
 *REQUIRES: Arduino OPL2 Library - https://github.com/DhrBaksteen/ArduinoOPL2
 *
 * OPL2 board is connected as follows:
 * Pin  8 - Reset
 * Pin  9 - A0
 * Pin 10 - Latch
 * Pin #  - Data    -MOSI of board being used (orignal Arduino Pin-11, ProMini Pin-16 )
 * Pin #  - Shift   -SCK of board being used (original Arduino Pin-13, ProMini Pin-15 )
 *
 * Code by Jason Hill 2021-09-23 adapted from https://github.com/DhrBaksteen/ArduinoOPL2
 * Pins can be adapted as neccesary, but make sure to use hardware SPI pins for best results.
 * Refer to DhrBaksteen's github wiki for more help wiring it and using his library.
 */

#include <SPI.h>
#include <OPL2.h>

#define SerData Serial

OPL2 opl2;
byte state, reg, value, bank, x0, x1, x2;

void setup() {
  Serial.begin(115200);
  opl2.begin();
  state=0;
}

void loop() {
  if(SerData.available()) {
    switch(state){
      case 0:
        x0=SerData.read();
        if(x0 & 0x80){
          state=1;  
        }
        else {
          state=0;
        }
        break;
      case 1:
        x1= SerData.read();
        state=2;
        break;
      case 2:
        x2= SerData.read();
        //bank = x0>>2; //Not needed for OPL2, only OPL3Duo!
        reg = ((x0 & 0x03) << 6) | ((x1 & 0x7E) >> 1);
        value = ((x1 & 0x01) << 7) | (x2 & 0x7F);
        opl2.write(reg, value);
        state=0;
        break;
      default:
        state=0;
        break;
    }
  }  
}