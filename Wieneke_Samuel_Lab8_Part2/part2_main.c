/************************************************************************************
 * Author: Samuel Wieneke
 * Course: EGR 226 - 902
 * Date: 3/17/2021
 * Project: Lab 8
 * File:
 * Description: This program will change the duty cycle of the DC motor within CCS
 * **Uses TimerA
 *
 *
 ************************************************************************************/

#include "msp.h"
#include <stdio.h>




void DC_Motor (void);

volatile float duty_cycle=20;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

   // duty_cycle=20;

    //uint16_t duty_cycle = 20;
    DC_Motor();
    while(1)
    {

        //DC_Motor(duty_cycle);
        TIMER_A0->CCR[1] = (50000 * (duty_cycle/100));
    }

}


/****| DC_Motor | *****************************************
 * Brief: This function is to initialize Timer A and the Pin
 * connected to the MSP
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void DC_Motor (void)
{
    //P2.4 is connected to DC Motor
    // Configure GPIO for PWM output
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~(BIT4);
    P2->DIR |= BIT4; // P2.4 set TA0.1

    //duty_cycle = 80;

    TIMER_A0->CCR[0] = 50000 - 1; // PWM Period (# cycles of clock)
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // CCR1 reset/set mode 7
    TIMER_A0->CCR[1] = (50000 * (duty_cycle/100)); // CCR1 PWM duty cycle in 10ths of percent
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // use SMCLK
    TIMER_A_CTL_MC__UP | // in Up mode
    TIMER_A_CTL_CLR; // Clear TAR to start


}

