#define DEBUG


#define VALVE_PIN D4 // This is shared with the build in LED (ups...)
#define LOW_POWER_PIN D5




void setupCooling()
{
  pinMode(VALVE_PIN, OUTPUT);

  println("5D Valve Demo");
  //pinMode(LOW_POWER_PIN, OUTPUT); // We don't need this but you might
  //digitalWrite(LOW_POWER_PIN, LOW);
}

void loopCooling()
{
float currentTemp=(getTemperatureByIndex(0)+getTemperatureByIndex(1))/2.0f;
    println(String(currentTemp)); 

  if (currentTemp >= tempUpperThreshold)
  {
    println("Opening valve...");
    digitalWrite(VALVE_PIN, HIGH);
  }
//delay(2000);
  if (currentTemp <= tempLowerThreshold)
  {
    println("Closing valve");
    digitalWrite(VALVE_PIN, LOW);
  }
//delay(2000);
}

inline void print(String message)
{
#ifdef DEBUG
  Serial.print(message);
#endif
}

inline void println(String message)
{
#ifdef DEBUG
  Serial.println(message);
#endif
}
