
int CorrAxis0 = 0;
int CorrAxis1 = 0;

int SCALE_JOYSTICK (int raw, int corr ){
  int Off = abs(512 - raw);
  
  long workval = Off;
  long outval = 512;
  
  workval = workval * workval;
  workval = workval / 512;

  

  if(raw > 512){
    outval =  512 + workval;
  }
  
  if(raw < 512){
    outval =  512 - workval;
  }
  
  
  return (int)outval;
}

int runningAverage(int M) {
  #define LM_SIZE 4
  static int LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static long sum = 0;
  static byte count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  return (int)sum / count;
}



long runningAverageLong(long M) {
  #define LM_SIZE 10
  static int LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static long sum = 0;
  static byte count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  return sum / count;
}
