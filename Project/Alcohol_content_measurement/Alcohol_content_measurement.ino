#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int val=0;

void setup(){
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  
}

void loop(){
  val=analogRead(A0);
  Serial.println(val);

  lcd.setCursor(0,0);
  lcd.print("Alcohol");
  lcd.setCursor(0,1);
  lcd.print(val);
  
  delay(100);
}
