#include <ArduinoJson.h>

void parseJSON(String pjson)
{
#ifdef DEBUG
    Serial.println(' ');
    Serial.println("Parsing JSON...");
#endif
    const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(6) + 3 * JSON_OBJECT_SIZE(1) + 6 * JSON_OBJECT_SIZE(2) + 3 * JSON_OBJECT_SIZE(7) + 560;
    DynamicJsonBuffer jsonBuffer(bufferSize);

    const char* json = pjson.c_str();

    JsonObject& root = jsonBuffer.parseObject(json);

    JsonObject& Program = root["Program"];
    const char* Program_DeviceName = Program["DeviceName"]; // "HBLUZ_1"
    deviceName = Program_DeviceName;
    const char* Program_ProgramName = Program["ProgramName"]; // "Phalaenopsis"
    programName = Program_ProgramName;
#ifdef DEBUG
    Serial.println("Device Name: " + deviceName);
    Serial.println("Program Name: " + programName);
#endif
    const char* Program_SyncTime = Program["SyncTime"]; // "00:43:31"
    const char* Program_SyncDate = Program["SyncDate"]; // "2017-08-06"
    SyncDateTime = DateTimeStringToTimeT(Program_SyncDate, Program_SyncTime);
#ifdef DEBUG
    Serial.print("Sync Date Time: ");
    printDateTime(SyncDateTime);
    Serial.print('\n');
    Serial.println(SyncDateTime);
#endif
    const char* Program_ProgramDateStart = Program["ProgramDateStart"]; // "2017-08-03"
    time_t ProgramDateStart = DateStringToTimeT(Program_ProgramDateStart);
#ifdef DEBUG
    Serial.print("Program Date Start: ");
    printDate(ProgramDateStart);
    Serial.print('\n');
    Serial.println(ProgramDateStart);
#endif
    pulseFrequency = Program["PulseFrequency"];
    setPulseFrequency(pulseFrequency);
    #ifdef DEBUG
    Serial.print("Pulse Frequency: ");
    Serial.println(pulseFrequency);
    #endif

    lightsettingCount = Program["Lightsettings"]["Lightsetting"].size();

    if (lightsettings != NULL) {
        delete[] lightsettings;
    }
    lightsettings = new Lightsetting[lightsettingCount];

    if (ledOnLookup != NULL) {
        delete[] ledOnLookup;
    }
    ledOnLookup = new bool[lightsettingCount];
    for(int i = 0;i<lightsettingCount;i++)
    {   ledOnLookup[i]=0;}

    

    for (int i = 0; i < lightsettingCount; i++) {

        JsonObject& Program_Lightsettings_Lightsetting = Program["Lightsettings"]["Lightsetting"][i];
        const char* Program_Lightsettings_Lightsetting_Description = Program_Lightsettings_Lightsetting["Description"]; // "Morgens Blau"

        int Program_Lightsettings_Lightsetting_RelativeStart = Program_Lightsettings_Lightsetting["RelativeStart"]; // 0
        lightsettings[i].StartDate = ProgramDateStart + SECS_PER_DAY * Program_Lightsettings_Lightsetting_RelativeStart;

        int Program_Lightsettings_Lightsetting_Duration = Program_Lightsettings_Lightsetting["Duration"]; // 1
        lightsettings[i].EndDate = lightsettings[i].StartDate + SECS_PER_DAY * Program_Lightsettings_Lightsetting_Duration;


        const char* Program_Lightsettings_Lightsetting_TimeStart = Program_Lightsettings_Lightsetting["TimeStart"]; // "01:00"
        lightsettings[i].StartTime = TimeStringToTimeT(Program_Lightsettings_Lightsetting_TimeStart);
        
        const char* Program_Lightsettings_Lightsetting_TimeEnd = Program_Lightsettings_Lightsetting["TimeEnd"]; // "02:00"
        lightsettings[i].EndTime = TimeStringToTimeT(Program_Lightsettings_Lightsetting_TimeEnd);

        if((lightsettings[i].EndDate + lightsettings[i].EndTime) > apocalypse)
        {
            apocalypse = lightsettings[i].EndDate + lightsettings[i].EndTime;
        }

        int Program_Lightsettings_Lightsetting_PWMPointA = Program_Lightsettings_Lightsetting["PWMPointA"]; // 50
        lightsettings[i].PWMPointA = Program_Lightsettings_Lightsetting_PWMPointA;

        int Program_Lightsettings_Lightsetting_PWMPointB = Program_Lightsettings_Lightsetting["PWMPointB"]; // 50
        lightsettings[i].PWMPointB = Program_Lightsettings_Lightsetting_PWMPointB;

        int Program_Lightsettings_Lightsetting_ChannelID = Program_Lightsettings_Lightsetting["ChannelID"]; // 1
        lightsettings[i].ChannelID = (LEDChannels)Program_Lightsettings_Lightsetting_ChannelID;

        tempUpperThreshold = Program["WaterCooling"]["UpperThreshold"];
        tempLowerThreshold = Program["WaterCooling"]["LowerThreshold"];
        
#ifdef DEBUG
        Serial.println(' ');
        Serial.print("Lightsetting Description: ");
        Serial.println(Program_Lightsettings_Lightsetting_Description);
        Serial.print("Date Start: ");
        printDate(lightsettings[i].StartDate);
        Serial.print(' (');
        Serial.print(lightsettings[i].StartDate);
        Serial.println(')');
        Serial.print("Date Start: ");
        printDate(lightsettings[i].EndDate);
        Serial.print(' (');
        Serial.print(lightsettings[i].EndDate);
        Serial.println(')');
        Serial.print("Time Start: ");
        printTime(lightsettings[i].StartTime);
        Serial.print(' (');
        Serial.print(lightsettings[i].StartTime);
        Serial.println(')');
        Serial.print("Time End: ");
        printTime(lightsettings[i].EndTime);
        Serial.print(' (');
        Serial.print(lightsettings[i].EndTime);
        Serial.println(')');
        Serial.print("Channel ID: ");
        Serial.println(lightsettings[i].ChannelID);
        Serial.print("PWMPoints: A:");
                Serial.print(lightsettings[i].PWMPointA);
                Serial.print(" B:");
                Serial.println(lightsettings[i].PWMPointB);     
       
#endif
    }

    #ifdef DEBUG
    Serial.print("Apocalypse: ");
    Serial.println(apocalypse);
    Serial.println(now());
    #endif

    
    #ifdef DEBUG
    Serial.println("Water Cooling: ");
    Serial.print("Upper Treshold: ");
    Serial.println(tempUpperThreshold);
    Serial.print("Lower Treshold: ");
    Serial.println(tempLowerThreshold);
    #endif

    if(now() < apocalypse)
    {
        Serial.print("Ruuning: ");
        
        currentStatus = ClientStatus::RUNNING;
    }
    else
    {
        currentStatus = ClientStatus::READY;
    }
    
}
