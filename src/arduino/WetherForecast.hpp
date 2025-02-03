
#include <HTTPClient.h>
#include <ArduinoJson.h>
//#include <GSON.h>

struct Wether{
  int TemperatureValue;
  uint8_t Icon;
  String IconPhrase;
  int WindSpeed;
  String TimeMoscow;
};

class WetherForecast{
  public:
    WetherForecast(HTTPClient* http){
      _http = http;
    }

    void get(int offsetHour, Wether* result){
      String buf;
      buf = "http://s1030761.srvape.com:5321/forecast?offsetHours=";
      buf += offsetHour;

      int httpResponseCode = 0;
      int retryCount = 0;
      while(httpResponseCode<=0){
        
        _http->begin(buf);
        httpResponseCode = _http->GET();
        if(httpResponseCode <=0){
          retryCount++;
          Serial.print(retryCount);
          Serial.printf(":Error occurred while sending HTTP REQUEST: %s\n", _http->errorToString(httpResponseCode).c_str());
          _http->end();
        }
      }
    

      String response = _http->getString();        
      Serial.println(response);
      
      JsonDocument doc;
      deserializeJson(doc, response);

      result->TemperatureValue = doc["temperature"];
      result->Icon = doc["weatherIcon"];

      const char* description = doc["description"];
      result->IconPhrase = "";
      result->IconPhrase += description;

      const char* timeMoscow = doc["timeMoscow"];
      result->TimeMoscow = "";
      result->TimeMoscow += timeMoscow;

      result->WindSpeed = doc["windSpeed"];
      _http->end();
    }
  private:
    HTTPClient* _http;
};