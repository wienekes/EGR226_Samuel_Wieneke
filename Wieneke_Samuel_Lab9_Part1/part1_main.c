/************************************************************************************
 * Author: Samuel Wieneke
 * Course: EGR 226 - 902
 * Date: 3/17/2021
 * Project: Lab 9
 * File:
 * Description: This program will change the duty cycle of the DC motor within CCS
 * **Uses TimerA
 *
 *
 ************************************************************************************/

#include "msp.h"
#include <stdio.h>



void button_init(void);
void DC_Motor (int duty_cycle);
void SysTick_Init (void);
void SysTick_delay (uint16_t delay);int DebounceSwitch1(void);

int duty_cycle=20;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer


    DC_Motor(duty_cycle);

    button_init();
    SysTick_Init();

    while(1)
    {

        if((P1->IN & BIT6) != BIT6) //blue button, increments by 10%
        {
            duty_cycle = duty_cycle +10;
            if (duty_cycle == 100)
            {
                duty_cycle = 0;
            }
            DC_Motor(duty_cycle);
            SysTick_delay(1000);
        }

        if((P1->IN & BIT7) != BIT7) //red button, decrements by 10%
        {



            duty_cycle = duty_cycle -10;
            if (duty_cycle == 0)
            {
                duty_cycle = 100;
            }
            DC_Motor(duty_cycle);
            SysTick_delay(1000);




        }
        if((P4->IN & BIT2) != BIT2) //white button, turns motor off
        {
            duty_cycle = 0;
            DC_Motor(duty_cycle);
            SysTick_delay(1000);

        }

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
void DC_Motor (int duty_cycle)
{
    //P2.4 is connected to DC Motor
    // Configure GPIO for PWM output
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~(BIT4);
    P2->DIR |= BIT4; // P2.4 set TA0.1



    TIMER_A0->CCR[0] = 30000 - 1; // PWM Period (# cycles of clock)
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // CCR1 reset/set mode 7
    TIMER_A0->CCR[1] = (30000 * (duty_cycle/100.0)); // CCR1 PWM duty cycle in 10ths of percent
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // use SMCLK
    TIMER_A_CTL_MC__UP | // in Up mode
    TIMER_A_CTL_CLR; // Clear TAR to start


}

/****| button_init | *****************************************
 * Brief: This function is to initialize the buttons
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void button_init(void)
{
    // Blue
    P1->SEL1 &= ~BIT6; // configure P1.6 as simple I/O
    P1->SEL0 &= ~BIT6;
    P1->DIR &= ~BIT6; // P1.6 set as output pin */
    P1->REN |= BIT6;
    P1->OUT |= BIT6;

    // Red
    P1->SEL1 &= ~BIT7; // configure P1.7 as simple I/O
    P1->SEL0 &= ~BIT7;
    P1->DIR &= ~BIT7; // P1.7 set as output pin */
    P1->REN |= BIT7;
    P1->OUT |= BIT7;

    // White
    P4->SEL1 &= ~BIT2; // configure P4.2 as simple I/O
    P4->SEL0 &= ~BIT2;
    P4->DIR &= ~BIT2; // P4.2 set as output pin */
    P4->REN |= BIT2;
    P4->OUT |= BIT2;
}

/****| SysTick_Init | *****************************************
 * Brief: This function is used for the initialization
 * of the SysTick timer
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void SysTick_Init (void)
{ //initialization of systic timer
    SysTick->CTRL = 0; // disable SysTick During step
    SysTick->LOAD = 0x00FFFFFF; // max reload value
    SysTick->VAL = 0; // any write to current clears it
    SysTick->CTRL = 0x00000005; // enable systic, 3MHz, No Interrupts
}

/****| SysTick_delay | *****************************************
 * Brief: This function sets the SysTick timer to be used as a
 * delay when called
 * param:
 * (uint16_t) data: one integer worth of data input that
 * determines the duration of the delay
 * return:
 * N/A
 *************************************************************/
void SysTick_delay (uint16_t delay)
{ // Systick delay function
    SysTick->LOAD = ((delay * 3000) - 1); //delay for 1 msecond per delay value
    SysTick->VAL = 0; // any write to CVR clears it
    while ( (SysTick->CTRL & 0x00010000) == 0); // wait for flag to be SET
}

