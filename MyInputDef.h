#define DEBOUNCE_TIME 5 // ms delay before the push button changes state

// Button
// JOYSTICK 1
//  0 - 12  MCP 1    (frei 13, 14, 15)
// 13 - 16  Analog 2 Digital 
// 17       Analog-Click - bei Modifier
// JOYSTICK 2
// 18 - 31  MCP 2



int axis0 = 0;
int axis1 = 0;
int axis2 = 0;
int axis3 = 0;

int Axis0_cutoffLow = 460;
int Axis0_cutoffHigh = 500;
int Axis1_cutoffLow = 485;
int Axis1_cutoffHigh = 585;


boolean SuppressAxisBtn = false;
int SuppressStep = 0;



int VirtAxDiff = 200;

int VirtAxModifierButton = 0;
int VirtAxModifiereSuppressButton = 12;
int VirtAxMod_ReplacementKeyVal = 0;
bool VirtAxMod_ReplacementKeyWork = false;
int axis_modifier_overwrite = 512;
bool IsModified = false;


byte switch_state[64];
byte switch_state_old[64];
byte reading, clk, clk_old, realpin, realbutton;
unsigned long debounce_time[64];

void InputDef_Setup(){
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    int average;

    average = 0;
    for (int i=0; i < 10; i++) {
      average = average + analogRead(A0);
    }
    average = average/10;
    CorrAxis0 = 512 - average; 

    average = 0;
    for (int i=0; i < 10; i++) {
      average = average + analogRead(A1);
    }
    average = average/10;
    CorrAxis1 = 512 - average;
    CorrAxis0 = 0;
    CorrAxis1 = 0;

}


int debounceVal(int BtnNum, int CurrentVal, int JoyStickNum){
            int FinalBtnNum = 0;
            FinalBtnNum = BtnNum;
            reading = (byte)CurrentVal;
            if (reading == switch_state[BtnNum]) debounce_time[BtnNum] = millis() + (unsigned long)DEBOUNCE_TIME;
            else if (millis() > debounce_time[BtnNum]) switch_state[BtnNum] = reading;
            if (switch_state[BtnNum] != switch_state_old[BtnNum]) { // debounced button has changed state
              if (switch_state[BtnNum]) {

                    if(BtnNum == VirtAxModifierButton){
                            IsModified = true;
                    }else{
                        if (BtnNum != VirtAxModifiereSuppressButton){
                        Joystick.pressButton(FinalBtnNum); 

                        debug(F("Press Button "));
                        debug(JoyStickNum);
                        debug(" : ");
                        debug(BtnNum);
                        debug(" - ");
                        debugln(FinalBtnNum);




                        if(JoyStickNum == 2){
                          if(BtnNum == 39) SuppressStep++;
                          if(BtnNum == 40) SuppressStep++;
                          if(BtnNum == 41) SuppressStep++;
                          if (SuppressStep == 3){
                              debug("Schalte AxenButtons ");
                              if(SuppressAxisBtn){
                                  debugln("ein");
                                  SuppressAxisBtn = false;
                              }else{
                                  debugln("aus");
                                  SuppressAxisBtn = true;                                
                              }
                              SuppressStep = 0 ;
                            
                          }

                                                                              
                        }else{
                          SuppressStep = 0 ; 
                        }




                        }else{
                            if(IsModified){
                                VirtAxMod_ReplacementKeyWork = true;
                                VirtAxMod_ReplacementKeyVal = 1;
                            }else{
                                Joystick.pressButton(FinalBtnNum); 
                                
                            //    Serial.print(F("Press Button "));
                            //    Serial.println(FinalBtnNum);                                
                            }
                        }
                    }
                }else{
                    if(BtnNum == VirtAxModifierButton){
                            IsModified = false;
                    }else{
                        if (BtnNum != VirtAxModifiereSuppressButton){
                            Joystick.releaseButton(FinalBtnNum);
                            
                        //    Serial.print("Release Button ");
                        //    Serial.println(FinalBtnNum);
                        }else{
                            if(IsModified){
                                VirtAxMod_ReplacementKeyWork = true;
                                VirtAxMod_ReplacementKeyVal = 0;
                            }else{
                                Joystick.releaseButton(FinalBtnNum);
                                
                            //    Serial.print("Release Button ");
                            //    Serial.println(FinalBtnNum);
                            }
                        }
                    }
                }
              switch_state_old[BtnNum] = switch_state[BtnNum]; // store new state such that the above gets done only once
            }
}


void GetInputs(){
        int Button = 0;

        // MCP 1 Digital Inputs
        //                  0 1 2 3 4 5 6  7  8  9  10  11 12
        int mcp1pins  [] = {14,4,5,6,7,8,9,10,11,13,12,15,0};
        for (int i=0; i<sizeof mcp1pins/sizeof mcp1pins[0]; i++) {           
            int s = mcp1pins[i];
            debounceVal(Button,(int) !mcp1.digitalRead(s),1);
            Button++;
        }
        // Todo:Modifier - wenn gedrückt axis0 und 1 auf Mittelstellung (512) setzen
        // Analog zu digital wandeln
        int axis0_raw;
        int axis1_raw;
        //auslesen der 2 achsen des daumen joysticks
        axis0_raw = analogRead(A0);
        axis1_raw = analogRead(A1);

        //Nullpunkt-Korrektur der Werte
//        axis0 = axis0_raw + CorrAxis0;
//        axis1 = axis1_raw + CorrAxis1;
        //Nullpunkt flattern beseitigen (zwischen 501 und 523 wird zu 512)
//        if (axis0 > 501 && axis0 < 523){axis0 = 512;}
//        if (axis1 > 501 && axis1 < 523){axis1 = 512;}
        //Eventuelle Grenzverletzungen entfernen
        //axis1 = map(axis1,125,860,0,1023);
        //axis0 = map(axis0,90,980,0,1023);

//        axis0 = map(axis0,125,860,0,1023);
//        axis1 = map(axis1,90,980,0,1023);


        axis0 = 512;
        axis1 = 512;

        if(axis0_raw < Axis0_cutoffLow) axis0 = map(axis0_raw,90,Axis0_cutoffLow,0,512);     
    
        if(axis0_raw > Axis0_cutoffHigh) axis0 = map(axis0_raw,Axis0_cutoffHigh,880,512,1023);     
    
        if(axis1_raw < Axis1_cutoffLow)  axis1 = map(axis1_raw,90,Axis1_cutoffLow,0,512);    
    
        if(axis1_raw > Axis1_cutoffHigh)   axis1 = map(axis1_raw,Axis1_cutoffHigh,960,512,1023);     
    
    
        if (axis0 < 0) axis0 = 0; 
        if (axis0 > 1023) axis0 = 1023;
        if (axis1 < 0) axis1 = 0;

        if (axis1 > 1023) axis1 = 1023;
        


        //axis2 = analogRead(A2);
        //axis3 = analogRead(A3);
        //binaere achsen (klick wenn groesser als schwellenwert)
        int axis0_A = 0;
        int axis0_B = 0;
        int axis1_A = 0;
        int axis1_B = 0;

        //wenn modified Button gedrückt ist
        if (IsModified){
            int mmin = 512 - VirtAxDiff;
            int mmax = 512 + VirtAxDiff; 
            if (axis0 > mmax)axis0_A = 1;
            if (axis0 < mmin)axis0_B = 1;
            if (axis1 > mmax)axis1_A = 1;
            if (axis1 < mmin)axis1_B = 1;
            axis2 = axis0;
            axis3 = axis1;
            axis0 = axis_modifier_overwrite;
            axis1 = axis_modifier_overwrite;
        }else{
            axis2 = axis_modifier_overwrite;
            axis3 = axis_modifier_overwrite;

        }

    if(!SuppressAxisBtn) debounceVal(Button,axis0_A,1);
            Button++;
    if(!SuppressAxisBtn) debounceVal(Button,axis0_B,1);
            Button++;
    if(!SuppressAxisBtn) debounceVal(Button,axis1_A,1);
            Button++;
    if(!SuppressAxisBtn) debounceVal(Button,axis1_B,1);
            Button++;

            axis0_A = 0;
            axis0_B = 0;
            axis1_A = 0;
            axis1_B = 0;

        if (!IsModified){
        int mmin = 513 - VirtAxDiff;
        int mmax = 513 + VirtAxDiff; 
        if (axis0 > mmax)axis0_A = 1;
        if (axis0 < mmin)axis0_B = 1;
        if (axis1 > mmax)axis1_A = 1;
        if (axis1 < mmin)axis1_B = 1;
        }

if(!SuppressAxisBtn) debounceVal(Button,axis0_A,1);
            Button++;
if(!SuppressAxisBtn) debounceVal(Button,axis0_B,1);
            Button++;
if(!SuppressAxisBtn) debounceVal(Button,axis1_A,1);
            Button++;
if(!SuppressAxisBtn) debounceVal(Button,axis1_B,1);
            Button++;

        if(VirtAxMod_ReplacementKeyWork){
            if (VirtAxMod_ReplacementKeyVal == 1){
                Joystick.pressButton(Button);
            }else{
                Joystick.releaseButton(Button);
            }
        }
        Button++;

//debug("Out-:");
//debug(axis0);
//debug("---");
//debugln(axis1);
        
        
        //einmal durchreichen
        Joystick.setXAxis(axis0);
//        Serial.print(axis0);
//        Serial.print("-");
//        Serial.println(axis1);
        Joystick.setYAxis(axis1);
        Joystick.setRxAxis(axis2);
        Joystick.setRyAxis(axis3);

        // MCP 2 Digital Inputs
        int mcp2pins  [] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        for (int i=0; i<sizeof mcp2pins/sizeof mcp2pins[0]; i++) {           
            int s = mcp2pins[i];
            debounceVal(Button,(int) !mcp2.digitalRead(s),2);
            Button++;
        }

        int mcp3pins  [] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        for (int i=0; i<sizeof mcp3pins/sizeof mcp3pins[0]; i++) {           

            int s = mcp3pins[i];
            if(MCPR2){
                debounceVal(Button,(int) !mcp3.digitalRead(s),2);
            }else{
                debounceVal(Button,(int) !mcp3.digitalRead(s),2);
            }
            Button++;
        }


        // Throttle
        int Throttlevel = GetThrottle();                
        //Only reporting changes
        if (Throttlevel < 1024){
        Joystick.setThrottle(Throttlevel);
        }

        debug(axis0_raw);
        debug("<->");
        debug(axis1_raw);
        debug("<->");
        debug(axis0);
        debug("<->");
        debug(axis1);
        debug("<--->");
        debug(WorkVal );
        debug("<->");
        debug(Throttlevel);
        debug("-");
        debug(NC);        
        debug("/");
        debug(NotOKVal);
        debug("/");
        debug(NotOKLimit);
        debug("/Reset:");
        debugln(ResetCount);
        

}
