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
        String request = "https://finnhub.io/api/v1/quote?symbol="+symbol+"&token=" + STOCK_API_KEY;
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

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}

float current_price = stockdoc["c"]; // 101.68
float d = stockdoc["d"]; // -1.09
float dp = stockdoc["dp"]; // -1.0606
float h = stockdoc["h"]; // 102.21
float l = stockdoc["l"]; // 99.42
float o = stockdoc["o"]; // 101.56
float pc = stockdoc["pc"]; // 102.77
long t = stockdoc["t"]; // 1663358402
Serial.println(current_price);
tft.setCursor(60, 125);
tft.setTextSize(3);
tft.print("$");
tft.println(String(current_price,2));


}
