#include "msp.h"
#include <stdio.h>

void setupInterrupts(void);
void PORT1_IRQHandler (void);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
    __enable_interrupt();

    setupInterrupts();

    fflush(stdin);
    while(1)
    {

    }
}


void setupInterrupts()
{
    // Blue
    P1->SEL1 &= ~BIT6; // configure P1.6 as simple I/O
    P1->SEL0 &= ~BIT6;
    P1->DIR &= ~BIT6; // P1.6 set as output pin */
    P1->REN |= BIT6;
    P1->OUT |= BIT6;
    P1->IES |= BIT6;
    P1->IE |= BIT6;
    P1->IFG &= ~BIT6;

    // Red
    P1->SEL1 &= ~BIT7; // configure P1.7 as simple I/O
    P1->SEL0 &= ~BIT7;
    P1->DIR &= ~BIT7; // P1.7 set as output pin */
    P1->REN |= BIT7;
    P1->OUT |= BIT7;
    P1->IES |= BIT7;
    P1->IE |= BIT7;
    P1->IFG &= ~BIT7;


}

void PORT1_IRQHandler ()
{
    if(P1->IFG & BIT6)
    {
        printf("pressed blue\n");
    }
    if(P1->IFG & BIT7)
    {
        printf("pressed red\n");
    }
    P1->IFG &= ~BIT6;
    P1->IFG &= ~BIT7;


}
