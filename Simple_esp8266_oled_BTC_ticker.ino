
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define OLED_MOSI  13 //D1
#define OLED_CLK   14 //D0
#define OLED_DC    0 // Data/Command
#define OLED_CS    4 // Chip select
#define OLED_RESET 15 // RESET

// SPI OLED
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

const char* ssid = "BlackPanther";
const char* password = "ladyluck";

//coindesk btc api (canadian price)
const String fullurl = "http://api.coindesk.com/v1/bpi/currentprice/CAD";

void setup() {

Serial.begin(115200);
  // initialize display
  display.begin(SSD1306_SWITCHCAPVCC);
    display.display();
    delay(2000);
  display.clearDisplay();
  display.display();


 WiFi.setOutputPower(0);   // reset wifi
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);    // force set to wifi station (client) mode when initializing
 WiFi.begin(ssid, password);
 delay(2000);
  display.setTextSize(0.5);
  display.setTextColor(INVERSE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("BTC PRICE TICKER");
  display.println("Starting");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected to: ");
  display.println(ssid);
  display.println("IP address: ");
  display.println(WiFi.localIP());
  display.display();
  delay(1000);

}
void loop() {

   HTTPClient http;
   http.begin(fullurl);
   int httpCode = http.GET();
   Serial.println(httpCode);

  String content = http.getString();

  //const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  //DynamicJsonBuffer jsonBuffer; //(BUFFER_SIZE);
   StaticJsonBuffer<800> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(content);

  //Serial.println(content);

  String currentTime = root["time"]["updated"];
  float CADprice = root["bpi"]["CAD"]["rate_float"];
  float USDprice = root["bpi"]["USD"]["rate_float"];

USDprice = round(USDprice*1000)/1000;
CADprice = round(CADprice);
  //Serial.println(USDprice);
  display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(BLACK, WHITE);
    display.println("CURRENT BTC PRICE: ");
    display.display();
    display.setTextColor(WHITE);
    display.setCursor(0, 12);
    display.print("USD = $");
  display.println(USDprice);
      display.setCursor(0, 24);
    display.print("CAD = $");
  display.println(CADprice);
      display.display();
        delay(5000); 
 display.clearDisplay();
    display.setCursor(0, 0);
     display.setTextSize(1);
     display.setTextWrap(0);
     display.setTextColor(WHITE);
    display.println(currentTime);
    display.setTextSize(1);
    display.setCursor(0, 12);
    display.print("USD = $");
  display.println(USDprice);
      display.setCursor(0, 24);
    display.print("CAD = $");
  display.println(CADprice);
      display.display();
        delay(5000); //alternate to show time every 5 sec
  
  
  

}
