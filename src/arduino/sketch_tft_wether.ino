#include "tft_setup.h"
//#include "Arduino.h" 
#include "SPI.h"
#include "TFT_eSPI.h"
//#include <Fonts\GFXFF\FreeSans9pt7b.h>

#include <WiFi.h>
#include <HTTPClient.h>
//#include <ArduinoJson.h>

#define TFT_BG_COLOOR TFT_BLACK
#define inPinMove 15

#include <deepSleep.hpp>
#include <WetherForecast.hpp>
#include <icons.h>

const char* ssid = "l2qwe";
const char* password = "19871990";



TFT_eSPI tft = TFT_eSPI();
HTTPClient http = HTTPClient();
WetherForecast wetherForecast = WetherForecast(&http);
Wether wether;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println(""); Serial.println("");
  Serial.println("Begin init."); 
 
  setup_tft();
  setup_wifi();
  
  Serial.println("Begin done."); 

  renderWetherScreen();
  delay(20*1000);
  gotoDeepSleep();
}

void loop() {
}

void setup_tft(){
  tft.init();    
  tft.fillScreen(TFT_BG_COLOOR);
  tft.setTextColor(TFT_WHITE);  
  tft.setFreeFont(&FreeSans9pt7b);
  //tft.loadFont(TFT_FONT_RUS16);
  //tft.setTextSize(24);
  tft.setSwapBytes(true);
}

void setup_wifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void renderWetherScreen(){
  tft.fillScreen(TFT_BG_COLOOR);
  tft.setCursor(0,0);
  
  drawDevider();
  
  wetherForecast.get(0, &wether);
  showWether(&wether);
  
  drawDevider();
  
  wetherForecast.get(3, &wether);
  showWether(&wether);
  
  drawDevider();
  
  wetherForecast.get(6, &wether);
  showWether(&wether);
  
  drawDevider();
}
void drawDevider(){
  int16_t zeroY = tft.getCursorY();
  zeroY+=2;
  tft.drawLine(0, zeroY, TFT_WIDTH, zeroY, TFT_YELLOW);
  tft.drawLine(0, zeroY+2, TFT_WIDTH, zeroY+2, TFT_YELLOW);
  zeroY+=4;
  tft.setCursor(0, zeroY);
}

#define IconPhraseOffsetX 1
#define IconPhraseOffsetY 5

#define TimeAndWindOffsetX 20
#define TimeAndWindOffsetY IconPhraseOffsetY + 37

#define TemperatureOffsetX 0
#define TemperatureOffsetY TimeAndWindOffsetY + 3

#define FinalyOffsetY TemperatureOffsetY + WETHER_ICO_HEIGHT + 8

void showWether(Wether* wether){
  int16_t zeroY = tft.getCursorY();
  
  tft.unloadFont();
  tft.setCursor(0, zeroY + IconPhraseOffsetY);
  tft.loadFont(TFT_FONT_RUS_SMALL);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);  
  tft.println(wether->IconPhrase);
  
  tft.unloadFont();  
  tft.setFreeFont(&FreeSans9pt7b);
  tft.setTextSize(1);

  tft.setCursor(0 + TimeAndWindOffsetX, zeroY + TimeAndWindOffsetY);
  tft.print(wether->TimeMoscow);
  tft.print("       ");
  tft.pushImage(0 + TimeAndWindOffsetX + 50 , zeroY + TimeAndWindOffsetY - WIND_ICO_HEIGHT + 5, WIND_ICO_WIDTH, WIND_ICO_HEIGHT, img_wind);    

  tft.print(wether->WindSpeed);
  tft.println("m/s");

  tft.pushImage(0 + TemperatureOffsetX, zeroY + TemperatureOffsetY + 4, WETHER_ICO_WIDTH ,WETHER_ICO_HEIGHT, imgs[calcIndex(wether->Icon)]);    

  int8_t extOffset = 17;
  if(wether->TemperatureValue > 9 || wether->TemperatureValue < -9){
    extOffset = 0;
  }

  tft.setCursor(0 + WETHER_ICO_WIDTH + TemperatureOffsetX + extOffset, zeroY + WETHER_ICO_HEIGHT + TemperatureOffsetY);
  tft.setTextSize(3);

  if(wether->TemperatureValue < 0){
    tft.setTextColor(TFT_BLUE);
  }else{
    tft.setTextColor(TFT_MAROON);
    tft.print("+");
  }      
  tft.print(wether->TemperatureValue);


  tft.setCursor(0, zeroY + FinalyOffsetY);
}

uint8_t calcIndex(uint8_t iconNumber){
  if(iconNumber >= 27){
    return iconNumber - 1 - 4;
  }
  if(iconNumber >= 9 /*09*/){
    return iconNumber - 1 - 2;
  }
  return iconNumber - 1;
}


void tetsFont(){
}