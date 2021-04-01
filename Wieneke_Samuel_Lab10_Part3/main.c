#include "msp.h"
#include <stdio.h>
#include <string.h>

void adcsetup(void);
void button_setup(void);
void SysTick_Init (void);
void SysTick_delay (uint16_t delay);
void LCD_init(void);
void commandWrite(uint8_t command);
void pushNibble(uint8_t data, uint8_t control);
void dataWrite(uint8_t data);
void delay_micro(uint32_t microsecond);
void delay_milli(uint32_t millisecond);
void lcdSetText(char* text, int x, int y);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init();
    adcsetup();
    LCD_init();
    button_setup();

    fflush(stdin);

    int result, i=0, j=2;
    float nADC, temp, temp1;

    //char currentTemp[15]={'C', 'U', 'R', 'R', 'E', 'N', 'T', ' ', 'T', 'E', 'M', 'P', ' ', 'I', 'S'};
    //char test[17] = "CURRENT TEMP IS\0";
    char charTemp[4];


    while(1)
    {



        while((P3->IN & BIT7) != BIT7)
        {
            j++;
            SysTick_delay(500);
        }
        if(j%2 == 0)
        {
            commandWrite(1); // Clear display
            commandWrite(0x80); // Setting cursor to 1st spot on line 1


            lcdSetText("Current Temp is", 0,0);


            ADC14->CTL0 |=1; //start a conversion
            while(!ADC14->IFGR0); // wait until conversion complete
            result = ADC14->MEM[5]; // immediately store value in a variable
            nADC = (result*3.3) / 16384;
            //printf ("Voltage: %f\n", nADC);
            nADC = nADC*1000;
            temp = (nADC-500)/10;

            commandWrite(0xC4); //sets cursor to 5th spot on line 2
            sprintf(charTemp, "%.1f", temp);

            for(i = 0; i < strlen(charTemp) ; i++)
            {
                dataWrite(charTemp[i]);
            }

            dataWrite(0xDF);
            lcdSetText("C", 9,1);

            SysTick_delay(1000); // wait for next value- using Systick function
        }

        if(j%2 == 1)
        {

            commandWrite(1); // Clear display
            commandWrite(0x80); // Setting cursor to 1st spot on line 1



            lcdSetText("Current Temp is", 0,0);

            ADC14->CTL0 |=1; //start a conversion
            while(!ADC14->IFGR0); // wait until conversion complete
            result = ADC14->MEM[5]; // immediately store value in a variable
            nADC = (result*3.3) / 16384;
            nADC = nADC*1000;
            temp = (nADC-500)/10;
            temp1 = ((temp * (9.0/5.0)) + 32.0);

            commandWrite(0xC4); //sets cursor to 5th spot on line 2
            sprintf(charTemp, "%.1f", temp1);


            for(i = 0; i < strlen(charTemp) ; i++)
            {
                dataWrite(charTemp[i]);
            }

            dataWrite(0xDF);
            lcdSetText("F", 9,1);

            SysTick_delay(1000); // wait for next value- using Systick function

        }






    }

}

void button_setup(void)
{
    P3->SEL1 &= ~BIT7; // configure P3.7 as simple I/O
    P3->SEL0 &= ~BIT7;
    P3->DIR &= ~BIT7; // P3.7 set as output pin */
    P3->REN |= BIT7;
    P3->OUT |= BIT7;
}

void adcsetup(void)
{
    ADC14->CTL0 = 0x00000010; //power on and disabled during configuration
    ADC14->CTL0 |= 0x04D80300; // S/H pulse mode, MCLCK, 32 sample clocks, sw trigger, /4 clock divide
    ADC14->CTL1 = 0x00000030; // 14-bit resolution
    ADC14->MCTL[5] = 1; // A0 input, single ended, vref=avcc
    P5->SEL1 |= BIT4; // configure pin 5.4 for AO
    P5->SEL0 |= BIT4; //0b 0100 0000
    ADC14->CTL1 |= 0x00050000; //start converting at mem reg 5
    ADC14->CTL0 |= 2; //enable ADC after configuration
}

void SysTick_Init (void)
{ //initialization of systic timer
    SysTick->CTRL = 0; // disable SysTick During step
    SysTick->LOAD = 0x00FFFFFF; // max reload value
    SysTick->VAL = 0; // any write to current clears it
    SysTick->CTRL = 0x00000005; // enable systic, 3MHz, No Interrupts
}

/// ****| lcdSetText | ******************************//*
/// * Brief: Display character string on the LCD at the
/// *          chosen coordinates.
/// * param:
/// *      (char*) text: character string to display
/// *      (int) x:      x-coordinate
/// *      (int) y:      y-coordinate
/// * return:
/// *      N/A
/// ****************************************************/
void lcdSetText(char* text, int x, int y) {
    int i;
    if (x < 16) {
        x |= 0x80;      // Set LCD for first line write
        switch (y){
        case 0:
            x |= 0x00;  // Set LCD for first line write
            break;
        case 1:
            x |= 0x40;  // Set LCD for Second line write
            break;
        case 2:
            x |= 0x10;  // Set LCD for Third line write
            break;
        case 3:
            x |= 0x50;  // Set LCD for Fourth line write
            break;
        case 5:
            x |= 0x20;  // Set LCD for second line write reverse
            break;
        }
        commandWrite(x);
    }
    i = 0;
    while (text[i] != '\0') {
        dataWrite(text[i]);
        i++;
    }
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

