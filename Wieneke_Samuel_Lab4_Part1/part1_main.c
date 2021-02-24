#include "msp.h"

int DebounceSwitch1(void);
//int DebounceDelay (void);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer



    P1->SEL1 &= 0xFD; // configure P1.1 as simple I/O
    P1->SEL0 &= 0xFD;
    P1->DIR &= 0xFD; // P1.1 set as output pin */
    P1->REN |= 0x02;
    P1->OUT |= 0x02;

    P2->SEL1 &= ~1; // configure P2.0 as simple I/O
    P2->SEL0 &= ~1;
    P2->DIR |= 1; // P2.0 set as output pin */

    P2->SEL1 &= ~2; // configure P2.1 as simple I/O
    P2->SEL0 &= ~2;
    P2->DIR |= 2; // P2.1 set as output pin */

    P2->SEL1 &= ~4; // configure P2.2 as simple I/O
    P2->SEL0 &= ~4;
    P2->DIR |= 4; // P2.2 set as output pin */

    int i=0;

    P2->OUT &= ~1;
    P2->OUT &= ~2;
    P2->OUT &= ~4;
    while (1) {




        while(DebounceSwitch1())
        {
            if(i==0)
                        {
                            P2->OUT &= ~2;
                            P2->OUT &= ~4;
                            P2->OUT ^= 1;
                            i++;
                        }
            else if(i==1)
                         {


                              P2->OUT &= ~4;
                              P2->OUT &= ~1;
                              P2->OUT ^= 2;
                              i++;
                         }
            else if(i==2)
                        {
                            P2->OUT &= ~1;
                            P2->OUT &= ~2;
                            P2->OUT ^= 4;
                            i=0;
                        }
           /* else if(i==3)
                        {
                            P2->OUT &= ~1;
                            P2->OUT &= ~2;
                            P2->OUT &= ~4;
                            i=0;
                        }*/
        }





    }
}


int DebounceSwitch1(void)
{
    int pin_Value = 0; //initialize variable as low
    if ((P1IN & 0x02) == 0x00) //check of button pushed
        { __delay_cycles(300000); //pause for 10 m-sec for switch bounce
        if ((P1IN & 0x02) == 0x00) //check of button is still pushed
            pin_Value = 1;
        }

    return pin_Value; //return 1 if pushed- 0 if not pushed
}


