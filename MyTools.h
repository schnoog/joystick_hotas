
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

int SCALE_JOYSTICK_Y (int raw, int corr) {
  int Deadband = 1;
  int Curving = 4;

  int SIGN = 1;
  int scaled = raw;
  scaled = map (scaled, 0, 1023, -1023, 1023);
  if (scaled < 0) {
    SIGN = -1;
  }
  scaled = abs (scaled);
  if (scaled < Deadband){
    scaled = 0;
  }
  else {
    scaled = (long) (scaled-Deadband)*1000/ (long)(1000-Deadband);
  }
  int squared = ((long)scaled* (long) scaled)/1000;
  switch (Curving) {
    case 0:
       break;
    case 1: // low
      scaled = (2*scaled + squared)/3;
      break;
    case 2: // mild
      scaled = (scaled + squared)/2;
      break;
    case 3: // mid
      scaled = (scaled + 2*squared)/3;
      break;
    case 4: // strong
      scaled = squared;
     break;
  }
  scaled = SIGN*scaled;
  return scaled;
} // end of SCALE_JOYSTICK
