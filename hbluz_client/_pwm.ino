#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
int pulseFrequency = 500;

void setupPWM() 
{
    pwm.begin();
    pwm.setPWMFreq(pulseFrequency); // This is the maximum PWM frequency    
    resetLEDS();
}

void setPulseFrequency(int frequency)
{
    #ifdef DEBUG
    Serial.print("Setting Pulse Frequency: ");
    Serial.println(frequency);
#endif
    pwm.setPWMFreq(frequency); 
}

void resetLEDS()
{
    for(int i = 0;i < 16;i++)
    {
        pwm.setPWM(i, 0, 4096);    
    }    
}