/************************************************************************************
* Author: Samuel Wieneke
* Course: EGR 226 - 902
* Date: 3/17/2021
* Project: Lab 7
* File:
* Description: This program will display various characters to the LCD
*
*
*
************************************************************************************/

#include "msp.h"
#include <stdio.h>


    void LCD_pins(void);
    void SysTick_Init (void);
    void SysTick_delay (uint16_t delay);
    //uint8_t Read_Keypad(void);
    void LCD_init (void);
    void delay_micro(uint32_t microsecond);
    void delay_milli(uint32_t millisecond);
    void PulseEnablePin (void);
    void pushNibble (uint8_t nibble);
    void pushByte (uint8_t byte);
    void commandWrite(uint8_t command);
    void dataWrite(uint8_t data);




void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    LCD_pins();
    SysTick_Init();
    LCD_init();



    while(1)
    {

    }


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

void LCD_pins(void)
{
    //RS
    P4->SEL1 &= ~BIT0; // configure P4.0 as simple I/O
    P4->SEL0 &= ~BIT0;
    P4->DIR |= BIT0; // P4.0 set as output pin */


    //E
    P4->SEL1 &= ~BIT2; // configure P2.4 as simple I/O
    P4->SEL0 &= ~BIT2;
    P4->DIR |= BIT2; // P4.2 set as output pin */


    //DB4
    P4->SEL1 &= ~BIT4; // configure P4.4 as simple I/O
    P4->SEL0 &= ~BIT4;
    P4->DIR |= BIT4; // P4.4 set as output pin */


    //DB5
    P4->SEL1 &= ~BIT5; // configure P4.5 as simple I/O
    P4->SEL0 &= ~BIT5;
    P4->DIR |= BIT5; // P4.5 set as output pin */


    //DB6
    P4->SEL1 &= ~BIT6; // configure P4.6 as simple I/O
    P4->SEL0 &= ~BIT6;
    P4->DIR |= BIT6; // P4.6 set as output pin */

    //DB7
    P4->SEL1 &= ~BIT7; // configure P4.7 as simple I/O
    P4->SEL0 &= ~BIT7;
    P4->DIR |= BIT7; // P4.7 set as output pin */

}
void LCD_init (void)
{
    commandWrite(3);
    delay_milli(20);

    commandWrite(3);
    delay_micro(200);

    commandWrite(3);
    delay_milli(20);

    commandWrite(2);
    delay_micro(100);

    commandWrite(0x28);
    delay_milli(100);

    delay_micro(10);
    commandWrite(0x0F);
    delay_micro(100);

    commandWrite(0x01);
    delay_micro(120);


    commandWrite(0x06);
    delay_micro(100);







}
void delay_milli(uint32_t millisecond)
{
    SysTick -> LOAD = (millisecond*3000 - 1); // delay*3000
    SysTick -> VAL = 0; //clears counter
    while((SysTick -> CTRL & 0x00010000) == 0);
}

void delay_micro(uint32_t microsecond)
{
    SysTick -> LOAD = (microsecond*3 - 1); // delay*3
    SysTick -> VAL = 0; //clears counter
    while((SysTick -> CTRL & 0x00010000) == 0);
}

void PulseEnablePin (void)
{
    //E is P4.2
    P4OUT &=~BIT2; // make sure pulse starts out at 0V
    delay_micro(10);
    P4OUT |=BIT2;
    delay_micro(10);
    P4OUT &=~BIT2;
    delay_micro(10);
}

void pushNibble (uint8_t nibble)
{
    P4OUT &= ~0xF0; // clear P4.4-P4.7
    P4OUT |= (nibble & 0x0F) << 4; // port pins P4.4 - P4.7 wired to D4 - D7
    PulseEnablePin();
}

void pushByte (uint8_t byte)
{
    uint8_t nibble;

    nibble = (byte & 0xF0) >> 4;
    pushNibble(nibble);
    nibble = byte & 0x0F;
    pushNibble(nibble);
    delay_micro(100);
}

void commandWrite(uint8_t command)
{
    P4OUT &= ~BIT0;

    pushByte(command & 0xF0);
    pushByte(command & 0x0F);
    delay_milli(4);
}

void dataWrite(uint8_t data)
{
    P4OUT |= ~BIT0;

    pushByte(data & 0xF0);
    pushByte(data & 0x0F);
    delay_milli(4);
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


