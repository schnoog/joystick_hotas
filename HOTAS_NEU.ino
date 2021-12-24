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

int ITA = 0;
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
  int Work;
  // put your main code here, to run repeatedly:
  GetInputs();
  Joystick.sendState();
  Joystick2.sendState();
  unsigned long dura = micros() - LastCall;
  LastCall = micros();
  Work = runningAverageLong(dura);
  
  ITA++;
  if(ITA > 9){
    ITA = 0;
     debug("Loop average (us):");
     debugln(Work);
  }
  
  //delay(1);
}
