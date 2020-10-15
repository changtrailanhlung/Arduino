#include <ESP8266HTTPClient.h> 
#include <LiquidCrystal_I2C.h>
#include "json_parser.h"
#include "WifiConnect.h"

#define s2ms(second) (second*1000)
unsigned long long prev_millis(0);

#define country_code "Vietnam"

LiquidCrystal_I2C lcd(0x3F, 20, 4);

int interval = s2ms(60);
unsigned long long PreviousMillis = 0;
unsigned long long CurrentMillis = interval;
bool bFirstKickMillis = false;

extern bool bGotIpFlag;

static String build_url_from_country(String country)
{
  String url = "http://coronavirus-19-api.herokuapp.com/countries/";
  url = url + country;
  return url;
}

void setup(void)
{ 
//  lcd.init();  // initialize the lcd 
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Covid-19 Watch");
  lcd.setCursor(0, 1);
  lcd.print(country_code);
  lcd.setCursor(9, 3);
  lcd.print("By LHN");

  delay(5000);  

  #if defined JSON_DEBUG
    Serial.begin(9600);
  #endif
  
  JSON_LOG("Connecting...");
  
  vConnWifiNetworkViaSdk();
}

void loop()
{
  if(bGotIpFlag) bGotIp();
  
  if(bFirstKickMillis) CurrentMillis = millis();
  
  if (!bGotIpFlag && CurrentMillis - PreviousMillis >= interval) 
  {
    if(!bFirstKickMillis) CurrentMillis = 0;
    
    bFirstKickMillis = true;
    
    PreviousMillis = CurrentMillis;
    
    HTTPClient http; 
    http.begin(build_url_from_country(country_code));
    
    int httpCode = http.GET(); 
  
    if(httpCode > 0) 
    {
      String payload = http.getString();
       
      char* JsonArray = (char *)malloc(payload.length() + 1);
      if (!JsonArray) JSON_LOG("upssss fuck");
      
      payload.toCharArray(JsonArray, payload.length() + 1);
      
      JSON_LOG(JsonArray);
      
      if (json_validate(JsonArray))
      {
        int confirmed = (int)get_json_value(JsonArray, "cases", INT);
        int deaths = (int)get_json_value(JsonArray, "deaths", INT);
        int recovered = (int)get_json_value(JsonArray, "recovered", INT);
        int critical = (int)get_json_value(JsonArray, "critical", INT);
        int todayCases = (int)get_json_value(JsonArray, "todayCases", INT);
        int totalTests = (int)get_json_value(JsonArray, "totalTests", INT);     
      
        JSON_LOG(confirmed);
        JSON_LOG(deaths);
        JSON_LOG(recovered);
        JSON_LOG(critical);
        JSON_LOG(todayCases);
        JSON_LOG(totalTests);
        
        lcd.clear();
        lcd.print("Confirm");
        lcd.setCursor(8,0);
        lcd.print("Dead");
        lcd.setCursor(14,0);
        lcd.print("Cure");
        lcd.setCursor(0,2);
        lcd.print("Special");
        lcd.setCursor(8,2);
        lcd.print("Today");
        lcd.setCursor(14,2);
        lcd.print("Test");
        lcd.setCursor(0, 1);
        lcd.print(confirmed);
        lcd.setCursor(8, 1);
        lcd.print(deaths);
        lcd.setCursor(14, 1);
        lcd.print(recovered);
        lcd.setCursor(0, 3);
        lcd.print(critical);
        lcd.setCursor(8, 3);
        lcd.print(todayCases);
        lcd.setCursor(13, 3);
        lcd.print(totalTests);          
        
      }
      
      free(JsonArray);
    }
    
    http.end(); 
  }
}
