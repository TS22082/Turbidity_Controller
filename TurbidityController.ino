#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

int pumpPin = 9; //ENA on H-BRIDGE (PWM PIN)
int in1 = 2;     // in1 and in2 pins control polarity on h-bridge
int in2 = 3;
int pumpSpeedValue = 0;
int luxValue;

void configureSensor(void)
{
  tsl.enableAutoRange(true);                            /* Auto-gain ... switches automatically between 1x and 16x */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS); /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}

void setup(void)
{
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.println("Light Sensor Test");
  Serial.println("");
  if (!tsl.begin())
  {
    Serial.print("No TSL2561 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }
  configureSensor();
  Serial.println("");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void loop(void)
{
  sensors_event_t event;
  tsl.getEvent(&event);

  if (event.light)
  {
    luxValue = event.light;
    pumpSpeedValue = 255 - map(luxValue, 0, 1023, 0, 255);
    constrain(pumpSpeedValue, 100, 255); //constraint to make sure motor is never off
    analogWrite(pumpPin, pumpSpeedValue);
    Serial.println("Lux Value: " + luxValue);
    Serial.println("pump speed (0-255): " + pumpSpeedValue);
    delay(2);
  }
  else
  {
    Serial.println("Sensor overload");
  }
  delay(250);
}
