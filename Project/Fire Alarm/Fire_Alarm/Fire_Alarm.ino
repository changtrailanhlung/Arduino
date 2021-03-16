#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensorPin = A0;// chân analog kết nối tới cảm biến LM35
int gasPin = A1;
int Relay = 9;


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
 
void setup() {
  Serial.begin(9600);
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.createChar(1, degree);
  lcd.createChar(2, thermometro);

  pinMode(Relay, OUTPUT);

  wellcome();
  lcd.clear();
  delay(2000);
}
 
void loop() {
 int reading = analogRead(sensorPin); 
 
 float voltage = reading * 5.0 / 1023.0;
 float temp = voltage * 100.0;

 Serial.println(temp);
 Serial.println(analogRead(gasPin));

 lcd.clear();
 lcd.setCursor(0,1);
 lcd.print("Gas: ");
 lcd.setCursor(5,1);
 lcd.print(analogRead(gasPin));
 lcd.print("ppm");
 lcd.setCursor(2,0);
 lcd.write(2);
 lcd.print(": ");
 lcd.print(temp);
 lcd.write(1);
 lcd.print("C");

  if ((analogRead(gasPin) > 500) || (temp > 50.00) || ((analogRead(gasPin) > 500) && (temp > 50.00))){
  lcd.clear();
  digitalWrite(Relay, HIGH);
  lcd.setCursor(3,0);
  lcd.print("!!!ALARM!!!");
  lcd.setCursor(0,1);
  lcd.print("----------------");
 }else{
  digitalWrite(Relay, LOW);
 }
 
 delay(1000);
}

void wellcome(){
 lcd.setCursor(2,0);
 lcd.print("WELLCOME TO");
 lcd.setCursor(0,1);
 lcd.print("System FireAlarm");
}
