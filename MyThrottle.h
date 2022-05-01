Adafruit_VL53L0X lox = Adafruit_VL53L0X();


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
int NotOKLimit = 30;
int LSV;
int MaxSame = 30;
int RC = 9;
#define SHT_LOX1 7


void IniSHT(boolean Restart){
    int Delay = 10;
    if(Restart) Delay = 100;
    digitalWrite(SHT_LOX1, LOW);    
    delay(Delay);
    // all unreset
    digitalWrite(SHT_LOX1, HIGH);
    delay(Delay); 
    if (!lox.begin()) {
    while(1);
    } 
}


//float ThrottleStep;

#if CONTINOUSTHROTTLE == 1
void Restart_ContinousOld(){
  debugln("TOF restart");
     if (!lox.begin()) {
    while(1);
    }
    lox.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);     
 lox.stopRangeContinuous();
 delay(100);
 lox.startRangeContinuous(200);  
}

void Restart_Continous(){
  debugln("Rebooting");
  delay(10);
  IniSHT(true);
  lox.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);  
  delay(100);
  lox.startRangeContinuous(200);  
}


#endif



void Throttle_Setup(){
    IniSHT(false);
    debugln("Setup Throttle");

#if CONTINOUSTHROTTLE == 1
    LSV = 0;
    LSame = 0;
    lox.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);        
    lox.startRangeContinuous(200);
#else

#endif
    
    LastThrottle = 9999;
}

int GetThrottleRaw(){

#if CONTINOUSTHROTTLE == 1
    RC++;

    if ( RC == 10){
    RC=1;  
    

        if(lox.isRangeComplete()){
            int WorkVal = (int) lox.readRangeResult();
    //        debugln("NewVal");        
            if(WorkVal > 0){
                if(WorkVal < 300) LastRawThrottle = WorkVal;
                NotOKVal = 0;
            }
          
        }else{
            NotOKVal++;
            if(NotOKVal > NotOKLimit){
                  NotOKVal = 0;
                  Restart_Continous();
            }
          
        }

    }
    
#else

    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      LastRawThrottle = (int) measure.RangeMilliMeter;
    } 
#endif
  

    
    return LastRawThrottle;
}



int GetThrottle(){
    int Myval = GetThrottleRaw();
//    debug("raw ");
//    debug(Myval);
//    debug(" mapped ");
    int WorkVal = map(Myval, LidarMin  , LidarMax , ThrottleMin, ThrottleMax);
//    debugln(WorkVal);
    if (Myval <= LidarMin) return ThrottleMin;
    if (Myval >= LidarMax) return ThrottleMax;
    if ( abs(WorkVal - LastThrottle) > MinThrottleSingleStep){
        LastThrottle = WorkVal;
        return WorkVal;
    }
    return LastThrottle;
}
