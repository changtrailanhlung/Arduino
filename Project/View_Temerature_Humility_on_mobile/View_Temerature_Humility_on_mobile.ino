#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"           
#include <SimpleTimer.h>
#define DHTTYPE DHT11

#define dht_dpin 2
DHT dht(dht_dpin, DHTTYPE); 
SimpleTimer timer;
char auth[] = "ld7BS4I7n0MC9jDY-Ber6kPl-O3xsftG";
char ssid[] = "Home 2.4ghz";
char pass[] = "07531592486";
float t;
float h;

void setup()
{
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    dht.begin();
    timer.setInterval(2000, sendUptime);
}

void sendUptime()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  Serial.println("Humidity and temperature\n\n");
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(t); 
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V5, h);
  
}

void loop()
{
  Blynk.run();
  timer.run();
}
