#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(11, 12); // RX, TX

DFRobotDFPlayerMini myDFPlayer;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);

const int trig = 9;     // chân trig của HC-SR04
const int echo = 10;     // chân echo của HC-SR04

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

int i;

void setup()
{
    Serial.begin(9600);     // giao tiếp Serial với baudrate 9600
    mySoftwareSerial.begin(9600);

    // initialize the LCD
    lcd.begin();

    // Turn on the blacklight and print a message.
    lcd.backlight();
    lcd.createChar(1, degree);
    wellcome();

    if (!myDFPlayer.begin(mySoftwareSerial, true, false)) {  while(true){delay(0); }   }
  myDFPlayer.volume(25); 
  soundwellcome();
    
    pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
    pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu

    mlx.begin();

    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    
    for (i = 1;i<=10;i=i+1) {
        ledlaunch();
    }
    delay(2000);
   
}
 
void loop()
{
  digitalWrite(5, HIGH); //....
  delay(80);
  digitalWrite(5, LOW);
  delay(80);
  digitalWrite(5, HIGH); //....
  delay(80);
  digitalWrite(5, LOW);
  delay(80);
  
  int t;
    unsigned long duration; // biến đo thời gian
    int distance;           // biến lưu khoảng cách
    
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distance = int(duration/2/29.412);
    
    /* In kết quả ra Serial Monitor */
    Serial.print(distance);
    Serial.println("cm");
    Serial.print("*CtObject = "); Serial.print(mlx.readObjectTempC()+1.5); Serial.println("*C");
    Serial.print("*FtObject = "); Serial.print(mlx.readObjectTempF()+48); Serial.println("*F");

    if (distance <=3 ){
      if(((mlx.readObjectTempC()+1.5) > 37.5) || ((mlx.readObjectTempC()+1.5) < 35.0)){
        digitalWrite(4, LOW);
        unhidetemperature();
        delay(2000);
         for (t = 1;t<=10;t=t+1) {
           digitalWrite(4, HIGH); //....
           delay(80);
           digitalWrite(4, LOW);
           delay(80);
           digitalWrite(4, HIGH); //....
           delay(80);
           digitalWrite(4, LOW);
           delay(80);
        }
        lcd.setCursor(2,0);
        lcd.print("!!!ALARM!!!");
        lcd.setCursor(0,1);
        lcd.print("----------------");
        soundalarm();
       
      }else{
        unhidetemperature();
        soundhelithynormal();
        digitalWrite(6, HIGH);
        delay(2000);
        }
    }else{
      checktemperature();
      
      soundwellcomehello();
      digitalWrite(6, LOW);
    }  
    delay(50);
}

void wellcome(){
   lcd.clear();
   lcd.setCursor(3,0);
   lcd.print("LOADING");
   lcd.setCursor(0,1);
   lcd.print("...............");
}
void checktemperature(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please measure");
  lcd.setCursor(0,1);
  lcd.print("the temperature");
}
void unhidetemperature(){
  lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Temperature");
    
    lcd.setCursor(5,1);
    lcd.print(mlx.readObjectTempC()+1.5);
    lcd.write(1);
    lcd.print("C");

//    lcd.setCursor(8,1);
//    lcd.print(mlx.readObjectTempF()+48);
//    lcd.write(1);
//    lcd.print("F");
}
void soundwellcome(){
  myDFPlayer.play(1);
  delay(4000); 
}
void soundalarm(){
  myDFPlayer.play(2);
  delay(11000); 
}
void soundhelithynormal(){
  myDFPlayer.play(3);
  delay(3000); 
}
void soundwellcomehello(){
  myDFPlayer.play(4);
  delay(3000); 
}
void ledlaunch(){
  digitalWrite(4, HIGH); //....
  delay(60);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(60);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  delay(60);
  digitalWrite(6, LOW);
  // reverse
  digitalWrite(6, HIGH);
  delay(60);
  digitalWrite(6, LOW);
  digitalWrite(5, HIGH);
  delay(60);
  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);
  delay(60);
  digitalWrite(4, LOW);
}
