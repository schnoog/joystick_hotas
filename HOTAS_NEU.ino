#define DEBUG 1
#define CONTINOUSTHROTTLE 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x) 
#define debugln(x) 
#endif

//#include <SoftwareReset.h>
#include "Adafruit_VL53L0X.h"
#include <avr/wdt.h>
#include "MyTools.h"
#include "MyADS.h"
#include "Adafruit_MCP23017.h"
#include "MyThrottle.h"
#include "MyMCP.h"

#include "MyJoystick.h"
#include "MyInputDef.h" 

#include "Settings.h"

int LidarDist;
long LoopStart = 0;



void setup() {

#if DEBUG == 1
  Serial.begin(115200);
  delay(100);
#endif
  InputDef_Setup();
  Throttle_Setup();
  MCP_Setup();
  ADS_Setup();
  LoopStart = micros();
}

void loop() {
  ADS_Loop();
  GetInputs();
  Joystick.sendState();
}
