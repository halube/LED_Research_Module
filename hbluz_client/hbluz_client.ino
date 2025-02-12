#define DEBUG
//#define DEBUBEX
#ifdef DEBUG
    //#define DEBUGEX
#endif

//#define FIVE_D_PROTOTYPE
#define Annina_Jan_Modul
//#define FIVE_D
#ifdef FIVE_D
    #define FIVE_D_UVB
#endif


#define MANUAL_MODE

enum ClientStatus {
    READY,
    RUNNING,
    FINISHED
};


ClientStatus currentStatus = ClientStatus::READY;

const int LOOP_DELAY = 100;
const int LIGHTSETTINGS_DELAY = 1000;

int elapsedMillis = 1000;


void setup()
{

#ifdef DEBUG
    Serial.begin(115200);
#endif

    //
    //EEPROM
    setupEEPROM();

        //
    //RTC
    setupRTC();
    
    //
    //JSON
    parseJSON(getProgramFromMemory());

    //
    //Wireless
    setupWifi();

    //
    //Webserver
    setupWebserver();

    //
    //PWM
    setupPWM();

    //
    //Temperature
    //setupTemperature();

    //
    //Cooling
    //setupCooling();

}

void loop()
{
    //Webserver
    loopWebserver();

    //Lightsettings
    if (elapsedMillis >= LIGHTSETTINGS_DELAY) {
    if(currentStatus == ClientStatus::RUNNING)
    {

        loopLightsettings();



#ifdef DEBUG
        Serial.println(getTemperatureAsJSON());
        loopRTC();
#endif   
    }
    else
    {
        #ifdef DEBUG
        Serial.println("Client Status: Ready. Waiting for instructions...");
#endif  

    }
    elapsedMillis = 0;

    loopWifi();
    loopCooling();
       
}

    elapsedMillis += LOOP_DELAY;


    delay(LOOP_DELAY);
}
