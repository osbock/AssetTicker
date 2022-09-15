#include <Arduino.h>
#include <WiFi.h>


#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>

/*ESP32S3*/
#include "config.h"
#include "TFT_eSPI.h"
extern TFT_eSPI tft;
extern void parse_data(String Payload);
//icons
extern uint8_t bitcoin [];
extern uint8_t ethereum [];
extern uint8_t litecoin [];

void update_crypto(String coins){
      if((WiFi.status() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        http.begin("https://api.coincap.io/v2/assets?ids="+coins); //HTTP

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
                parse_data(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
}
StaticJsonDocument<1536> doc;
void parse_data(String input){
  // String input;



DeserializationError error = deserializeJson(doc, input);
Serial.println("in deserialization");
if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}
//tft.fillRect(60,15,260,155,CUSTOM_DARK);
  tft.fillScreen(CUSTOM_DARK);
  tft.drawBitmap(5, 5, bitcoin, 45, 45, TFT_ORANGE);
  tft.drawBitmap(5, 55, ethereum, 45, 45, TFT_BLUE);
for (JsonObject data_item : doc["data"].as<JsonArray>()) {

  const char* data_item_id = data_item["id"]; // "bitcoin", "ethereum"
  const char* data_item_rank = data_item["rank"]; // "1", "2"
  const char* data_item_symbol = data_item["symbol"]; // "BTC", "ETH"
  const char* data_item_name = data_item["name"]; // "Bitcoin", "Ethereum"
  USE_SERIAL.print(String(data_item_name));USE_SERIAL.print(": ");
  const char* data_item_supply = data_item["supply"]; // "19149925.0000000000000000", ...
  const char* data_item_maxSupply = data_item["maxSupply"]; // "21000000.0000000000000000", nullptr
  const char* data_item_marketCapUsd = data_item["marketCapUsd"]; // "385776251642.8157997053189150", ...
  const char* data_item_volumeUsd24Hr = data_item["volumeUsd24Hr"]; // "20176762685.9265514054208251", ...
  const char* data_item_priceUsd = data_item["priceUsd"]; // "20145.0528732000673478", ...
  double price = String(data_item_priceUsd).toFloat();
  USE_SERIAL.println(String(price,2));
  tft.setTextColor(TFT_WHITE);
  if (*data_item_rank == '1')
    tft.setCursor(60, 15);
  if (*data_item_rank == '2')
    tft.setCursor(60, 70);
  tft.setTextSize(3);
  tft.print("$");
  tft.println(String(price,2));

  const char* data_item_changePercent24Hr = data_item["changePercent24Hr"]; // "-9.9315691374496807", ...
  const char* data_item_vwap24Hr = data_item["vwap24Hr"]; // "21777.6169300201272102", ...
  const char* data_item_explorer = data_item["explorer"]; // "https://blockchain.info/", ...
//tft.setCursor(60, 120);
//tft.println("Iteration:"+String(iteration++));
}

long long timestamp = doc["timestamp"]; // 1663106567693
}
