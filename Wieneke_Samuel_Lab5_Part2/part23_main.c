#include "msp.h"

int DebounceSwitch1(void);
void SysTick_Init (void);
void SysTick_delay (uint16_t delay);


void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init ();

    P3->SEL1 &= ~BIT6; // configure P3.6 as simple I/O
    P3->SEL0 &= ~BIT6;
    P3->DIR &= ~BIT6; // P3.6 set as output pin */
    P3->REN |= BIT6;
    P3->OUT |= BIT6;

    P3->SEL1 &= ~BIT7; // configure P3.7 as simple I/O
    P3->SEL0 &= ~BIT7;
    P3->DIR &= ~BIT7; // P3.7 set as output pin */
    P3->REN |= BIT7;
    P3->OUT |= BIT7;

    P2->SEL1 &= ~BIT5; // configure P2.5 as simple I/O
    P2->SEL0 &= ~BIT5;
    P2->DIR |= BIT5; // P2.5 set as output pin */

    P3->SEL1 &= ~BIT0; // configure P3.0 as simple I/O
    P3->SEL0 &= ~BIT0;
    P3->DIR |= BIT0; // P3.0 set as output pin */

    P5->SEL1 &= ~BIT7; // configure P5.7 as simple I/O
    P5->SEL0 &= ~BIT7;
    P5->DIR |= BIT7; // P5.7 set as output pin */

    int i=0;

    P2->OUT &= ~BIT5;
    P3->OUT &= ~BIT0;
    P5->OUT &= ~BIT7;


    while (1) {





        while((P3->IN & BIT7) != BIT7)
        {

            if(i==3)
            {
                i=0;
            }

            if(i==0)
            {
                P3->OUT &= ~BIT0;
                P2->OUT &= ~BIT5;
                P5->OUT |= BIT7; //green
                SysTick_delay(1000);
                i++;
            }

            else if(i==1)
            {
                P2->OUT &= ~BIT5;
                P5->OUT &= ~BIT7;
                P3->OUT |= BIT0; //yellow
                SysTick_delay(1000);
                i++;
            }
            else if(i==2)
            {
                P5->OUT &= ~BIT7;
                P3->OUT &= ~BIT0;
                P2->OUT |= BIT5; //red
                SysTick_delay(1000);
                i++;
            }



        }

        while((P3->IN & BIT6) != BIT6)
        {

            if(i==0)
            {
                i=3;
            }

            if(i==1)
            {
                P3->OUT &= ~BIT0;
                P2->OUT &= ~BIT5;
                P5->OUT |= BIT7; //green
                SysTick_delay(1000);
                i--;
            }

            else if(i==2)
            {
                P2->OUT &= ~BIT5;
                P5->OUT &= ~BIT7;
                P3->OUT |= BIT0; //yellow
                SysTick_delay(1000);
                i--;
            }
            else if(i==3)
            {
                P5->OUT &= ~BIT7;
                P3->OUT &= ~BIT0;
                P2->OUT |= BIT5; //red
                SysTick_delay(1000);
                i--;
            }





                }




    }
}


int DebounceSwitch1(void)
{
    int pin_Value = 0; //initialize variable as low
    if ((P3IN & 0x80) == 0x00) //check of button pushed
    { __delay_cycles(300000); //pause for 100 m-sec for switch bounce
    if ((P3IN & 0x80) == 0x00) //check of button is still pushed
        pin_Value = 1;
    }

    return pin_Value; //return 1 if pushed- 0 if not pushed
}

void SysTick_Init (void)
{ //initialization of systic timer
    SysTick->CTRL = 0; // disable SysTick During step
    SysTick->LOAD = 0x00FFFFFF; // max reload value
    SysTick->VAL = 0; // any write to current clears it
    SysTick->CTRL = 0x00000005; // enable systic, 3MHz, No Interrupts
}

void SysTick_delay (uint16_t delay)
{ // Systick delay function
    SysTick->LOAD = ((delay * 3000) - 1); //delay for 1 msecond per delay value
    SysTick->VAL = 0; // any write to CVR clears it
    while ( (SysTick->CTRL & 0x00010000) == 0); // wait for flag to be SET
}


