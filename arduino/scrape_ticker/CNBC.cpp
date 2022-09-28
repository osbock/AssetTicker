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

String update_symbol(String symbol, double* change){
      if((WiFi.status() == WL_CONNECTED)) {

        HTTPClient http;

        //USE_SERIAL.print("[HTTP] begin...\n");
        http.setTimeout(10000);
        http.begin("https://www.cnbc.com/quotes/"+symbol); //HTTP

        //USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            
            if(httpCode == HTTP_CODE_OK) {
              WiFiClient * myStream = http.getStreamPtr();
              int len = myStream->available();
              //USE_SERIAL.printf("%d\n",len);
              bool found = myStream->find("price\":\"");
              if (!found){
                USE_SERIAL.printf("notfound\n");
                http.end();
                return("ERR");
              }
              String Price = myStream->readStringUntil('\"');
              USE_SERIAL.printf("%s price = %s\n",symbol, Price);
              // add price change...
              found = myStream->find("priceChange\":\"");
              if (!found) USE_SERIAL.printf("notfound\n");
              String PriceChange = myStream->readStringUntil('\"');
              USE_SERIAL.printf("priceChange = %s\n",PriceChange);
              *change = PriceChange.toFloat();
   
              http.end();
              return(Price);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    *change =0.0;
    return("ERR");
}
