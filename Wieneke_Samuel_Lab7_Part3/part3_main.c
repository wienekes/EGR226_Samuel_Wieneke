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
#include <string.h>



void SysTick_Init(void);
void LCD_init(void);
void SysTick_delay(uint16_t delay);
void commandWrite(uint8_t command);
void pushNibble(uint8_t data, uint8_t control);
void dataWrite(uint8_t data);
void delay_micro(uint32_t microsecond);
void delay_milli(uint32_t millisecond);

int main(void)
{
    SysTick_Init();
    LCD_init();


    char firstline[16]={'L', 'A', 'B', 'O', 'R', 'A', 'T', 'O', 'R', 'Y', ' ', 'O', 'V', 'E', 'R'};

    int i,j;

    commandWrite(1); // Clear display
    commandWrite(0x80); // Setting cursor to 1st spot on line 1
    delay_milli(4);


    for(i = 0; i < strlen(firstline) ; i++)
    {
        dataWrite(firstline[i]);
        delay_milli(4);
    }

    commandWrite(0x0C); //turns off cursor
    delay_milli(500); //delay for half second

    while(1)
    {
        for (i=0; i<strlen(firstline); i++) //shifts the string left
        {
            commandWrite(0x18);
            delay_milli(500); //half second delay between each shift

            if(i==7) //there is 7 spaces needed so it doesn't carry over onto line 3
            {
                commandWrite(0x80);
                delay_milli(10);
                dataWrite(' ');
                dataWrite(' ');
                dataWrite(' ');
                dataWrite(' ');
                dataWrite(' ');
                dataWrite(' ');
                dataWrite(' ');
                delay_milli(4);
            }
        }

        commandWrite(0x01); //clears LCD
        delay_milli(100);

        for(j=0; j<strlen(firstline); j++)
        {
            commandWrite(0x1C);
            delay_milli(4);
        }

        for(j=0; j<strlen(firstline); j++)
        {
            dataWrite(firstline[j]);
            delay_milli(500); //half second delay between shifting letters
            commandWrite(0x18);
            delay_milli(4);
        }
        delay_milli(500);
    }





}
/****| SysTick_Init | *****************************************
 * Brief: This function is used for the initialization
 * of the SysTick timer
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void SysTick_Init(void)
{
    SysTick -> CTRL = 0; // Disable SysTick
    SysTick -> LOAD = 0x00FFFFFF; // Max reload value
    SysTick -> VAL = 0; // Any write to current clears it
    SysTick -> CTRL = 0x00000005; // Enable systick, 3MHz, no interrupts
}


/****| LCD_init | *****************************************
 * Brief: This function is used for the initialization
 * of the LCD
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/


void LCD_init(void)
{
    // Setting pins 4.0, 4.2, 4.4-4.7 to output
    P4->DIR |= 0xF5;  //0b 1111 0101

    commandWrite(3); //reset sequence
    delay_milli(20);

    commandWrite(3);
    delay_micro(200);

    commandWrite(3);
    delay_milli(20);

    commandWrite(2); //setting 4 bit mode
    delay_micro(100);

    commandWrite(0x28); //4 limes 5x7 format
    delay_milli(100);

    delay_micro(10);
    commandWrite(0x0F); //display on, cursor on, blinking
    delay_micro(100);

    commandWrite(0x01); //clear display
    delay_micro(120);


    commandWrite(0x06); //increment cursor
    delay_micro(100);

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
void SysTick_delay(uint16_t delay)
{
    SysTick->LOAD = ((delay * 3000) - 1); // Delay 1 msecond per delay value
    SysTick->VAL = 0; // Any write to CVR clears it
    while ((SysTick->CTRL & 0x00010000) == 0); // Wait for flag to be SET
}

/****| pushNibble | *****************************************
 * Brief: This function pushes 1 nibble onto the data pins and
 * pulses the enable pin
 * param:
 * (uint8_t) data & control: two integers worth of data used to assign bits
 * to data pins, tutor helped with this and edited to fit code in class because code given did not work for whatever reason.
 * return:
 * N/A
 *************************************************************/

void pushNibble(uint8_t data, uint8_t control)
{
    P4->OUT &= ~0x10; // Set RS = 0
    data &= 0xF0; // Assign most significant 4 bits to data pins
    control &= 0x0F; // Assign least significant 4 bits to data pins
    P4->OUT = data | control | 4; // Pulse enable pin
    SysTick_delay(1);
    P4->OUT = data; // Clear enable pin
    P4->OUT = 0;
}

/****| commandWrite | *****************************************
 * Brief: This function writes one byte of command by calling
 * the pushNibble() function twice with the command parameter
 * param:
 * (uint8_t) data: one integer worth of data that gives a
 * command to the LCD
 * return:
 * N/A
 *************************************************************/

void commandWrite(uint8_t command)
{
    pushNibble(command & 0xF0, 0); // Sending upper nibble
    pushNibble(command << 4, 0); // Sending lower nibble

    delay_milli(4);
}


/****| dataWrite | *****************************************
 * Brief: This function writes one byte of data by calling
 * the pushNibble() function twice with the data parameter
 * param:
 * (void) data: one unsigned char worth of data to be sent
 * to the LCD
 * return:
 * N/A
 *************************************************************/
void dataWrite(unsigned char data)
{
    pushNibble(data & 0xF0, 1); // Sending upper nibble
    pushNibble(data << 4, 1); // Sending lower nibble
    SysTick_delay(1);
}

/****| delay_milli | *****************************************
 * Brief: This function uses Systick to get a millisecond delay
 *
 * param:
 * (uint32_t) millisecond: one integer worth of data input that
 * determines the duration of the delay
 * return:
 * N/A
 *************************************************************/
void delay_milli(uint32_t millisecond)
{
    SysTick -> LOAD = (millisecond*3000 - 1); // delay*3000
    SysTick -> VAL = 0; //clears counter
    while((SysTick -> CTRL & 0x00010000) == 0);
}

/****| delay_micro | *****************************************
 * Brief: This function uses Systick to get a microsecond delay
 *
 * param:
 * (uint32_t) microsecond: one integer worth of data input that
 * determines the duration of the delay
 * return:
 * N/A
 *************************************************************/
void delay_micro(uint32_t microsecond)
{
    SysTick -> LOAD = (microsecond*3 - 1); // delay*3
    SysTick -> VAL = 0; //clears counter
    while((SysTick -> CTRL & 0x00010000) == 0);
}
