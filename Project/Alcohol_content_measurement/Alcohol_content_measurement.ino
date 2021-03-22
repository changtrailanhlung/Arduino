#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensorPin = A0;
float value;

void setup(){
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  
}

void loop(){
  int Value = analogRead(sensorPin);
  value = analogRead(A0);
  Serial.println((value/1000)-(-0.01)-0.03);

  lcd.setCursor(0,0);
  lcd.print("Alcohol:");
  lcd.setCursor(9,0);
  lcd.print((value/1000)-(-0.01)-0.03);
  lcd.print("mg/l");

  if ((value/1000)-(-0.01)-0.03 > 0.17){
    lcd.setCursor(0,1);
    lcd.print(".....Danger.....");
  }else{
    lcd.setCursor(0,1);
    lcd.print(".....Normal.....");
  }
  
  delay(100);
}
