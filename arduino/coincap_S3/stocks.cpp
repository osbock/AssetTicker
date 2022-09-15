#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
/*ESP32S3*/
#include "config.h"
#include "TFT_eSPI.h"
extern TFT_eSPI tft;

extern void parse_stock(String Payload);
extern uint8_t boom [];
void update_stock(String symbol){
      tft.drawBitmap(5, 120, boom, 45, 45, TFT_RED);
      if((WiFi.status() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        String request = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol="+symbol+"&apikey=" + STOCK_API_KEY;
        //Serial.print(request);
        http.begin(request); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                parse_stock(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
}
StaticJsonDocument<512> stockdoc;
void parse_stock(String payload){
  // String input;



DeserializationError error = deserializeJson(stockdoc, payload);
//Serial.println(payload);
if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}

JsonObject Global_Quote = stockdoc["Global Quote"];
const char* Global_Quote_01_symbol = Global_Quote["01. symbol"]; // "COF"
const char* Global_Quote_02_open = Global_Quote["02. open"]; // "101.5400"
const char* Global_Quote_03_high = Global_Quote["03. high"]; // "101.6200"
const char* Global_Quote_04_low = Global_Quote["04. low"]; // "98.5300"
const char* Global_Quote_05_price = Global_Quote["05. price"]; // "100.6500"
double price = String(Global_Quote_05_price).toFloat();
Serial.println(price);
tft.setCursor(60, 125);
tft.setTextSize(3);
tft.print("$");
tft.println(String(price,2));
const char* Global_Quote_06_volume = Global_Quote["06. volume"]; // "3349982"
const char* Global_Quote_07_latest_trading_day = Global_Quote["07. latest trading day"]; // "2022-09-14"
const char* Global_Quote_08_previous_close = Global_Quote["08. previous close"]; // "101.0400"
const char* Global_Quote_09_change = Global_Quote["09. change"]; // "-0.3900"
const char* Global_Quote_10_change_percent = Global_Quote["10. change percent"]; // "-0.3860%"

}
