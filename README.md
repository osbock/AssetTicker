# AssetTicker
Crypto and Stock Asset ticker based on Lilygo T-Display-S3
![Ticker in action](./images/ticker_hero.jpg)
This started as an attempt to port over and improve the Cryptoticker from the TTGO-T4 example code. The original used an API from CoinMarketCap that has since been deprecated, and the new one (I did find a fork for that) is more and more limited as they are limiting the functionality of the free API vs. a paid one.

The Coincap API is free, reasonable limits, and while it doesn't have as many of the same up-down comparison metrics, it will do. The Stock API currently included isn't so hot, with only daily updates, so if anyone knows of a better one, let me know.

While not the same shape and size, the new Lilygo T-Display-S3 features a really sharp display with The new ESP-S3 processor which features more memory and several other interesting features. The problem is that this board is not very well documented, so I will attempt to also consolodate my learnings here.

## Program notes
A lot can be improved, but you can edit config.h to put in your stock api key
As I said above, the api isn't real time, but you can check it out here:
https://www.alphavantage.co/documentation/
There is some configurability in the stocks and cryptocurrencies, but the symbols aren't currently looked up, so you'll have to manually change the code.

The WiFiManager is the latest version from github, because I couldn't get the library manager to work. The "Key" button will run the portal on demand if you need to configure past the first time. I hope to eventually add parameters to the portal page.

## Arduino Settings
The most important thing is to get the latest ESP32 board support package because the S3 Variant is fairly new and offers quite a few new features.

Select 
 * ESP32S3 Dev Module
 * Flash Size: 16 MB
 * Partition Scheme: "Huge APP (3MB No OTA/1MB spiffs) // note: this is probably flexible but what I'm currently using
 * USBCDC On Boot: "Enabled" // Note: if you don't do this your Serial.println won't work
 * PSRAM: "OPI PSRAM

Select the port that shows up. I believe those settings above are different than defaults, but I've included a screenshot of current working settings.

![Arduino Board settings](./images/arduinosettings.png)
Here is the pinout diagram that is provided:
![pinout diagram](./images/T-DISPLAY-S3.jpg)

