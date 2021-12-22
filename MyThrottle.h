Adafruit_VL53L0X lox = Adafruit_VL53L0X();


int average = 0;
int ThrottleMin = 0;
int ThrottleMax = 1023;

int LidarMin = 60;
int LidarMax = 228;
int LastThrottle;
int MinThrottleSingleStep = 1;

float ThrottleStep;


void Throttle_Setup(){
    //Serial.println("Adafruit VL53L0X test");
    if (!lox.begin()) {
    //Serial.println(F("Failed to boot VL53L0X"));
    while(1);
    }
    //ThrottleStep = (ThrottleMax - ThrottleMin) / (LidarMax - LidarMin);
    lox.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);    
    ThrottleStep = 5.919;
    LastThrottle = 9999;
}

int GetThrottleRaw(){
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    int mdata;
    mdata = (int)measure.RangeMilliMeter;
    average  = runningAverage(mdata);
    return average;
}



int GetThrottle(){
//    int Myval = GetThrottleAvg();
    int Myval = GetThrottleRaw();
    debugln(Myval);
    if (Myval <= LidarMin) return ThrottleMin;
    if (Myval >= LidarMax) return ThrottleMax;
    


    Myval = Myval - LidarMin;
    float TmpVal = (float) Myval * ThrottleStep;
    int WorkVal = (int) TmpVal;
    if ( abs(WorkVal - LastThrottle) > MinThrottleSingleStep){
        LastThrottle = WorkVal;
        return WorkVal;
    }
    
    //if (WorkVal > LastThrottle + MinThrottleSingleStep) {
    //    LastThrottle = WorkVal;
//        return WorkVal;
//    }
//    if (WorkVal < LastThrottle - MinThrottleSingleStep) {
//        LastThrottle = WorkVal;
//        return WorkVal;
//    }
    return LastThrottle;
}
