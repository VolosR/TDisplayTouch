#define TOUCH_MODULES_CST_SELF
#include "Arduino.h"
#include "TFT_eSPI.h" /* Please use the TFT library provided in the library. */
#include "TouchLib.h"
#include "Wire.h"
#include "pin_config.h"

TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

int deb=0;
bool switch1=true;
bool switch2=false;

void setup() {
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  pinMode(PIN_TOUCH_RES, OUTPUT);
  digitalWrite(PIN_TOUCH_RES, LOW);
  delay(500);
  digitalWrite(PIN_TOUCH_RES, HIGH);
  tft.begin();
  sprite.createSprite(170,320);
  sprite.setTextColor(TFT_WHITE,TFT_BLACK);
  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
  draw(-1,-1);
}


void draw(int x, int y)
{  
  sprite.fillSprite(TFT_BLACK);
 
      if(y<160)
      switch1=!switch1;
      else
      switch2=!switch2;

    sprite.drawString("SWITCH1= "+String(switch1),20,30,4);
    sprite.drawRect(18,58,134,54,TFT_GREEN);
    sprite.fillRect(20+(switch1*80),60,50,50,TFT_GREEN);
    
    sprite.drawString("SWITCH2= "+String(switch2),20,190,4);
    sprite.drawRect(18,218,134,54,TFT_YELLOW);
    sprite.fillRect(20+(switch2*80),220,50,50,TFT_YELLOW);

  sprite.pushSprite(0,0); 
}

void loop() {
  
  if (touch.read()) {
    if(deb==0){
    deb=1;
      TP_Point t = touch.getPoint(0);
      if(t.x<170)
      draw(t.x,t.y);
    } }else {
    deb=0;}
}