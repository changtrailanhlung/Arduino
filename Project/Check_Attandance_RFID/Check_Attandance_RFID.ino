#include <MFRC522.h> // for the RFID
#include <SPI.h> // for the RFID and SD card module
#include <SD.h> // for the SD card
#include <DS1302.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// define pins for RFID
#define CS_RFID 10
#define RST_RFID 9
// define select pin for SD card module
#define CS_SD 4

// Create a file to store the data
File myFile;

// Instance of the class for RFID
MFRC522 rfid(CS_RFID, RST_RFID); 

// Variable to hold the tag's UID
String uidString;

// Instance of the class for RTC
//RTC_DS1307 rtc;
DS1302 rtc(6, 5, 3);

// Define check in time (Gio check IN)
//const int checkInHour = 9;
//const int checkInMinute = 5;

//Variable to hold user check in
int userCheckInHour;
int userCheckInMinute;

// Pins for LEDs and buzzer
//const int redLED = 6;
//const int greenLED = 7;
//const int buzzer = 5;

//Setup LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Buzzer
const int buzzerPin = 2;
int switchPosition=0;
int c = 261;
int d = 293;
int e = 329;
int f = 349;
int g = 391;
int a = 440;
int Bb = 466;
int b = 493;
int highc = 523;
int eighth = 500/2;
int quarter = 1000/2;
int half = 2000/2;

void setup() { 
  // Set LEDs and buzzer as outputs
//  pinMode(redLED, OUTPUT);  
//  pinMode(greenLED, OUTPUT);
//  pinMode(buzzer, OUTPUT);
  
  // Init Serial port
  Serial.begin(9600);
  while(!Serial); // for Leonardo/Micro/Zero

  // Init LCD
  lcd.init();
  //Setup LCD
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  
  // Init SPI bus
  SPI.begin(); 
  // Init MFRC522 
  rfid.PCD_Init();

  //Setup buzzer
  pinMode(buzzerPin,OUTPUT);
 
  // Setup for the SD card
  lcd.setCursor(0, 0);
  lcd.print("Setup System");
  lcd.setCursor(0, 1);
  lcd.print("Loading......");
//  happBirthdayBuzzer();
  
//  Serial.print("Initializing SD card...");
//  if(!SD.begin(CS_SD)) {
//    Serial.println("initialization failed!");
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("Check SD Card");
//  lcd.setCursor(4, 1);
//  lcd.print("Again!!!");
//    return;
//  }
//  Serial.println("initialization done.");
  
  delay(5000);
  lcd.clear();

  // Setup for the RTC  
  rtc.halt(false);
  rtc.writeProtect(false);
  // following line sets the RTC to the date & time this sketch was compiled
//  rtc.setDOW(FRIDAY);
//  rtc.setTime(9, 39, 0);     
//  rtc.setDate(20, 12, 2019);
  //(luu y nap lan dau set gia tri vao cho dung, nap tu lan sau thi an di)

  lcd.setCursor(5, 0);
  lcd.print("Welcome");
  delay(500);
  lcd.clear();
}

void loop() {
   // Display time centered on the upper line
  lcd.setCursor(4, 0);
  lcd.print(rtc.getTimeStr());
  
//  // Display abbreviated Day-of-Week in the lower left corner
//  lcd.setCursor(0, 1);
//  lcd.print(rtc.getDOWStr(FORMAT_SHORT));
  
  // Display date in the lower right corner
  lcd.setCursor(3, 1);
  lcd.print(rtc.getDateStr());

  //look for new cards
  if(rfid.PICC_IsNewCardPresent()) {
    readRFID();
    logCard();
//    verifyCheckIn();
  }
  delay(10);
}

void readRFID() {
  rfid.PICC_ReadCardSerial();
  Serial.print(rtc.getDOWStr(FORMAT_SHORT));
  Serial.print(" ");
  Serial.print(rtc.getDateStr());
  Serial.print(" ");
  Serial.print(rtc.getTimeStr());
  Serial.print(" ");
  Serial.print("Tag UID: ");
  uidString = String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + 
    String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]);
  Serial.println(uidString);
  
  delay(100);
}

void logCard() {
  // Enables SD card chip select pin
  digitalWrite(CS_SD,LOW);
  
  // Open file
  myFile=SD.open("DATA.txt", FILE_WRITE);

  // If the file opened ok, write to it
  if (myFile) {
    Serial.println("File opened ok");
    myFile.print(uidString);
    myFile.print(", ");   
    
    // Save time on SD card
//    DateTime now = rtc.now();
//    myFile.print(now.year(), DEC);
//    myFile.print('/');
//    myFile.print(now.month(), DEC);
//    myFile.print('/');
//    myFile.print(now.day(), DEC);
//    myFile.print(',');
//    myFile.print(now.hour(), DEC);
//    myFile.print(':');
//    myFile.println(now.minute(), DEC);
      myFile.print(rtc.getDateStr());
      myFile.print(' -- ');
      myFile.print(rtc.getTimeStr());
    
    // Print time on Serial monitor
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(' ');
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.println(now.minute(), DEC);
    Serial.print(rtc.getDOWStr(FORMAT_SHORT));
    Serial.print(" ");
    Serial.print(rtc.getDateStr());
    Serial.print(" ");
    Serial.print(rtc.getTimeStr());
    Serial.println("sucessfully written on SD card");
    myFile.close();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(uidString);
    lcd.setCursor(2, 1);
    lcd.print("Thank You");
    alarmBuzzer();
    delay(1000);
    lcd.clear();
    
    // Save check in time;
//    userCheckInHour = now.hour();
//    userCheckInMinute = now.minute();
  }
  else {
//    Serial.println("error opening DATA.txt");    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(uidString);
    lcd.setCursor(2, 1);
    lcd.print("Thank You");
//    lcd.setCursor(0, 1);
//    lcd.print("Please Try Again");
    alarmBuzzer();
//    alarmBuzzer();
    delay(1000);  
    lcd.clear();
    
  }
  // Disables SD card chip select pin  
  digitalWrite(CS_SD,HIGH);
}

  //Den bao
//void verifyCheckIn(){
//  if((userCheckInHour < checkInHour)||((userCheckInHour==checkInHour) && (userCheckInMinute <= checkInMinute))){
//    digitalWrite(greenLED, HIGH);
//    delay(2000);
//    digitalWrite(greenLED,LOW);
//    Serial.println("You're welcome!");
//  }
//  else{
//    digitalWrite(redLED, HIGH);
//    delay(2000);
//    digitalWrite(redLED,LOW);
//    Serial.println("You are late...");
//  }
//}

  // Buzzer
void happBirthdayBuzzer(){ //(12s)
  tone(2,c,150*2);
  delay(200*2);
  tone(2,c,50*2);
  delay(50*2);
  tone(2,d,quarter);
  delay(quarter);
  tone(2,c,quarter);
  delay(quarter);
  tone(2,f,quarter);
  delay(quarter);
  tone(2,e,half);
  delay(half);
  tone(2,c,150*2);
  delay(200*2);
  tone(2,c,50*2);
  delay(50*2);
  tone(2,d,quarter);
  delay(quarter);
  tone(2,c,quarter);
  delay(quarter);
  tone(2,g,quarter);
  delay(quarter);
  tone(2,f,half);
  delay(half);
  tone(2,c,150*2);
  delay(200*2);
  tone(2,c,50*2);
  delay(50*2);
  tone(2,highc,quarter);
  delay(quarter);
  tone(2,a,quarter);
  delay(quarter);
  tone(2,f,quarter);
  delay(quarter);
  tone(2,e,quarter);
  delay(quarter);
  tone(2,d,half);
  delay(half);
  tone(2,Bb,150*2);
  delay(200*2);
  tone(2,Bb,50*2);
  delay(50*2);
  tone(2,a,quarter);
  delay(quarter);
  tone(2,f,quarter);
  delay(quarter);
  tone(2,g,quarter);
  delay(quarter);
  tone(2,f,half);
  delay(half);
}

void alarmBuzzer(){
  tone(buzzerPin, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzerPin);     // Stop sound...
  delay(1000);        // ...for 1sec
}
