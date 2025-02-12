#include <EEPROM.h>

const int RESERVED_EEPROM_SIZE = 1024 * 8;

const String jsonFallback = "{\"Program\":{\"DeviceName\":\"Annina_Modul_Reihe_2\",\"ProgramName\":\"Unknown\",\"SyncTime\":\"00:00:00\",\"SyncDate\":\"2018-01-01\",\"ProgramDateStart\":\"2018-01-01\",\"Lightsettings\":{\"Lightsetting\":[{\"Description\":\"Unknown\",\"RelativeStart\":0,\"Duration\":0,\"TimeStart\":\"00:00\",\"TimeEnd\":\"00:00\",\"Intensity\":0,\"ChannelID\":1,\"PWMPointA\":0,\"PWMPointB\":0}]},\"PulseFrequency\":40,\"WaterCooling\":{\"UpperThreshold\":20.1,\"LowerThreshold\":19.9}}}";

void setupEEPROM()
{


    #ifdef DEBUG
    Serial.println("Setting up EEPROM...");
    #endif 

    EEPROM.begin(RESERVED_EEPROM_SIZE);

    #ifdef DEBUG
    Serial.println("Checking Checksum...");
    #endif 

    if(!isChecksumValid())
    {
        #ifdef DEBUG
        Serial.println("Checksum invalid!");
        Serial.println("Resetting to Fallback...");
        #endif
    resetEEPROMToFallback();   
           }
   
    #ifdef DEBUG
    Serial.println("Checksum valid.");
    #endif 

}


void resetEEPROMToFallback()
{
    #ifdef DEBUGEX
    Serial.println("Zeroing EEPROM...");
    #endif

    for (int i = 0; i < RESERVED_EEPROM_SIZE; i++) {
        EEPROM.write(i, 0);
    }

    #ifdef DEBUGEX
    Serial.println("Writing JSON Fallback into EEPROM...");
    #endif

    EEPROM.put(0, jsonFallback.length());
    
    byte lowByte = EEPROM.read(0);
    byte highByte = EEPROM.read(1);

    byte xor1 = lowByte ^ highByte;
    byte xor2 = xor1 ^ highByte;

    EEPROM.put(2, xor1);
    EEPROM.put(3, xor2);

        for (int i = 0; i < jsonFallback.length(); i++) {
            EEPROM.write(i+4, jsonFallback[i]);
        }
        EEPROM.commit();

    #ifdef DEBUGEX
    Serial.println("Done writing JSON Fallback into EEPROM.");
    #endif
}

void putProgramIntoMemory(String json)
{

    for (int i = 0; i < RESERVED_EEPROM_SIZE; i++) {
        EEPROM.write(i, 0);
    }

    EEPROM.put(0, json.length());

    byte lowByte = EEPROM.read(0);
    byte highByte = EEPROM.read(1);

    byte xor1 = lowByte ^ highByte;
    byte xor2 = xor1 ^ highByte;

    EEPROM.put(2, xor1);
    EEPROM.put(3, xor2);

    for (int i = 0; i < json.length(); i++) {
        EEPROM.write(i+4, json[i]);
    }

    EEPROM.commit();
}

String getProgramFromMemory()
{

    if(!isChecksumValid())
    {
        resetEEPROMToFallback();
        return getProgramFromMemory();
    }

    unsigned int JSONLength = readJSONLengthFromEEPROM();

    #ifdef DEBUGEX
    Serial.print("JSON Length in EEPROM:");
    Serial.println(JSONLength);
    #endif  

    if (JSONLength == 0) {
        resetEEPROMToFallback();
        return getProgramFromMemory();
    }

    String temp = "";

    for (int i = 0; i < JSONLength; i++) {
        temp += (char)EEPROM.read(i+4);
    }

    return temp;
}

unsigned int readJSONLengthFromEEPROM()
{
    byte lowByte = EEPROM.read(0);
    byte highByte = EEPROM.read(1);

    return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

bool isChecksumValid()
{
    byte lowByte = EEPROM.read(0);
    byte highByte = EEPROM.read(1);

    byte xor1 = EEPROM.read(2);
    byte xor2 = EEPROM.read(3);

    #ifdef DEBUGEX
    Serial.println("Checksum:");
    Serial.print("LowByte: ");
    Serial.println(lowByte);
    Serial.print("HighByte: ");
    Serial.println(highByte);
    Serial.print("Xor1: ");
    Serial.println(xor1);
    Serial.print("Xor2: ");
    Serial.println(xor2);
    #endif  

    if(((lowByte ^ highByte) == xor1) && ((xor1 ^ highByte) == xor2))
    {
        #ifdef DEBUGEX
        Serial.println("Checksum valid.");
        #endif  
        return true;
    }

    #ifdef DEBUGEX
    Serial.println("Checksum invalid.");
    #endif  
    return false;

}
