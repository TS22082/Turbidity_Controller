#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
   
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

int motorPin = 9;
int motorValue = 0;
int luxValue;

void configureSensor(void) {
  tsl.enableAutoRange(true); /* Auto-gain ... switches automatically between 1x and 16x */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}

void setup(void) {
  Serial.begin(9600);
  Serial.println("Light Sensor Test"); Serial.println("");
  
  if(!tsl.begin()) {
    Serial.print("No TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  configureSensor();
  Serial.println("");
}

void loop(void) {  
  sensors_event_t event;
  tsl.getEvent(&event);

  if (event.light){
    luxValue = event.light;  
    motorValue = map(luxValue, 0, 1023, 0, 255) - 255;
    analogWrite(motorPin, motorValue);  
    Serial.print("Lux Value = ");     
    Serial.print(luxValue);
    Serial.print(" motor = ");
    Serial.println(motorValue);
    delay(2);
  }
  else {
    Serial.println("Sensor overload");
  }
  delay(250);    
}
