#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x) 
#define debugln(x) 
#endif

#include "Adafruit_VL53L0X.h"

#include "MyTools.h"

#include "Adafruit_MCP23017.h"
#include "MyThrottle.h"
#include "MyMCP.h"

#include "MyJoystick.h"
#include "MyInputDef.h" 

#include "Settings.h"


int LidarDist;
int LastDur;
unsigned long LastCall;
void setup() {
  Serial.begin(115200);
  delay(100);
  InputDef_Setup();
  Throttle_Setup();
  MCP_Setup();
  LastCall = micros();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  GetInputs();
  Joystick.sendState();
  Joystick2.sendState();
  unsigned long dura = micros() - LastCall;
  LastCall = micros();
  debug("Loop average (us):");
  debugln(runningAverageLong(dura));
  
  //delay(1);
}
