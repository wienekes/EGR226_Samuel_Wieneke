/************************************************************************************
* Author: Samuel Wieneke
* Course: EGR 226 - 902
* Date: 3/17/2021
* Project: Lab 8
* File:
* Description: This program will change the duty cycle of the DC motor within CCS
* ** Uses SysTick
*
*
************************************************************************************/


#include "msp.h"
#include <stdio.h>


void SysTick_Init (void);
void SysTick_delay (uint16_t delay);
void DC_Motor (void);

volatile float duty_cycle;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    DC_Motor();
    SysTick_Init();


    float timeON, timeOFF, period;
    //float duty_cycle =10;
   // int on, off;

    duty_cycle = 10;
    period = 100;


    while(1)
    {
        timeON = ((duty_cycle/100)* period);
        timeOFF = period - timeON;

        //on = (int)timeON;
        //off = (int)timeOFF;

        P2->OUT |= BIT4;
        SysTick_delay(timeON);
        P2->OUT &= ~BIT4;
        SysTick_delay(timeOFF);
    }

}

/****| DC_Motor | *****************************************
 * Brief: This function is to initialize the pin
 * connected to the MSP
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void DC_Motor (void)
{
    //P2.4 is connected to DC Motor
    P2->SEL0 &= ~BIT4; // configure P2.4 as simple I/O
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4; // P2.4 set as output pin */

    P2->OUT &= ~BIT4;


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



