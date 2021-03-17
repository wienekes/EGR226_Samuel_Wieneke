#include "msp.h"
#include <stdio.h>


//uint8_t Read_Keypad(void);
void SysTick_Init (void);
void SysTick_delay (uint16_t delay);
void DC_Motor (void);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    DC_Motor();
    int duty_cycle=0, timeON, timeOFF, x;
    int period = 25;

    duty_cycle = x;

    timeON = duty_cycle;
    timeOFF = period - duty_cycle;
    while(1)
    {
       P2->OUT |= BIT4;
       SysTick_delay(timeON);
       P2->OUT &= BIT4;
       SysTick_delay(timeOFF);
    }

}

void DC_Motor (void)
{
    //P2.4 is connected to DC Motor
    P2->SEL1 &= ~BIT4; // configure P2.4 as simple I/O
    P2->SEL0 &= ~BIT4;
    P2->DIR |= BIT4; // P2.4 set as output pin */

    //P2->OUT |= BIT4;
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



/*
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
*/
