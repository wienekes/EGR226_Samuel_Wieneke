#include "msp.h"
#include <stdio.h>

void TimerA0Init(void);

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop watchdog timer

    fflush(stdin);

    TimerA0Init();

    while (1)
    {
        ;
    }

}

void TimerA0Init(void)
{
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4; // P2.4 set TA0.1
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK
            TIMER_A_CTL_MC__UP | // Up mode
            TIMER_A_CTL_CLR | //Clear timer
            TIMER_A_CTL_ID__8; //Divide clock by 8
    TIMER_A0->CCR[0] = 26785; // 14 Hz
    TIMER_A0->CCR[1] = 26785 / 2; // 50% duty cycle
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
}
