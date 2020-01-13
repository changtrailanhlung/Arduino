    #include <SPI.h>
    #include "RF24.h"
    #include <AxisJoystick.h>

    #define SW_PIN  8
    #define VRX_PIN A1
    #define VRY_PIN A2
    AxisJoystick* joystick;
    
    const uint64_t pipe = 0xE8E8F0F0E9BB; // địa chỉ để phát
    RF24 radio(9,10); //thay 10 thành 53 với mega
    byte msg[3];

    
     
    void setup(){ 
      //============================================================Module NRF24
      joystick = new AxisJoystick(SW_PIN, VRX_PIN, VRY_PIN);
      Serial.begin(9600);
      radio.begin();                     
      radio.setAutoAck(1);               
      radio.setRetries(1,1);             
      radio.setDataRate(RF24_1MBPS);    // Tốc độ truyền
      radio.setPALevel(RF24_PA_MAX);      // Dung lượng tối đa
      radio.setChannel(10);               // Đặt kênh
      radio.openWritingPipe(pipe);        // mở kênh
      
    }
     
    void loop(){
        const AxisJoystick::Move move = joystick->multipleRead();
        
        if (move == AxisJoystick::Move::PRESS) {
              msg[0] = 1;
              radio.write(&msg, sizeof(msg));
        } else if (move == AxisJoystick::Move::UP) {
              msg[0] = 2;
              radio.write(&msg, sizeof(msg));
        } else if (move == AxisJoystick::Move::DOWN) {
              msg[0] = 3;
              radio.write(&msg, sizeof(msg));
        } else if (move == AxisJoystick::Move::LEFT) {
              msg[0] = 4;
              radio.write(&msg, sizeof(msg));
        } else if (move == AxisJoystick::Move::RIGHT) {
              msg[0] = 5;
              radio.write(&msg, sizeof(msg));
        } else {
              msg[0] = 0;
              radio.write(&msg, sizeof(msg));
        }
        Serial.println(msg[0]);
      }
      
