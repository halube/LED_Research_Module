#include <TimeLib.h>

enum CapabilityType
{
    LED,
    TemperatureSensor
};

enum ProgramMode
{
    Calendar,
    Manual
};

enum LEDChannels
{
    UVB,
    ROYAL_BLUE,
    TRUE_GREEN,
    HYPER_RED,
    FAR_RED,
    STREET_WHITE
};

struct Lightsetting
{
    time_t StartDate;
    time_t EndDate;
    time_t StartTime;
    time_t EndTime;
    uint16_t PWMPointA;
    uint16_t PWMPointB;
    LEDChannels ChannelID;
};



float tempUpperThreshold = 0.0f;
float tempLowerThreshold = 0.0f;

Lightsetting* lightsettings = NULL; 
int lightsettingCount = 0;
int apocalypse = 0;

#ifdef Annina_Jan_Modul
const uint8_t ledPinLookup[6] = {0,2,5,3,1,4}; //<key, pair> where key = enum LEDChannels
#endif

#ifdef FIVE_D
const uint8_t ledPinLookup[6] = {0,1,2,3,4,5}; //<key, pair> where key = enum LEDChannels
#endif

#ifdef FIVE_D_PROTOTYPE
const uint8_t ledPinLookup[6] = {9,11,14,15,12,13};
#endif
bool* ledOnLookup;

String deviceName = "Cooling_I";
String programName = "Cooling";
time_t SyncDateTime = 0;

time_t DateStringToTimeT(const char* dateString) //"2017-08-06"
{
    tmElements_t ProgramDateStart;
    ProgramDateStart.Hour = ProgramDateStart.Minute = ProgramDateStart.Second = 0;
    ProgramDateStart.Year = atoi(dateString)- 1970;
    ProgramDateStart.Month = atoi(dateString + 5);
    ProgramDateStart.Day = atoi(dateString + 8);

    time_t t;
    t = makeTime(ProgramDateStart);
    return t;
}


time_t TimeStringToTimeT(const char* timeString) //"00:43:31"
{
    tmElements_t ProgramDateStart;
    ProgramDateStart.Hour = atoi(timeString);
    ProgramDateStart.Minute = atoi(timeString + 3);
    ProgramDateStart.Second = atoi(timeString + 6);
    ProgramDateStart.Year = 0;
    ProgramDateStart.Month = ProgramDateStart.Day = 1;

    time_t t;
    t = makeTime(ProgramDateStart);
    return t;
}


time_t DateTimeStringToTimeT(const char* dateString, const char* timeString) //"2017-08-06" "00:43:31"
{
    tmElements_t ProgramDateStart;
    ProgramDateStart.Hour = atoi(timeString);
    ProgramDateStart.Minute = atoi(timeString + 3);
    ProgramDateStart.Second = atoi(timeString + 6);
    ProgramDateStart.Year = atoi(dateString)- 1970;
    ProgramDateStart.Month = atoi(dateString + 5);
    ProgramDateStart.Day = atoi(dateString + 8);

    time_t t;
    t = makeTime(ProgramDateStart);
    return t;
}
