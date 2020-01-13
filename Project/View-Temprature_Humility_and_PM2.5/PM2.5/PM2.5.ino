#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

const int DHTPIN = 6;
const int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);
/* GP2Y1010AU0F Compact Optical Dust Sensor + Adapter  (GP2Y1010AU0F,DFR0280) [S065] : http://rdiot.tistory.com/114 [RDIoT Demo] */
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F, 20, 4);

int measurePin = A0;
int ledPower = 2;
 
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
 
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
 
int B = 9; // OUTPUT PIN blue
int R = 11; // OUTPUT PIN red
int G = 10; // OUTPUT PIN green
 
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

byte traitim[8]={
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000
};

void setup()
{
  lcd.init();  // initialize the lcd 
  lcd.backlight();
  lcd.print("Hello Nguyen");

  lcd.createChar(1, degree);
  lcd.createChar(2, thermometro);
  lcd.createChar(3, igrasia);
  lcd.createChar(4, traitim);
  
  pinMode(ledPower,OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
 
  delay(1000);
  lcd.clear();
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

//  lcd.setCursor(0,0);
//  lcd.print("S065:Dust GP2Y1010AU0F");
 
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
 
  // 0 - 5.0V mapped to 0 - 1023 integer values 
  calcVoltage = voMeasured * (5.0 / 1024); 
  
  dustDensity = (0.17 * calcVoltage - 0.1)*100; 
  
  lcd.setCursor(0,0);
  lcd.print("Signal: " + (String)voMeasured + " ");
//  lcd.print((String)voMeasured + " ");
  
  lcd.setCursor(0,1);
  lcd.print("Voltage: " + (String)calcVoltage + " ");
//  lcd.print("V: " + (String)calcVoltage + " ");
  
  lcd.setCursor(0,2);
  lcd.print("PM2.5: " + (String)dustDensity + "ug/m3 ");
//  lcd.print("P2.5:" + (String)dustDensity + "ug/m3 ");

   if (isnan(t) || isnan(h)) {} else {
    lcd.setCursor(0, 3);
    lcd.write(2);
    lcd.print(round(t));
    lcd.write(1);
    lcd.print("C");

    lcd.setCursor(7, 3);
    lcd.write(3);
    lcd.print(round(h));
    lcd.print("%");
  }
  
  
  if(dustDensity <= 40)
  {
   // GREEN
   digitalWrite(R, HIGH);
   digitalWrite(G, LOW);
  }else if(dustDensity <= 80)
  {
   // BLUE
   digitalWrite(G, HIGH);
   digitalWrite(B, LOW);
  }
  else if(dustDensity <= 120)
  {
    // RED + GREEN = YELLOW
    digitalWrite(B, HIGH);
    digitalWrite(R, LOW);
    digitalWrite(G, LOW);
  }
  else
  {
    // RED
    digitalWrite(R, LOW);
    digitalWrite(G, HIGH);
    digitalWrite(B, HIGH); 
  }

//  lcd.noDisplay();
  delay(1000);
//  lcd.display();
 
  // all off
  digitalWrite(B, HIGH);
  digitalWrite(R, HIGH);
  digitalWrite(G, HIGH);
}
