#define TOUCH_MODULES_CST_SELF
#include <TFT_eSPI.h> 
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

TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite img = TFT_eSprite(&tft);

#define LCD_MODULE_CMD_1

#warning Please confirm that you have purchased a display screen with a touch chip, otherwise the touch routine cannot be implemented.
#if defined(TOUCH_MODULES_CST_MUTUAL)
TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS328_SLAVE_ADDRESS, PIN_TOUCH_RES);
#elif defined(TOUCH_MODULES_CST_SELF)
TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);
#endif

#define TOUCH_GET_FORM_INT 0

bool flags_sleep = false;
#if TOUCH_GET_FORM_INT
bool get_int = false;
#endif

#define gray 0xB5B6
#define blue 0x0AAD
#define orange 0xC260
#define purple 0x604D
#define green 0x1AE9

int fromTop=124;
int fromLeft=6;
int boxW=36;
int boxH=42;
int space=5;

int n=4;
int m=4;

int posX[16];
int posY[16];

int cx,cy=0;
float n1=0;
float n2=0;
String num="";
int operation=0;
uint16_t colors[5]={0x02F1,0x4E78,0xF7FE,0xD9E7,0xFF2D};
char buttons[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'0','.','=','+'}};
uint16_t butColor[4][4]={{colors[1],colors[1],colors[1],colors[3]},{colors[1],colors[1],colors[1],colors[3]},{colors[1],colors[1],colors[1],colors[3]},{colors[1],colors[3],colors[3],colors[3]}};
uint16_t butTxtColor[4][4]={{colors[2],colors[2],colors[2],colors[0]},{colors[2],colors[2],colors[2],colors[0]},{colors[2],colors[2],colors[2],colors[0]},{colors[2],colors[0],colors[0],colors[0]}};
int brightness=150;

void setup() {

  pinMode(15,OUTPUT);
  digitalWrite(15,1);

  pinMode(0,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);

  
    
    tft.init();
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);
    img.createSprite(170, 320);
    img.setTextDatum(4);
    img.setTextColor(TFT_WHITE,TFT_BLACK);

    ledcSetup(0, 2000, 8);
    ledcAttachPin(PIN_LCD_BL, 0);
    ledcWrite(0, 200);

      Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
   draw(-1,-1);

}



#define color1 0x33AE  //body
#define color2 0x22CE //number region
#define color3 0x2C8E  //buttons region



 int seg=0;  
 long t=0;

void draw(int x, int y)
{
  cx=-1;
  cy=-1;
  
  if(y<90)   //reset all
  {num="",n1=0;n2=0;}

  img.fillSprite(TFT_BLACK);
  img.setTextColor(TFT_WHITE,TFT_BLACK);
  img.setTextDatum(4);
  img.loadFont(small);
  //img.drawRoundRect(0,0,170,320,5,TFT_WHITE); ///border
  img.fillRoundRect(0,0,170,320,5,colors[2]);
  img.fillRoundRect(fromLeft,36,158,46,5,colors[0]);
  img.setTextColor(colors[0],colors[2]);
  img.drawString("VOLOS",34,20);
  img.setTextColor(colors[3],colors[2]);
  img.drawString("RX32",144,20);

   img.unloadFont();
   img.loadFont(sans20);  
  for(int i=0;i<n;i++){
    posY[i]=fromTop+(boxH*i)+(space*i);
    if(y>=posY[i] && y<=posY[i]+boxH)
    cy=i;
    for(int j=0;j<m;j++)
    {   
      posX[j]=fromLeft+(boxW*j)+(space*j);
      img.fillSmoothRoundRect(posX[j],posY[i],boxW,boxH,3,butColor[i][j],colors[2]);
      img.setTextColor(butTxtColor[i][j],butColor[i][j]);
      img.drawString(String(buttons[i][j]),posX[j]+boxW/2,posY[i]+boxH/2);   
       if(x>=posX[j] && x<=posY[j]+boxW)
       cx=j;
    }}
   
   img.unloadFont();

    if(cx>=0 && cy>=0){
      img.drawRoundRect(posX[cx],posY[cy],boxW,boxH,3,TFT_WHITE);
    if(buttons[cy][cx]=='0' || buttons[cy][cx]=='1' || buttons[cy][cx]=='2' || buttons[cy][cx]=='3' || buttons[cy][cx]=='4' || buttons[cy][cx]=='5' || buttons[cy][cx]=='6' || buttons[cy][cx]=='7' || buttons[cy][cx]=='8' ||  buttons[cy][cx]=='9' || buttons[cy][cx]=='.' )
  num=num+String(buttons[cy][cx]);

               if(buttons[cy][cx]=='+') {operation=1; n1=num.toFloat(); num="";}
              if(buttons[cy][cx]=='-') {operation=2; n1=num.toFloat(); num="";}
              if(buttons[cy][cx]=='*') {operation=3; n1=num.toFloat(); num="";}
              if(buttons[cy][cx]=='/') {operation=4; n1=num.toFloat(); num="";}

  if(buttons[cy][cx]=='=') {

               if(operation==1)
               {float r=n1+num.toFloat(); num=String(r); n1=num.toFloat();
               int p=r*10.00;
               if(p%10==0)
               num=String(p/10);
               }

                if(operation==2)
               {float r=n1-num.toFloat(); num=String(r); n1=num.toFloat();
                int p=r*10.00;
               if(p%10==0)
               num=String(p/10);}

                if(operation==3)
               {float r=n1*num.toFloat(); num=String(r); n1=num.toFloat();
                int p=r*10.00;
               if(p%10==0)
               num=String(p/10);}

                if(operation==4)
               {float r=n1/num.toFloat(); num=String(r); n1=num.toFloat();
                int p=r*10.00;
               if(p%10==0)
               num=String(p/10);}

              }}
  
  
  

   
   img.loadFont(star);
   img.setTextDatum(5);  
   img.setTextColor(TFT_WHITE,colors[0]);
   img.drawString(num,fromLeft+150,62,2);
    img.unloadFont();
    
    img.setTextDatum(0);  
    img.setTextColor(TFT_BLACK,colors[2]);
    img.drawString("T-Display S3 Touch",12,90);

for(int i=0;i<5;i++)
  img.fillRect(126+(i*7),86,6,10,colors[4]);  
   
   img.pushSprite(0,0);
   }

int deb=0;

void loop() 
  {

   if (touch.read()) {
    if(deb==0){
    deb=1;
      TP_Point t = touch.getPoint(0);
      if(t.x<170)
      draw(t.x,t.y);
    } }else {
    deb=0;}



  
 
  }


  