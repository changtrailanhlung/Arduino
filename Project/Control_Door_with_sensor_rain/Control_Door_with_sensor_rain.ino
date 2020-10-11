  #include <Servo.h>
int rainSensor = 6;   // Khai báo chân A6 kết nối cảm biến mưa.
  #define servoPin  9     // Khai báo chân sô 9 kết nối động cơ servo.

Servo myservo; 
  
void setup() {
  // put your setup code here, to run once:
    pinMode(rainSensor,INPUT);// Đặt chân cảm biến mưa là INPUT, vì tín hiệu sẽ được truyền đến cho Arduino
  Serial.begin(9600);// Khởi động Serial ở baudrate 9600
  Serial.println("Da khoi dong xong");
   myservo.attach(servoPin); // Cấu hình đối tượng servo với chân số 9.

}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(rainSensor);//Đọc tín hiệu cảm biến mưa
  if (value == HIGH) { // Cảm biến đang không mưa
     delay(1000);  
    Serial.println("Dang khong mua");
     myservo.write(180); 
  } else {
      delay(1000);  
    Serial.println("Dang mua");
    myservo.write(0); 
  }
//  delay(200); // Đợi 1 tí cho lần kiểm tra tiếp theo. Bạn hãy tham khảo bài "Viết chương trình không dùng làm delay" trên Arduino.VN để kết hợp đoạn code này và cả chương trình của bạn
}
