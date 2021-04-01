/************************************************************************************
 * Author: Samuel Wieneke
 * Course: EGR 226 - 902
 * Date: 3/17/2021
 * Project: Lab 8
 * File:
 * Description: This program will change the duty cycle of the DC motor by using keypad
 * **Uses TimerA
 *
 *
 ************************************************************************************/

#include "msp.h"
#include <stdio.h>

void keypad_init (void);
uint8_t Read_Keypad(void);
void SysTick_Init (void);
void SysTick_delay (uint16_t delay);
void DC_Motor (int duty_cycle);

uint8_t pressed, num;
int duty_cycle=50;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer



    keypad_init();
    SysTick_Init();
    DC_Motor(duty_cycle);


    int i=0;
    uint8_t pinArray[2];




    while(1)
    {

        pressed = Read_Keypad();


        if(pressed)
        {


            if(num != 10 && num !=12) //if the number is not * or #
            {
                if (i==2)
                {
                    pinArray[0] = pinArray[1]; //takes the last 2 numbers pressed
                    pinArray[1] = num;
                }

                if (i<2)
                {
                    pinArray[i] = num;
                    i++;
                }
            }

            if(num == 12 && i==2) //once # is pressed and there are 2 numbers it will change the value of duty cycle
            {
                duty_cycle = ((10 * pinArray[0]) + pinArray[1]);
                DC_Motor(duty_cycle);
            }
            if(num == 12 && i==1)
            {
                duty_cycle = pinArray[0];
                DC_Motor(duty_cycle);
            }
        }

    }

}

/****| DC_Motor | *****************************************
 * Brief: This function is to initialize Timer A and the Pin
 * connected to the MSP
 * param: int duty_cycle
 *
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

    //duty_cycle = 80;

    TIMER_A0->CCR[0] = 50000 - 1; // PWM Period (# cycles of clock)
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // CCR1 reset/set mode 7
    TIMER_A0->CCR[1] = (50000 * (duty_cycle/100.0)); // CCR1 PWM duty cycle in 10ths of percent
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // use SMCLK
    TIMER_A_CTL_MC__UP | // in Up mode
    TIMER_A_CTL_CLR; // Clear TAR to start

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

/****| keypad_init | *****************************************
 * Brief: This function is to initialize all of the pins used in the
 * keypad
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void keypad_init (void)
{
    P4->SEL1 &= ~BIT0; // configure P4.0 as simple I/O
    P4->SEL0 &= ~BIT0;
    P4->DIR &= ~BIT0; // P4.0 set as output pin */
    P4->REN |= BIT0;
    P4->OUT |= BIT0;

    P4->SEL1 &= ~BIT1; // configure P4.1 as simple I/O
    P4->SEL0 &= ~BIT1;
    P4->DIR &= ~BIT1; // P4.1 set as output pin */
    P4->REN |= BIT1;
    P4->OUT |= BIT1;

    P4->SEL1 &= ~BIT2; // configure P4.2 as simple I/O
    P4->SEL0 &= ~BIT2;
    P4->DIR &= ~BIT2; // P4.2 set as output pin */
    P4->REN |= BIT2;
    P4->OUT |= BIT2;

    P4->SEL1 &= ~BIT3; // configure P4.3 as simple I/O
    P4->SEL0 &= ~BIT3;
    P4->DIR &= ~BIT3; // P4.3 set as output pin */
    P4->REN |= BIT3;
    P4->OUT |= BIT3;

    P4->SEL1 &= ~BIT4; // configure P4.4 as simple I/O
    P4->SEL0 &= ~BIT4;
    P4->DIR &= ~BIT4; // P4.4 set as output pin */


    P4->SEL1 &= ~BIT5; // configure P4.5 as simple I/O
    P4->SEL0 &= ~BIT5;
    P4->DIR &= ~BIT5; // P4.5 set as output pin */


    P4->SEL1 &= ~BIT6; // configure P4.6 as simple I/O
    P4->SEL0 &= ~BIT6;
    P4->DIR &= ~BIT6; // P4.6 set as output pin */

}


/****| Read_Keypad | *****************************************
 * Brief: This function is to read the keypad and assign a number based off the one
 * pressed to the global variable num.
 * param:
 * (void) data: N/A
 * return:
 * 1
 *************************************************************/
uint8_t Read_Keypad(void)
{
    uint8_t col, row;
    for ( col = 0; col < 3; col++ )
    {
        P4->DIR = 0x00; // Set Columns to inputs
        P4->DIR |= BIT ( 4+col ); // Set column 3 to output
        P4->OUT &= ~BIT ( 4+col ); // Set column 3 to LOW
        SysTick_delay (10); // Delay the while loop
        row = P4->IN & 0x0F; // read all rows
        while ( !(P4IN & BIT0) | !(P4IN & BIT1) | !( P4IN & BIT2) | !( P4IN & BIT3) );
        if (row != 0x0F) break; // if one of the input is low, some key is pressed.
    }
    P4->DIR = 0x00; // Set Columns to inputs
    if ( col == 3)
        return 0;
    if (row == 0x0E) num = col + 1; // key in row 0
    if (row == 0x0D) num = 3 + col +1; // key in row 1
    if (row == 0x0B) num = 6 + col +1; // key in row 2
    if (row == 0x07) num = 9 + col+1; // key in row 3

    return 1;
}

