/* Please make sure your touch IC model. */
//#define TOUCH_MODULES_CST_MUTUAL
#define TOUCH_MODULES_CST_SELF

#include "Arduino.h"
#include "OneButton.h"
#include "TFT_eSPI.h" /* Please use the TFT library provided in the library. */
#include "TouchLib.h"
#include "Wire.h"
#include "pin_config.h"

#include "Latin_Hiragana_24.h"
#include "NotoSansBold15.h"
#include "NotoSansMonoSCB20.h"
#include "Final_Frontier_28.h"

#define latin Latin_Hiragana_24
#define small NotoSansBold15
#define sans20 NotoSansMonoSCB20
#define star Final_Frontier_28

/* The product now has two screens, and the initialization code needs a small change in the new version. The LCD_MODULE_CMD_1 is used to define the
 * switch macro. */
#define LCD_MODULE_CMD_1

#warning Please confirm that you have purchased a display screen with a touch chip, otherwise the touch routine cannot be implemented.
#if defined(TOUCH_MODULES_CST_MUTUAL)
TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS328_SLAVE_ADDRESS, PIN_TOUCH_RES);
#elif defined(TOUCH_MODULES_CST_SELF)
TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);
#endif

#define TOUCH_GET_FORM_INT 0

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
bool flags_sleep = false;
#if TOUCH_GET_FORM_INT
bool get_int = false;
#endif
OneButton button(PIN_BUTTON_1, true);
#if defined(LCD_MODULE_CMD_1)
typedef struct {
  uint8_t cmd;
  uint8_t data[14];
  uint8_t len;
} lcd_cmd_t;

lcd_cmd_t lcd_st7789v[] = {
    {0x11, {0}, 0 | 0x80},
    {0x3A, {0X05}, 1},
    {0xB2, {0X0B, 0X0B, 0X00, 0X33, 0X33}, 5},
    {0xB7, {0X75}, 1},
    {0xBB, {0X28}, 1},
    {0xC0, {0X2C}, 1},
    {0xC2, {0X01}, 1},
    {0xC3, {0X1F}, 1},
    {0xC6, {0X13}, 1},
    {0xD0, {0XA7}, 1},
    {0xD0, {0XA4, 0XA1}, 2},
    {0xD6, {0XA1}, 1},
    {0xE0, {0XF0, 0X05, 0X0A, 0X06, 0X06, 0X03, 0X2B, 0X32, 0X43, 0X36, 0X11, 0X10, 0X2B, 0X32}, 14},
    {0xE1, {0XF0, 0X08, 0X0C, 0X0B, 0X09, 0X24, 0X2B, 0X22, 0X43, 0X38, 0X15, 0X16, 0X2F, 0X37}, 14},
};
#endif





 uint16_t colors[5]={0x986D,0xDEFB,0xBDF6,0xFFFF,0xE73B};
 int press=0;
 int xpos[3]={5,60,115};
 int ypos[4]={5,60,115,170};
 char chars[4][3]={{'1','2','3'},{'4','5','6'},{'7','8','9'},{'c','0','.'}};
 int sx,sy=0;
 String num="";
 double res=0;
 double mul=7.53450;

void setup() {
  gpio_hold_dis((gpio_num_t)PIN_TOUCH_RES);

  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  pinMode(PIN_TOUCH_RES, OUTPUT);
  digitalWrite(PIN_TOUCH_RES, LOW);
  delay(500);
  digitalWrite(PIN_TOUCH_RES, HIGH);

 

  tft.begin();
  tft.setSwapBytes(true);
#if defined(LCD_MODULE_CMD_1)
  for (uint8_t i = 0; i < (sizeof(lcd_st7789v) / sizeof(lcd_cmd_t)); i++) {
    tft.writecommand(lcd_st7789v[i].cmd);
    for (int j = 0; j < lcd_st7789v[i].len & 0x7f; j++) {
      tft.writedata(lcd_st7789v[i].data[j]);
    }

    if (lcd_st7789v[i].len & 0x80) {
      delay(120);
    }
  }
#endif
 
  

  ledcSetup(0, 2000, 8);
  ledcAttachPin(PIN_LCD_BL, 0);
  ledcWrite(0, 120);
  sprite.createSprite(170,320);
  
  sprite.setTextColor(TFT_BLACK,colors[4]);
  sprite.setTextDatum(4);
  

  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
  if (!touch.init()) {
    Serial.println("Touch IC not found");
  }

  button.attachClick([] { flags_sleep = 1; });

#if TOUCH_GET_FORM_INT
  attachInterrupt(
      PIN_TOUCH_INT, [] { get_int = true; }, FALLING);
#endif
draw(-1,-1);
}

void draw(int x, int y)
  {

    sprite.loadFont(sans20);
    sprite.setTextDatum(4);
    press++;
    sx=10;
    sy=10;
    sprite.fillSprite(TFT_BLACK);
    sprite.setTextColor(TFT_BLACK,colors[4]);
    
    for(int i=0;i<4;i++){
      if(y>=ypos[i] && y<=ypos[i]+50)
      sy=i;
    for(int j=0;j<3;j++){
      if(x>=xpos[j] && x<=xpos[j]+50)
    sx=j;
    if(sx==j && sy==i)    
    sprite.fillSmoothRoundRect(xpos[j],ypos[i],50,50,5,TFT_BLUE,TFT_BLACK);
    else
    sprite.fillSmoothRoundRect(xpos[j],ypos[i],50,50,5,colors[0],TFT_BLACK);
    sprite.fillSmoothRoundRect(xpos[j]+3,ypos[i]+3,44,44,3,colors[1],colors[0]);
    

    sprite.fillSmoothRoundRect(xpos[j]+7,ypos[i]+7,36,36,3,colors[2],colors[1]);
    sprite.fillSmoothRoundRect(xpos[j]+11,ypos[i]+10,28,28,3,colors[3],colors[2]);
    sprite.fillSmoothRoundRect(xpos[j]+13,ypos[i]+11,24,24,3,colors[4],colors[3]);

    sprite.drawWedgeLine(xpos[j]+7,ypos[i]+42,xpos[j]+11,ypos[i]+38,2,1,colors[1]);
    sprite.drawWedgeLine(xpos[j]+40,ypos[i]+42,xpos[j]+36,ypos[i]+38,2,1,colors[1]);
     

    

    sprite.drawString(String(chars[i][j]),xpos[j]+19,ypos[i]+24);
    if(x>=xpos[j] && x<=xpos[j]+50)
    sx=j;
    }}
       
       if(x>=0)       
       if(chars[sy][sx]=='c')
         num="";
       else
         num=num+String(chars[sy][sx]);

    sprite.setTextColor(colors[4],TFT_BLACK);
    //sprite.drawString(String(x),20,240,2);
    //sprite.drawString(String(y),60,240,2);
    //sprite.drawString(String(press),80,240,2);
    sprite.unloadFont();
    sprite.setTextDatum(0);
    sprite.loadFont(star);
    sprite.drawString(num,10,230,4);
    res=num.toDouble()*mul;
    sprite.drawFloat(res,2,10,260);
    sprite.unloadFont();
    sprite.pushSprite(0,0);
  }
int deb=0;



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