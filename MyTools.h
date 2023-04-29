#define LM_SIZE 4
int CorrAxis0 = 0;
int CorrAxis1 = 0;

//void(* resetFunc) (void) = 0;
void ResetDevice(){
    wdt_enable(WDTO_15MS);
    while (1) {}
}