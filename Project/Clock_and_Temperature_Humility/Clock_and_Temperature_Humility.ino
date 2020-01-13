#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

const int DHTPIN = 2;
const int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);

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

RTC_DS1307 rtc;
//RTCDateTime dt;
char daysOfTheWeek[7][7] = {
  "Sun",
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat"
};
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
//    lcd.begin(16, 2);
//    lcd.setCursor(0, 0);

  Serial.begin(9600);
//  Serial.begin(57600);
  dht.begin();

  lcd.init();
  lcd.createChar(1, degree);
  lcd.createChar(2, thermometro);
  lcd.createChar(3, igrasia);

  rtc.begin();
  //  clock.setDateTime(__DATE__, __TIME__);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//
//
//   if (! rtc.begin()) 
//   {
//   Serial.println("Couldn't find RTC");
//   while (1);
//   }
//  
//  if (! rtc.isrunning()) 
//   {
//   Serial.println("RTC is NOT running!");
//   // following line sets the RTC to the date & time this sketch was compiled
//   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//   // This line sets the RTC with an explicit date & time, for example to set
//   // January 21, 2014 at 3am you would call:
//   // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
//   }
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

//  Serial.print("Nhiet do: ");
//  Serial.println(t);               
//  Serial.print("Do am: ");
//  Serial.println(h);             
//  
//  Serial.println();               

  //  dt = clock.getDateTime();
  DateTime now = rtc.now();

  lcd.backlight();

  lcd.setCursor(6, 1);
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
//  lcd.print(" (");
//  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
//  lcd.println(") ");

  lcd.setCursor(7, 0);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  lcd.print(" ");
  lcd.print(" ");
//  lcd.println();
  

  if (isnan(t) || isnan(h)) {} else {
    lcd.setCursor(0, 0);
//    lcd.print("T:");
    lcd.write(2);
    lcd.print(round(t));
    lcd.write(1);
    lcd.print("C");

    lcd.setCursor(0, 1);
//    lcd.print("H:");
    lcd.write(3);
    lcd.print(round(h));
    lcd.print("%");
  }

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(' ');

  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.println(") ");

   if (isnan(t) || isnan(h)) {} else {
    Serial.print("T:");
    Serial.print(round(t));
    Serial.print("^C ");

    Serial.print("H:");
    Serial.print(round(h));
    Serial.print("%");
  }

    Serial.println();
    Serial.println();


  delay(1000);
}
