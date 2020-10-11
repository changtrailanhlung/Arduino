  #include <Servo.h>
int rainSensor = 6;   
  #define servoPin  9     

Servo myservo; 
  
void setup() {
  // put your setup code here, to run once:
  pinMode(rainSensor,INPUT);
  Serial.begin(9600);
  Serial.println("Da khoi dong xong");
  myservo.attach(servoPin); 
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(rainSensor);
  if (value == HIGH) { 
     delay(1000);  
    Serial.println("Dang khong mua");
     myservo.write(180); 
  } else {
      delay(1000);  
    Serial.println("Dang mua");
    myservo.write(0); 
  }
//  delay(200); 
}
