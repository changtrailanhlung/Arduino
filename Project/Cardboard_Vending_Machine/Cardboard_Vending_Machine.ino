#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


// Keypad Pins
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {30, 32, 34, 36};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// RFID Pins
#define SS_PIN 53
#define RST_PIN 49
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Servo Pins
const int servoPin = 23;
Servo doorServo;

// Motor Pins
const int motorPinA1 = 42;
const int motorPinA2 = 38;
const int motorPinB3 = 40;
const int motorPinB4 = 44;

// LED Pins
const int bluePin = 6;
const int greenPin = 5;

// Global Variables
String selectedCode = "";
float selectedPrice = 0.0;
bool isCardPresent = false;
bool isCardCorrect = false;
bool isMotorRunning = false;

// Constants
const int numItems = 4;
struct Item {
  String code;
  float price;
};
Item items[numItems] = {
  {"A1", 1.50},
  {"A2", 2.80},
  {"B3", 1.95},
  {"B4", 2.50}
};

// Servo Positions
const int lockedPosition = 20;
const int unlockedPosition = 180;

// Motor Run Time (in milliseconds)
const unsigned long motorRunTime = 5000; // 5 seconds

// Timing Constants
const unsigned long cardPresentTimeout = 20000; // 20 seconds
unsigned long cardPresentStartTime = 0;

void setup() {
  Serial.begin(115200);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Please Select");
  lcd.setCursor(0, 1);
  lcd.print("Item: ");
  Serial.println("Please Select");
  Serial.println("Item: ");

  // Initialize Servo
  doorServo.attach(servoPin);
  doorServo.write(lockedPosition);

  // Initialize Motor Pins
  pinMode(motorPinA1, OUTPUT);
  pinMode(motorPinA2, OUTPUT);
  pinMode(motorPinB3, OUTPUT);
  pinMode(motorPinB4, OUTPUT);

  // Initialize LED Pins
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  // Initialize RFID
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  // Reset the machine
  resetMachine();
}

void loop() {
  // Handle keypad input
  char customKey = customKeypad.getKey();

  if (customKey) {
    if (customKey == '#') {
      selectedCode += customKey;
    } else if (customKey == '*') {
      resetMachine();
      lcd.clear();
      lcd.print("Cancelled");
      delay(2000);
      lcd.clear();
      lcd.print("Please Select");
      lcd.setCursor(0, 1);
      lcd.print("Item: ");
      return;
    } else {
      selectedCode += customKey;
      lcd.setCursor(7, 1);
      lcd.print(selectedCode);

      // Check if item selection is complete
      if (selectedCode.length() == 2) {
        selectedPrice = getItemPrice(selectedCode);
        if (selectedPrice != 0.0) {
          lcd.clear();
          lcd.print("Price: $");
          lcd.print(selectedPrice);
          lcd.setCursor(0, 1);
          lcd.print("Present Card...");
        } else {
          lcd.clear();
          lcd.print("Invalid Item");
          delay(2000);
          lcd.clear();
          lcd.print("Please Select");
          lcd.setCursor(0, 1);
          lcd.print("Item: ");
          selectedCode = "";
        }
      }
    }
  }

  // Check if a card is present
  if (selectedPrice != 0.0) {
    isCardPresent = checkCardPresent();
  }

  // If a card is present and it hasn't been verified yet, verify the card
  if (isCardPresent && !isCardCorrect) {
    isCardCorrect = verifyCard();
  }

  // If the card is verified and the item is selected, process the transaction
  if (isCardCorrect && selectedPrice != 0.0) {
    processTransaction();
  }
  // Check if the card present timeout has exceeded
  if (isCardPresent && !isCardCorrect && cardPresentStartTime > 0 && millis() - cardPresentStartTime > cardPresentTimeout) {
    lcd.clear();
    lcd.print("Transaction");
    lcd.setCursor(0, 1);
    lcd.print("Failed");
    digitalWrite(bluePin, LOW); // Turn off blue LED
    delay(2000); // Wait for 2 seconds
    resetMachine();
    lcd.clear();
    lcd.print("Please Select");
    lcd.setCursor(0, 1);
    lcd.print("Item: ");
  }
}


bool checkCardPresent() {
  // Check for a new card
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  // Wait for a card to be removed if it was detected previously
  if (isCardPresent) {
    unsigned long cardRemovedTime = millis();
    while (mfrc522.PICC_IsNewCardPresent()) {
      if (millis() - cardRemovedTime > cardPresentTimeout) {
        lcd.clear();
        lcd.print("Transaction");
        lcd.setCursor(0, 1);
        lcd.print("Failed");
        delay(2000); // Wait for 2 seconds
        resetMachine();
        lcd.clear();
        lcd.print("Please Select");
        lcd.setCursor(0, 1);
        lcd.print("Item: ");
        return false;
      }
    }
  }


  cardPresentStartTime = millis();  // Record the start time when a card is detected


  // Check if card UID matches the correct UID
  byte correctUid[4] = {0x22, 0xAF, 0x47, 0x39}; // Correct UID tag: 22 AF 47 39
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] != correctUid[i]) {
      lcd.clear();
      lcd.print("Unauthorized");
      lcd.setCursor(0, 1);
      lcd.print("Card");
      delay(2000); // Wait for 2 seconds
      resetMachine();
      lcd.clear();
      lcd.print("Please Select");
      lcd.setCursor(0, 1);
      lcd.print("Item: ");
      return false;
    }
  }



  return true;
}


bool verifyCard() {
  // Verify card by entering a PIN
  lcd.clear();
  lcd.print("Enter PIN:");
  String enteredPin = "";
  int pinLength = 4;
while (enteredPin.length() < pinLength) {
  char key = customKeypad.getKey();
  if (key) {
    if (key == '#') {
      if (enteredPin.length() > 0) {
        enteredPin = enteredPin.substring(0, enteredPin.length() - 1);
        lcd.setCursor(enteredPin.length(), 1);
        lcd.print(" ");
      }
    } else if (key == '*') {
      return false;
    } else {
      enteredPin += key;
      lcd.setCursor(enteredPin.length() - 1, 1);
      lcd.print('*');
    }
  }
}




  // Check if entered PIN is correct
  String correctPin = "9447"; // Correct PIN: 1234
  if (enteredPin == correctPin) {
    lcd.clear();
    lcd.print("Card Verified");
    delay(2000); // Wait for 2 seconds
    lcd.clear();
    lcd.print("Price: $");
    lcd.print(selectedPrice);
    lcd.setCursor(0, 1);
    lcd.print("Confirm (#)");
    while (true) {
     char key = customKeypad.getKey();
     if (key == '*') {
       resetMachine();
       break;
      } else if (key == '#') {
        break;
      }
  // Do nothing if no key is pressed, continue waiting
    }
    return true;
  } else {
    lcd.clear();
    lcd.print("Invalid PIN");
    digitalWrite(bluePin, LOW);
    delay(250);
    digitalWrite(bluePin, HIGH);
    delay(250);
    digitalWrite(bluePin, LOW);
    delay(250);
    digitalWrite(bluePin, HIGH);
    delay(250);
    digitalWrite(bluePin, LOW);
    delay(250);
    resetMachine();
    lcd.clear();
    lcd.print("Please Select");
    lcd.setCursor(0, 1);
    lcd.print("Item: ");
    return false;
  }
}

float getItemPrice(String code) {
  for (int i = 0; i < numItems; i++) {
    if (items[i].code == code) {
      return items[i].price;
    }
  }
  return 0.0;
}

void processTransaction() {
  lcd.clear();
  lcd.print("Processing");
  lcd.setCursor(0, 1);
  lcd.print("Payment...");
  delay(500);
  lcd.clear();
  lcd.print("Processing");
  lcd.setCursor(0, 1);
  lcd.print("Payment..");
  delay(500);
  lcd.clear();
  lcd.print("Processing");
  lcd.setCursor(0, 1);
  lcd.print("Payment...");
  delay(500);
  lcd.clear();
  lcd.print("Processing");
  lcd.setCursor(0, 1);
  lcd.print("Payment..");
  delay(500);
  lcd.clear();
  lcd.print("Processing");
  lcd.setCursor(0, 1);
  lcd.print("Payment...");
  // Check if the transaction was successful
  if (isCardCorrect) {
    lcd.clear();
    lcd.print("Transaction");
    lcd.setCursor(0, 1);
    lcd.print("Completed!");
      
    if (selectedCode == "A1") {
      spinMotor(42, 1);
    }   
    else if (selectedCode == "A2") {
      spinMotor(38, 2);
    }
    else if (selectedCode == "B3") {
      spinMotor(40, 1);
    }
    else if (selectedCode == "B4") {
      spinMotor(44, 1);
    }
    digitalWrite(bluePin, LOW); 
    digitalWrite(greenPin, HIGH); 
    unlockDoor();
    lcd.clear();
    lcd.print("Enjoy!");
    delay(8000); // Wait for 8 seconds
    resetMachine();
    digitalWrite(bluePin, HIGH);
    lcd.clear();
    lcd.print("Please Select");
    lcd.setCursor(0, 1);
    lcd.print("Item: ");
  } else {
    // Card verification failed
    lcd.clear();
    lcd.print("Transaction");
    lcd.setCursor(0, 1);
    lcd.print("Failed");
    delay(2000); // Wait for 2 seconds
    resetMachine();
    lcd.clear();
    lcd.print("Please Select");
    lcd.setCursor(0, 1);
    lcd.print("Item: ");
  }
}


void resetMachine() {
  selectedCode = "";
  selectedPrice = 0.0;
  isCardPresent = false;
  isCardCorrect = false;
  isMotorRunning = false;
  cardPresentStartTime = 0;
  digitalWrite(bluePin, HIGH); // Turn off blue LED
  digitalWrite(greenPin, LOW); // Turn off green LED
  doorServo.write(lockedPosition);
  stopMotor();
}


void spinMotor(int motorPin, unsigned long duration) {
  digitalWrite(motorPin, HIGH);
  isMotorRunning = true;
  delay(duration * 1000);
  digitalWrite(motorPin, LOW);
  isMotorRunning = false;
}


void stopMotor() {
  if (isMotorRunning) {
    digitalWrite(motorPinA1, LOW);
    digitalWrite(motorPinA2, LOW);
    digitalWrite(motorPinB3, LOW);
    digitalWrite(motorPinB4, LOW);
    isMotorRunning = false;
  }
}


void unlockDoor() {
  doorServo.write(unlockedPosition);
  digitalWrite(greenPin, HIGH); // Turn on green LED
}
