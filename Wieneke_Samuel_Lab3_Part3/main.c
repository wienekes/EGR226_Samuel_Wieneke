#include "msp.h"



void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    P1->DIR = BIT0; // configures GPIO for output on P1.0 LED1 on MSP

    //temporary variable for loop-maintenance
    int i,q;


    while (1) {
        P1->OUT ^= BIT0;  //toggle LED status
        for(i=q; i>0; i--);  //Crude delay
    }
}
