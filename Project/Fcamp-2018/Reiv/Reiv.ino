    #include <SPI.h>
    #include "RF24.h"
    #include <AFMotor.h>
    #include <Wire.h> 
    #include <LiquidCrystal_I2C.h>

    const uint64_t pipe = 0xE8E8F0F0E9BB; // địa chỉ phát
    RF24 radio(9,10);//thay 10 thành 53 với mega
    byte msg[3];

    
    AF_DCMotor DC3(3); // create motor #2
    AF_DCMotor DC4(4); // create motor #4

   
    void setup(){
      DC3.setSpeed(250);
      DC4.setSpeed(250);
      Serial.begin(9600);
      radio.begin();                     
      radio.setAutoAck(1);              
      radio.setDataRate(RF24_1MBPS);    // Tốc độ dữ liệu
      radio.setChannel(10);               // Đặt kênh
      radio.openReadingPipe(1,pipe);     
      radio.startListening();   

     
    }
     
    void loop(){
      
      if (radio.available()){
        while (radio.available()){
          radio.read(&msg, sizeof(msg));
          Serial.println(msg[0]);
        
          if(msg[0] == 2) {
            DC3.run(FORWARD);      // turn it on going forward
            DC4.run(FORWARD);
          }
          else if(msg[0] == 3) {
            DC3.run(BACKWARD);      // turn it on going forward
            DC4.run(BACKWARD);
          }
          else if(msg[0] == 4) {
            DC3.run(FORWARD);           
          }
          else if(msg[0] == 5) {
            DC4.run(FORWARD);
          }
          else if(msg[0] == 1) {
            digitalWrite(2,HIGH);
          }
          else {
            DC3.run(RELEASE);      // turn it on going forward
            DC4.run(RELEASE);
          }
        }
        
      }
    }


  
