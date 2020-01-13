#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
//#include <Wire.h>
//#include "DHT.h"
//
//#define DHTTYPE DHT11   
//#define DHTPin 8
//
//  // Initialize DHT sensor.
//DHT dht(DHTPin, DHTTYPE);

const char *ssid     = "Home 2.4ghz";
const char *password = "07531592486";

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tues", 
"Wed", "Thu", "Fri", "Sat"};

WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
NTPClient timeClient(ntpUDP);

String formattedDate;
String dayStamp;
String timeStamp;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

byte thermometro[8] = //icon for termometer
{
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};


byte igrasia[8] = //icon for water droplet
{
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110,
};

void setup() {
  Serial.begin(115200);

//  dht.begin();

  lcd.init();                                           // Initialize the lcd 
  lcd.backlight();

  lcd.createChar(1, degree);
  lcd.createChar(2, thermometro);
  lcd.createChar(3, igrasia);

  WiFi.begin(ssid, password);
  int cursorPosition=0;
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("   Connecting");
   
  Serial.println("Conecting Wifi");
  Serial.println(ssid);
  
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    lcd.setCursor(cursorPosition,2); 
    lcd.print(".");
    cursorPosition++;
    
  }
  lcd.clear();                                          // Clear LCD display  
  lcd.setCursor(0,0);                                   // Set cursor position
  lcd.print("Wifi: ");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  delay(2000);

  lcd.clear();                                          // Clear LCD display  
  lcd.setCursor(0,0);                                   // Set cursor position
  lcd.print("IP:");
  lcd.setCursor(0,1);  
  lcd.print(WiFi.localIP());
  delay(4000);

  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  // GMT +7 = 25200
  timeClient.setTimeOffset(25200);

}

void loop() {
  
//  float h = dht.readHumidity();
//  float t = dht.readTemperature();
//
//  if (isnan(t) || isnan(h)) {} else {
//    lcd.setCursor(0, 0);
//    lcd.write(2);
//    lcd.print(round(t));
//    lcd.write(1);
//    lcd.print("C");
//
//    lcd.setCursor(0, 1);
//    lcd.write(3);
//    lcd.print(round(h));
//    lcd.print("%");
//  }
  
  
  while(!timeClient.update()) {
    timeClient.forceUpdate();
    lcd.clear();
    lcd.setCursor(0,0);  
    lcd.print("Update");
    lcd.setCursor(0,1);  
    lcd.print("----------------");
  }

  formattedDate = timeClient.getFormattedDate();

  
  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);

  //  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  
  lcd.clear();                                          // Clear LCD display  
  lcd.setCursor(0,0);                                   // Set cursor position
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.setCursor(6,0); 
  lcd.print(dayStamp);
//  lcd.print(" ");
  
  lcd.setCursor(7,1);  
  lcd.print(timeStamp);
//  lcd.print(" ");
//  lcd.print(" ");
 
  Serial.println(dayStamp);
  Serial.println(timeStamp);
  delay(1000);

}
