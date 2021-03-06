#include <TFT_eSPI.h>                
TFT_eSPI    tft = TFT_eSPI();  
#include "back.h"
#include "player.h"
#include "asteroid.h"
#include "logo.h"
#include "button.h"
#include "gameover.h"

#define TFT_GREY 0x5AEB
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

//player
float pX=random(30,100);
float pY=200;
int pW=24;
int pH=29;
float pXs=0.12;
float pYs=0.2;
int pos[4]={2,35,68,101};

//bulet
float bx[10];
float by[10];
int bw=2;   //player bullet size
float bs=0.35; //player bulett speed

//enemies
float ex[8];
float ey[8];
int ew=24;
int eh=24;
float esy=0.18;
float esx=0;

float pres1=0;//debounce A button
float pres2=0;//debounce B button
int counter=0;
int fire=0;
int score=0;
int timeAlive=0;
int currentTime=0;
int newLevelTime=15;

float spaceX[30];
float spaceY[30];

void setup() {
  pinMode(0,INPUT_PULLUP);
  pinMode(35,INPUT_PULLUP);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

   ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, 67);


    for(int i=0;i<8;i++)
    {
    ex[i]=random(2,102);
    ey[i]=-50;
    
    
    }

    for(int i=0;i<10;i++)
    by[i]=250;

    for(int i=0;i<30;i++){
    spaceY[i]=random(20,235);
    spaceX[i]=random(2,132);
  }

}
int count=0;
int startCounter=0;
int endCounter=0;
int aniFrame=0;
int level=1;

int fase=0; //fase 0=start screen 1=play 2=gameover

void loop() {

  if(fase==0)
     {
     delay(500);
     tft.fillScreen(TFT_BLACK);
     tft.setTextColor(TFT_WHITE,TFT_BLACK);
     tft.pushImage(5,5,133,115,logo);
     tft.pushImage(28,125,72,72,button);
     tft.drawString("FOR TTGO",5,210,2);
     tft.drawString("T-DISPLAY",5,228,1);
     while(digitalRead(0)==1)
        {
      
        }
     tft.fillScreen(TFT_BLACK);
     tft.setTextColor(TFT_WHITE,0x01E8);
     tft.fillRect(0,0,135,17,0x01E8);
     tft.drawString("LVL:",2,1,2);
     tft.drawString("TIME:",70,1,2);
     tft.drawLine(0,17,135,17,0x6577);
     
     
     fase=1; 
     }

  if(fase==1){
 
  if(pX<2 || (pX>135-pW-1))
  pXs=pXs*-1;
  
  pX=pX+pXs;
  
  if(millis()-currentTime>1000)
  {
    timeAlive++;
    tft.drawString(String(timeAlive),105,1,2);
   
    currentTime=millis();
    if(timeAlive==newLevelTime)
      {
        level++;
        tft.drawString("LVL:"+String(level),2,1,2);
        esy=esy+0.02;
        pXs=pXs+0.01;
        newLevelTime=newLevelTime+15;
      }
   }
    

    
 
  

  if(startCounter==0)
  if(count<8)
  {
    startCounter=1;
    ey[count]=19;
    count++;
    if(count==8)
    count=0;
    endCounter=random(150-(level*15),1000-(level*180));//.....................................
  }
  
  draw();
  checkButton();
  drawSpace();



for(int i=0;i<10;i++)
   {
    if(by[i]>23 && by[i]<240)
    by[i]=by[i]-bs;
  
   }

for(int i=0;i<8;i++)
   {
    if(ey[i]!=-50.00){
    ey[i]=ey[i]+esy;
    ex[i]=ex[i]-esx;
    }

    if(ey[i]>242)
        {
        
         ex[i]=random(2,102);
         ey[i]=-50;
         
         
          
        }
   }   

  if(fire>0)
  fire++;
  if(fire>60)
  fire=0;

  if(startCounter>0)
  startCounter++;
  if(startCounter>=endCounter)
  startCounter=0;


  collision();
   
 }

 if(fase==2)
    {
      tft.fillScreen(TFT_BLACK);
      tft.pushImage(0,0,135,240,gameover);
       tft.drawString("LVL: "+String(level),25,120,2);
      tft.drawString("TIME: "+String(timeAlive),25,140,2);
      delay(500);
      while(digitalRead(37)==1);
      restart();
      fase=0;
      
    }
 
 }

void draw()
  {
 
  tft.pushImage(pX,pY,pW,pH,player);

      for(int i=0;i<10;i++)
      { 
      
        tft.fillCircle(bx[i],by[i],bw+2,TFT_BLACK);
        if(by[i]>23)
        tft.fillCircle(bx[i],by[i],bw,TFT_RED);
      }

   for(int i=0;i<8;i++)
      { 
        if(ex[i]<-32)
       delay(1);
        else{
       tft.pushImage(ex[i],ey[i],ew,eh,asteroid);
        }
        
      }    
  }

void checkButton()
    {
    if(digitalRead(0)==0 && digitalRead(35)==1 ){
    if(pres1==0)
    {
      pres1=1;
      pXs=pXs*-1;
      
    }}else {pres1=0;}
   


/*
   if(digitalRead(0)==0 && digitalRead(35)==1){
    if(pres2==0)
       {
        if(fire==0){
        pres2=1;
        bx[counter]=pX+pW/2;
        by[counter]=pY-bw/2;
        counter++;
        fire=1;
        if(counter>=10)
        counter=0;}
       }}else pres2=0; 
    */   
    }  

    void collision()
    {
      for(int i=0;i<10;i++){
        for(int j=0;j<8;j++){

            if(pX+pW/2>ex[j]-2 && pX+pW/2<ex[j]+ew-2 && ey[j]+22>pY && ey[j]<225)
                 fase=2;
        
           
        } }
      
      }

      void restart()
      {
      level=1;
      esy=0.18;
      for(int i=0;i<8;i++)
      {
      ex[i]=random(2,102);
      ey[i]=-50;

    
    
    }

    for(int i=0;i<10;i++)
    by[i]=250;

pres1=0;//debounce left button
pres2=0;//debounce right button
counter=0;
fire=0;
score=0;
timeAlive=0;
currentTime=0;
newLevelTime=15;
count=0;
startCounter=0;
endCounter=0;
aniFrame=0;
pXs=0.12;
        }

        void drawSpace()
        {
            for(int i=0;i<30;i++){ //drawStars..........................................
            tft.drawPixel(spaceX[i],spaceY[i],TFT_BLACK);
           spaceY[i]=spaceY[i]+0.08;
          tft.drawPixel(spaceX[i],spaceY[i],0xE73C);
        if(spaceY[i]>240){
        tft.drawPixel(spaceX[i],spaceY[i],TFT_BLACK);
  
  spaceY[i]=19;
  }}
          
          }
    
