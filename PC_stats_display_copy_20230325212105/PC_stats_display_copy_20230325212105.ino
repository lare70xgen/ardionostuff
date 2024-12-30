//Libraries 
#include "TFT_eSPI.h" //TFT LCD library 
#include "Free_Fonts.h" //include the font library

//Initializations
TFT_eSPI tft; //Initializing TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); //Initializing buffer

void setup() {
  Serial.begin(9600); //start serial communication 
  tft.begin(); //Start TFT LCD
  tft.setRotation(1); //Set LCD rotation 1=Down, 3=Up
  spr.createSprite(TFT_HEIGHT,TFT_WIDTH); //Create buffer
}

//initialize data types for variables 
String serialReceive;  
String IP;
//String cpufreq;


void loop() {
  if(Serial.available() > 0) { //check whether if any data is available in serial buffer
    serialReceive = Serial.readString(); //read it as string and put into serialReceive variable
  }
  
  IP = serialReceive.substring(0, 30); //split the received long string to substrings. (5 characters/information)
  //cpufreq = serialReceive.substring(5, 10);
  
  spr.fillSprite(tft.color565(0,255,255)); //Fill background with white color

  //spr.drawFastHLine(0,84,320,TFT_BLACK); //Draw horizontal line
  //spr.drawFastHLine(0,143,320,TFT_BLACK); 

  //CPU name 
  spr.setTextColor(TFT_BLACK); //set text color
  spr.setFreeFont(FSSB12); //set font 
  //spr.drawString("Intel i5",5,5); //draw string 

  //CPU stats 
  spr.setFreeFont(FSS12);
  spr.drawString("IP: ",5,8);
  spr.drawString(IP,35,8); 
  //spr.drawString("%",120,35);
  //spr.drawString(": ",10,60);
  //spr.drawString(cpufreq,75,60);
  //spr.drawString("GHz",118,60);

  //GPU name

  //GPU stats 
  spr.setFreeFont(FSS12);

  //RAM name
  spr.setFreeFont(FSSB12);
  //spr.drawString("8GB DDR4",10,90);

  //RAM stats
  spr.setFreeFont(FSS12);
  //spr.drawString("Used: ",10,120);
  
  //SSD and HDD names
  spr.setFreeFont(FSSB12);
  //spr.drawString("1TB HDD",170,150);

  //SSD stats
  spr.setFreeFont(FSS12);
  //spr.drawString("Used: ",10,180);

  //spr.drawFastVLine(165,0,84,TFT_BLACK); //draw vertical line
  //spr.drawFastVLine(165,143,240,TFT_BLACK);

  //HDD stats
  spr.setFreeFont(FSS12);
  //spr.drawString("Used: ",170,180);
  spr.pushSprite(0,0); //Push to LCD
  delay(50);

}
