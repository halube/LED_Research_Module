#include <OneWire.h>

#define ONE_WIRE_PIN D4
#define NUMBER_OF_SENSORS 2

OneWire ds(ONE_WIRE_PIN);  // 4.7K resistor is necessary

byte addresses[NUMBER_OF_SENSORS * 8];

void setupTemperature()
{
  #ifdef DEBUG
  Serial.println("Searching for Temperature Sensors...");
  #endif

  for(int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
      byte addr[8];
      if(ds.search(addr))
      {
        #ifdef DEBUG
        Serial.print("ROM =");
        #endif
        for(int j = 0; j < 8; j++) {
          #ifdef DEBUG
          Serial.write(' ');
          Serial.print(addr[j], HEX);
          #endif
          if (OneWire::crc8(addr, 7) != addr[7]) {
            #ifdef DEBUG
            Serial.println("CRC is not valid!");
            #endif
            return;
          }
          addresses[i*8 + j] = addr[j];
        }

        //Set 11-bit Resolution
        ds.reset(); // rest 1-Wire
        ds.select(addr); // select DS18B20
        
        ds.write(0x4E); // write on scratchPad
        ds.write(0x00); // User byte 0 - Unused
        ds.write(0x00); // User byte 1 - Unused
        ds.write(2 << 5); // set up en 11 bits //0 = 9 bit, 1 = 10 bit....
        
        ds.reset(); // reset 1-Wire
      }
      #ifdef DEBUG
      Serial.println();
      #endif
  }

  #ifdef DEBUG
  Serial.println("No more addresses.");
  Serial.println();
  #endif
  
  ds.reset_search();
  delay(250);

  //getTemperatureAsJSON();
  return;

}

String getTemperatureAsJSON()
{
  #ifdef DEBUG
  Serial.println("Temperature 1:");
#endif
  String temp = "{\"Temperature1\":";  
  temp += "" + String(getTemperatureByIndex(0)) + ",";
  #ifdef DEBUG
  Serial.println("Temperature 2:");
#endif
  temp += "\"Temperature2\":";
  temp += "" + String(getTemperatureByIndex(1)) + ",";  
  #ifdef DEBUG
  Serial.println("Temperature 3:");
#endif
  temp += "\"Temperature3\":";
  temp += "" + String(getTemperatureByIndex(2)) + ",";
  #ifdef DEBUG
  Serial.println("Temperature 4:");
#endif
  temp += "\"Temperature4\":";
  temp += "" + String(getTemperatureByIndex(3)) + "}";
  return temp;

}

float getTemperatureByIndex(int index)
{
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius = 0;

  for(int i = 0; i < 8;i++)
  {
      addr[i]= addresses[index * 8 + i];
  }


  switch (addr[0]) {
    case 0x10:
      #ifdef DEBUGEX
      Serial.println("  Chip = DS18S20");  // or old DS1820
      #endif
      type_s = 1;
      break;
    case 0x28:
    #ifdef DEBUGEX
      Serial.println("  Chip = DS18B20");
      #endif
      type_s = 0;
      break;
    case 0x22:
    #ifdef DEBUGEX
      Serial.println("  Chip = DS1822");
      #endif
      type_s = 0;
      break;
    default:
    #ifdef DEBUGEX
      Serial.println("Device is not a DS18x20 family device.");
      #endif
      return 0;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);        // start conversion, use ds.write(0x44,1) with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  #ifdef DEBUGEX
  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  #endif

  for (int i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    #ifdef DEBUGEX
    Serial.print(data[i], HEX);
    Serial.print(" ");
    #endif
  }

  #ifdef DEBUGEX
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  #endif
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    #ifdef DEBUGEX
    Serial.print(" CFG=");
    Serial.print(cfg, HEX);
    Serial.println();
    #endif
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  #ifdef DEBUG
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.println(" Celsius");

  #endif
  return celsius;
}

 
