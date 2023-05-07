

VL53L0X_mod sensor;

//int average = 0;
int ThrottleMin = 0;
int ThrottleMax = 1023;
int LastRawThrottle;
int LidarMin = 58;
int LidarMax = 210;
int LastThrottle = 0;
int MinThrottleSingleStep = 1;
int LSame;
int NotOKVal = 0;
int NotOKLimit = 5;
int LSV;
int MaxSame = 30;
int RC = 9;
int NC = 0;
int ResetCount = 0;
int WorkVal;
long LastReadSuccess = 0;
long tdiff = 0;

#define SHT_LOX1 7



void Throttle_Setup(){
    debugln(F("Setup Throttle"));
    Wire.begin();
    sensor.setTimeout(500);
    if (!sensor.init())
    {
      Serial.println("Failed to detect and initialize sensor!");
      while (1) {}
    }
    sensor.startContinuous(200);
    LastThrottle = 9999;
}

int GetThrottleRaw(){
  uint16_t NewMeasurement;
  tdiff = millis() - LastReadSuccess;
  if (tdiff > 200){
    if(sensor.readRangeNoBlocking(NewMeasurement)){
      LastRawThrottle = (int)NewMeasurement;
      LastReadSuccess = millis();
    }
  }
  return LastRawThrottle;
}



int GetThrottle(){
    int Myval = GetThrottleRaw();
    int MWorkVal = map(Myval, LidarMin  , LidarMax , ThrottleMin, ThrottleMax);
    if (Myval <= LidarMin) return ThrottleMin;
    if (Myval >= LidarMax) return ThrottleMax;
    if ( abs(MWorkVal - LastThrottle) > MinThrottleSingleStep){
        LastThrottle = MWorkVal;
        return MWorkVal;
    }
    return LastThrottle;
}
