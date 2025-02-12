#include <DS3232RTC.h>

void setupRTC()
{
    setSyncProvider(RTC.get);

#ifdef DEBUG
    if (timeStatus() != timeSet) {
        Serial.println("Unable to sync with the RTC");
    }
    else {
        Serial.println("RTC has set the system time");
    }
#endif
}


void loopRTC()
{
    #ifdef DEBUG
    digitalClockDisplay();
#endif

}

void setRTCTime(time_t syncDateTime)
{
    RTC.set(syncDateTime);
    setTime(syncDateTime);

#ifdef DEBUG
    Serial.print("Syncing RTC: ");
    printDateTime(syncDateTime);
    Serial.print("\n");
#endif
}

#ifdef DEBUG
//print date and time to Serial
void printDateTime(time_t t)
{
    printDate(t);
    Serial.print(' ');
    printTime(t);
}

//print time to Serial
void printTime(time_t t)
{
    printI00(hour(t), ':');
    printI00(minute(t), ':');
    printI00(second(t), ' ');
}

//print date to Serial
void printDate(time_t t)
{
    printI00(day(t), 0);
    Serial.print(monthShortStr(month(t)));
    Serial.print(year(t));
}

//Print an integer in "00" format (with leading zero),
//followed by a delimiter character to Serial.
//Input value assumed to be between 0 and 99.
void printI00(int val, char delim)
{
    if (val < 10)
        Serial.print('0');
    Serial.print(val);
    if (delim > 0)
        Serial.print(delim);
    return;
}

void digitalClockDisplay()
{
    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(' ');
    Serial.print(day());
    Serial.print(' ');
    Serial.print(month());
    Serial.print(' ');
    Serial.print(year());
    Serial.println();
}

void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(':');
    if (digits < 10)
        Serial.print('0');
    Serial.print(digits);
}
#endif
