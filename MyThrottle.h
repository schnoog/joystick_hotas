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


void IniSHT(boolean Restart){
    if(!Restart)pinMode(SHT_LOX1, INPUT_PULLUP);
    if(Restart)ResetCount++;
    int Delay = 100;
    digitalWrite(SHT_LOX1, LOW);    
    delay(Delay);
    // all unreset
    digitalWrite(SHT_LOX1, HIGH);
    delay(Delay); 


    while (!lox.begin()) {
    debugln(F("Failed to boot VL53L0X"));
    debugln(F("Adafruit VL53L0X XShut set Low to Force HW Reset"));
    digitalWrite(SHT_LOX1, LOW);
    delay(Delay);
    digitalWrite(SHT_LOX1, HIGH);
    debugln(F("Adafruit VL53L0X XShut set high to Allow Boot"));
    delay(Delay);      
    }
    
}



#if CONTINOUSTHROTTLE == 1
void Restart_Continous(){
  ResetDevice();
  debugln(F("Rebooting"));
  delay(10);
  IniSHT(true);
  lox.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);  
  delay(100);
  lox.startRangeContinuous(200);  
}
#endif



void Throttle_Setup(){
    IniSHT(false);
    debugln(F("Setup Throttle"));
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
    tdiff = millis() - LastReadSuccess;
//    if ( RC == 10){
    if (tdiff > 200){

        RC=1;  
        if(lox.isRangeComplete()){
            //WorkVal = (int) lox.readRangeResult();
            WorkVal = (int) lox.readRange();
            LastReadSuccess = millis();
            if(WorkVal > 0){
                if(WorkVal < 300) LastRawThrottle = WorkVal;
                NotOKVal = 0;
            }else{
                NC++;
                if(NC == 2){
                  NC = 0;
                  Restart_Continous();                  
                }

              
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
    int MWorkVal = map(Myval, LidarMin  , LidarMax , ThrottleMin, ThrottleMax);
    if (Myval <= LidarMin) return ThrottleMin;
    if (Myval >= LidarMax) return ThrottleMax;
    if ( abs(MWorkVal - LastThrottle) > MinThrottleSingleStep){
        LastThrottle = MWorkVal;
        return MWorkVal;
    }
    return LastThrottle;
}
