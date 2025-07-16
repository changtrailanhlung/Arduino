//PM1.0 PM2.5 PM10




#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>


// You can use any (4 or) 5 pins 
#define sclk 2
#define mosi 3
#define dc   4
#define cs  5
#define rst  6

// Color definitions
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

// Option 1: use any pins but a little slower
Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, mosi, sclk, rst);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
//Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);




#define HEADER_H  0x42
#define HEADER_L  0x4D
#define FRAME_LENGTH  0x14


unsigned char cur_rx_data;
unsigned char pre_rx_data;
unsigned char main_status;
unsigned char recv_buff[256];
unsigned char recv_buff_index;
unsigned char incomingByte;

unsigned short check_sum;
int i;

int pm1_0_cf_1;
int pm2_5_cf_1;
int pm10_cf_1;

int pm1_0;
int pm2_5;
int pm10;





void setup() {
  Serial.begin(9600);
  
  check_sum = 0;
  for(i=0;i< (sizeof(recv_buff)); i++)
  {
    recv_buff[i] = 0x00;
  }
  recv_buff_index = 0x00;
  cur_rx_data = 0;
  pre_rx_data = 0;
  main_status = 0;


   tft.begin();

   tft.fillRect(0, 0, 128, 128, BLACK);
     
    tft.fillScreen(BLACK);
    tft.setCursor(46, 50);
    tft.setTextColor(BLUE);  
    tft.setTextSize(1);
    tft.println("Smart");
    tft.setCursor(30, 70);
    tft.setTextColor(BLUE);  
    tft.setTextSize(1.8);
    tft.println("Prototyping");
 }

//int n = 0;
char temp_buff[64];
void loop() {

  if (Serial.available() > 0) 
  {
    // read the incoming byte:
     cur_rx_data = Serial.read();
     //Serial.print("recv: "); 
     //Serial.println(  recv_buff[recv_buff_index-1], HEX);

     switch(main_status)
     {
        case 0:
         if( cur_rx_data == HEADER_L )
         {
            if( pre_rx_data ==  HEADER_H )
            {
                 main_status++;
                 
                 check_sum += pre_rx_data;
                 check_sum += cur_rx_data;
                 
                 cur_rx_data = 0;
                 pre_rx_data = 0;

                 
            }
         }else
         {
            pre_rx_data = cur_rx_data;
         }
        break;
        case 1:
        if( cur_rx_data == FRAME_LENGTH )
         {
            if( pre_rx_data ==  0x00 )
            {
                 main_status++;
                 
                 check_sum += pre_rx_data;
                 check_sum += cur_rx_data;
                 
                 cur_rx_data = 0;
                 pre_rx_data = 0;
            }
         }else
         {
            pre_rx_data = cur_rx_data;
         }
        break;
        case 2:
        recv_buff[recv_buff_index++] = cur_rx_data;
        check_sum += cur_rx_data;
        if( recv_buff_index == 18)
        {
             main_status++;
          
             cur_rx_data = 0;
             pre_rx_data = 0;
        }
        break;
        case 3:
        recv_buff[recv_buff_index++] = cur_rx_data;
        if( recv_buff_index == 20)
        { 
            if( check_sum == ( (recv_buff[18]<<8) + recv_buff[19]) )
            {
                recv_buff_index = 0;  
                pm1_0_cf_1 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm2_5_cf_1 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm10_cf_1 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                
                pm1_0 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm2_5 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm10 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
Serial.print(pm1_0_cf_1,HEX);Serial.print("\r\n");
Serial.print(pm2_5_cf_1,HEX);Serial.print("\r\n");
Serial.print(pm10_cf_1,HEX);Serial.print("\r\n");
Serial.print(pm1_0,HEX);Serial.print("\r\n");
Serial.print(pm2_5,HEX);Serial.print("\r\n");
Serial.print(pm10,HEX);Serial.print("\r\n");

    memset(temp_buff,0x00,sizeof(temp_buff));
    sprintf(temp_buff,"pm1.0@cf=1: %d ug/m3",pm1_0_cf_1);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 16);
    //tft.setTextColor(YELLOW);  
    tft.setTextSize(1);
    tft.println(temp_buff);

    memset(temp_buff,0x00,sizeof(temp_buff));
    sprintf(temp_buff,"pm2.5@cf=1: %d ug/m3",pm2_5_cf_1);
    tft.setCursor(0, 32);
    //tft.setTextColor(YELLOW);  
    //tft.setTextSize(2);
    tft.println(temp_buff);

    memset(temp_buff,0x00,sizeof(temp_buff));
    sprintf(temp_buff,"pm10@cf=1: %d ug/m3",pm10_cf_1);
    tft.setCursor(0, 48);
    //tft.setTextColor(YELLOW);  
    //tft.setTextSize(2);
    tft.println(temp_buff);

    memset(temp_buff,0x00,sizeof(temp_buff));
    sprintf(temp_buff,"pm1.0: %d ug/m3",pm1_0);
    tft.setCursor(0, 64);
    //tft.setTextColor(YELLOW);  
    //tft.setTextSize(2);
    tft.println(temp_buff);

    memset(temp_buff,0x00,sizeof(temp_buff));
    sprintf(temp_buff,"pm2.5: %d ug/m3",pm2_5);
    tft.setCursor(0, 80);
    //tft.setTextColor(YELLOW);  
    //tft.setTextSize(2);
    tft.println(temp_buff);

    memset(temp_buff,0x00,sizeof(temp_buff));
    sprintf(temp_buff,"pm10: %d ug/m3",pm10);
    tft.setCursor(0, 96);
    //tft.setTextColor(YELLOW);  
    //tft.setTextSize(2);
    tft.println(temp_buff);
                   
            }
            main_status = 0;    
            cur_rx_data = 0;
            pre_rx_data = 0; 
            check_sum = 0;
            for(i=0;i< (sizeof(recv_buff)); i++)
            {
              recv_buff[i] = 0x00;
            }
            recv_buff_index = 0x00;  
        }
        break;
        case 4:
        break;
        case 5:
        break;
        case 6:
        break;
        case 7:
        break;
        case 8:
        break;
     }
 
  }
  


}
