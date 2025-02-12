void loopLightsettings()
{



    for (int i = 0; i < lightsettingCount; i++) {
        int channelID = (int)lightsettings[i].ChannelID;
        int lightsettingID = i;

        #ifdef DEBUG
        Serial.print("Checking channel ID: ");
        Serial.println(channelID);
#endif

        if (IsItTime(now(), lightsettings[i].StartDate, lightsettings[i].EndDate, lightsettings[i].StartTime, lightsettings[i].EndTime)) {
            if(ledOnLookup[lightsettingID]==0)
            {
                #ifdef DEBUG
                Serial.print("Enabling Channel: ");
                Serial.println(channelID);
                #ifdef DEBUGEX
                Serial.print("LED on Lookup: ");
                Serial.println(ledOnLookup[lightsettingID - 1]);
        Serial.print("PWMPoints: A:");
                Serial.print(lightsettings[i].PWMPointA);
                Serial.print(" B:");
                Serial.println(lightsettings[i].PWMPointB);
                #endif                         
                #endif
                 
                pwm.setPWM(ledPinLookup[channelID], lightsettings[i].PWMPointA, lightsettings[i].PWMPointB);
                ledOnLookup[lightsettingID] = 1; 
            }

        }
        else
        {
            if(ledOnLookup[lightsettingID]==1)
            {
                #ifdef DEBUG
                Serial.print("Disabling Channel: ");
                Serial.println(channelID);
                Serial.print("LED on Lookup: ");
                Serial.println(ledOnLookup[lightsettingID]);  
                #endif
                pwm.setPWM(ledPinLookup[channelID], 0, 4096); //
                ledOnLookup[lightsettingID] = 0;

                #ifdef DEBUGEX
                Serial.println("Apocalypse now?");
                Serial.print("Now: ");
                Serial.println(now());
                Serial.print("Apocalpyse: ");
                Serial.println(apocalypse);
                #endif
                if(now() >= apocalypse)
                {
                    #ifdef DEBUG
                    Serial.println("Apocalypse now.");
                    #endif
                    currentStatus = ClientStatus::FINISHED;
                }
            }

           
        }
    }

}

bool IsItTime(time_t now, time_t startDate, time_t endDate, time_t startTime, time_t endTime)
{
    tmElements_t temp;

    time_t nowDateOnly;
    temp.Year = year(now) - 1970;
    temp.Month = month(now);
    temp.Day = day(now);
    temp.Hour = 0;
    temp.Minute = 0;
    temp.Second = 0;
    nowDateOnly = makeTime(temp);

    #ifdef DEBUGEX
    Serial.print("Now Date Only: ");
    Serial.println(nowDateOnly);
    Serial.print("Start Date: ");
    Serial.println(startDate);
    Serial.print("End Date: ");
    Serial.println(endDate);
#endif

    if (startDate <= nowDateOnly && nowDateOnly <= endDate) {


        temp.Year = year(now) - 1970;
        temp.Month = month(now);
        temp.Day = day(now);

        temp.Hour = hour(startTime);
        temp.Minute = minute(startTime);
        temp.Second = second(startTime);
        startTime = makeTime(temp);

        temp.Hour = hour(endTime);
        temp.Minute = minute(endTime);
        temp.Second = second(endTime);
        endTime = makeTime(temp);

        #ifdef DEBUGEX
        Serial.println("Date correct. Checking time...");
        Serial.print("Now: ");
        Serial.println(now);
        Serial.print("StartTime: ");
        Serial.println(startTime);
        Serial.print("EndTime: ");
        Serial.println(endTime);
    #endif

        if (startTime <= now && now <= endTime) {
            #ifdef DEBUG
            Serial.println("Time correct. Go go!");
        #endif
            return true;
        }
    }

    return false;
}