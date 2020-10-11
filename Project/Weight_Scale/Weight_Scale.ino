#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

HX711 scale(3, 2);

int rbutton = 9; // this button will be used to reset the scale to 0.
float weight;
float calibration_factor = 419640; // for me this vlaue works just perfect 419640  -199825
float weightg;

void setup() 
{
  Serial.begin(9600);
  pinMode(rbutton, INPUT_PULLUP); 
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading

 
  lcd.init();
  lcd.begin(16,2);
  lcd.backlight(); // turns on the backlight
  lcd.setCursor(6,0);
  lcd.print("DIY");
  lcd.setCursor(1,1);
  lcd.print("Weighing Scale");
  delay(3000);
  lcd.clear();

  
}

void loop() 

{
  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  weight = scale.get_units(5);
  weightg = weight*1000;
//
  lcd.setCursor(0, 0);
//  lcd.print("Measured Weight");
//  lcd.setCursor(0, 1);
  lcd.print(weight);
  lcd.print(" KG ");
//  lcd.setCursor(16, 1);
  lcd.setCursor(0, 1);
  lcd.print(weightg,0);
  lcd.print(" G");
  delay(3000);
  lcd.clear();
  
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" KG");
  Serial.println();
  

  if ( digitalRead(rbutton) == LOW)
{
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
}

}
