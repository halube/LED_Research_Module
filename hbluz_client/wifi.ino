#include <ESP8266WiFi.h>

const char* AP_SSID = "5D-AP";
const char* AP_PASSWORD = "Nomistake24x7";
bool isConnectedToAP = false;

void setupWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.hostname(deviceName);
    WiFi.begin(AP_SSID, AP_PASSWORD);
}

void loopWifi()
{

  if(WiFi.status() == WL_CONNECTED)
  {
    if(isConnectedToAP == false)
    {
    #ifdef DEBUG
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(AP_SSID);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println(WiFi.softAPIP());
    #endif
    isConnectedToAP = true;
    }
  }
  else
  {
    isConnectedToAP == false;
    #ifdef DEBUG
        Serial.println("Unable to connect to AP.");
#endif
}


    

}
