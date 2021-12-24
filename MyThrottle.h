Adafruit_VL53L0X lox = Adafruit_VL53L0X();


//int average = 0;
int ThrottleMin = 0;
int ThrottleMax = 1023;
int LastRawThrottle;
int LidarMin = 58;
int LidarMax = 210;
int LastThrottle = 0;
int MinThrottleSingleStep = 1;

//float ThrottleStep;


void Throttle_Setup(){
    if (!lox.begin()) {
    while(1);
    }
    lox.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);        
    lox.startRangeContinuous(200);
    LastThrottle = 9999;
}

int GetThrottleRaw(){  
    if(lox.isRangeComplete()){
        int WorkVal = (int) lox.readRangeResult();
        if(WorkVal > 0){
            if(WorkVal < 300) LastRawThrottle = WorkVal;
        }      
    }
    return LastRawThrottle;
}



int GetThrottle(){
    int Myval = GetThrottleRaw();
    debug("raw ");
    debug(Myval);
    debug(" mapped ");
    int WorkVal = map(Myval, LidarMin  , LidarMax , ThrottleMin, ThrottleMax);
    debugln(WorkVal);
    if (Myval <= LidarMin) return ThrottleMin;
    if (Myval >= LidarMax) return ThrottleMax;
    if ( abs(WorkVal - LastThrottle) > MinThrottleSingleStep){
        LastThrottle = WorkVal;
        return WorkVal;
    }
    return LastThrottle;
}
