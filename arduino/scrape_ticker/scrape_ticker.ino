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

extern String update_symbol(String symbol);
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
  update();
 }
#define UPDATE_INTERVAL 1000*60*1

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
  unsigned long currenttime = millis();
  if ((currenttime - last_update) > UPDATE_INTERVAL){
    last_update = currenttime;
    update();
    }


    
  
    
}
void update(){
    String BTCprice = update_symbol("BTC.CB=");
    String ETHprice = update_symbol("ETH.CB=");
    String DXYprice = update_symbol(".DXY");
    String GOLDprice = update_symbol("@GC.1");
    tft.fillScreen(CUSTOM_DARK);
    tft.setTextColor(TFT_WHITE);
    //update_symbol("BTC.CM=");
    tft.setCursor(0, 15);
    tft.setTextSize(3);
    tft.println("BTC: $" + BTCprice);
    tft.println("ETH: $" + ETHprice);
    tft.println("DXY: $" + DXYprice);
    tft.println("GOLD: $" + GOLDprice);
}


  
