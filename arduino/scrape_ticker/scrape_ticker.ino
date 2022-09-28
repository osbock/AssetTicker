/**
 * Coincap API ticker
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>

/*ESP32S3*/
#include "config.h"
#include "TFT_eSPI.h"/* Please use the TFT library provided in the library. */

TFT_eSPI tft = TFT_eSPI();

extern String update_symbol(String symbol,double* change);
extern void update();

unsigned int iteration=0;
unsigned long last_update =0L;

void setup() {
    USE_SERIAL.begin(115200);
    pinMode(PIN_POWER_ON, OUTPUT);
    digitalWrite(PIN_POWER_ON, HIGH);
    pinMode(14,INPUT_PULLUP);

    WiFi.mode(WIFI_STA);
        //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
  
  wm.autoConnect("AssetTracker");
  
  Serial.println("we must be connected");
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(CUSTOM_DARK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextWrap(true);
  tft.setCursor(0, 170);
  tft.setTextSize(2);
  delay(3000); //let things settle down
  update();
 }
#define UPDATE_INTERVAL 1000*10*1

void loop() {
   // is configuration portal requested?
  if ( digitalRead(14) == LOW) {
    WiFiManager wm;    
 
    //reset settings - for testing
    //wm.resetSettings();
  
    // set configportal timeout
    wm.setConfigPortalTimeout(120);
 
    if (!wm.startConfigPortal("AssetTracker")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }
  }
  maybeUpdateData();
  unsigned long currenttime = millis();
  if ((currenttime - last_update) > UPDATE_INTERVAL){
    last_update = currenttime;
    update();
    }


    
  
    
}

void outputSymbol(String symbol){ 
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW);
  tft.print(symbol);
}
void outputChange(double change){
  if (change < 0) tft.setTextColor(TFT_RED);
    else tft.setTextColor(TFT_GREEN);
    tft.print(String(change));
}
String lBTCprice,lETHprice,lDXYprice,lGOLDprice, lSILVERprice;
double lBTCChange,lETHChange,lDXYChange,lGLDChange, lSLVChange;
#define API_UPDATE_INTERVAL 12000
unsigned long LastAPIUpdate =0.0L;
int update_index = 0;
String symbols[] = {"BTC.CB=","ETH.CB=",".DXY", "@GC.1","@SI.1"};
double *changes[] = {&lBTCChange,&lETHChange,&lDXYChange,&lGLDChange,&lSLVChange};
String *prices[] = {&lBTCprice,&lETHprice,&lDXYprice,&lGOLDprice,&lSILVERprice};

void maybeUpdateData(){
  unsigned long current = millis();
  if ((current -LastAPIUpdate) > API_UPDATE_INTERVAL)
  {
    USE_SERIAL.printf("updating %s\n",symbols[update_index]);
    LastAPIUpdate = current;
    double change;
    String price = update_symbol(symbols[update_index],&change);
    if (!price.equals("ERR")){
      *prices[update_index] = price;
      *changes[update_index] = change;
    }
    (++update_index==5)?update_index=0:update_index;
    USE_SERIAL.printf("update_index %d\n",update_index);
  }
}
void update(){
    tft.fillScreen(CUSTOM_DARK);
    tft.setTextColor(TFT_YELLOW);

    tft.setCursor(0, 15);
    tft.setTextSize(3);
    int screen_width = tft.width();
    int width = tft.textWidth("$"+lBTCprice);
    outputSymbol("BTC:");
    outputChange(lBTCChange);
    tft.setTextSize(3);
    tft.setCursor(screen_width-width, tft.getCursorY());
    tft.println("$" + lBTCprice);
    width = tft.textWidth("$"+lETHprice);
    outputSymbol("ETH:");
    outputChange(lETHChange);
    tft.setTextSize(3);
    tft.setCursor(screen_width-width, tft.getCursorY());
    tft.println("$" + lETHprice);
    width = tft.textWidth("$"+lDXYprice);
    outputSymbol("DXY:");
    outputChange(lDXYChange);
    tft.setTextSize(3);
    tft.setCursor(screen_width-width, tft.getCursorY());
    tft.println("$" + lDXYprice);
    outputSymbol("GLD:");
    outputChange(lGLDChange);
    tft.setTextSize(3);
    width = tft.textWidth("$"+lGOLDprice);
    tft.setCursor(screen_width-width, tft.getCursorY());
    tft.println("$" + lGOLDprice);
    outputSymbol("SLV:");
    outputChange(lSLVChange);
    tft.setTextSize(3);
    width = tft.textWidth("$"+lSILVERprice);
    tft.setCursor(screen_width-width, tft.getCursorY());
    tft.println("$" + lSILVERprice);
}


  
