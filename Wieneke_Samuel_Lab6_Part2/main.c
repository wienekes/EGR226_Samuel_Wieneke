#include "msp.h"
#include <stdio.h>



    void SysTick_Init (void);
    void SysTick_delay (uint16_t delay);
    uint8_t Read_Keypad(void);
    void keypad_init (void);




    uint8_t pressed, num;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    keypad_init ();
    SysTick_Init();

    fflush(stdin);
    uint8_t pinArray[4];
    int i=0;



    printf("Enter a 4 digit pin, then press # to confirm\n");
    while (1)
    {

        pressed = Read_Keypad ();
        if (pressed)
        {
            if(num == 10)
            {
                printf("*\n");
            }
            else if(num == 11)
            {
                num = 0;
                printf("%d\n",num);
            }
            else if(num == 12)
            {
                printf("#\n");
            }
            else
            {
                printf("%d\n", num);
            }
            SysTick_delay (100);

            if(num != 10 && num !=12)
            {
                if (i==4)
                {
                    pinArray[0] = pinArray[1];
                    pinArray[1] = pinArray[2];
                    pinArray[2] = pinArray[3];
                    pinArray[3] = num;
                }

                if (i<4)
                {
                    pinArray[i] = num;
                    i++;
                }

            }
            if(num == 12 && i==4)
            {
                printf("Pin Entered: %d %d %d %d\n", pinArray[0], pinArray[1], pinArray[2], pinArray[3]);
                4-i;
            }
            if(num == 12 && i<4)
            {
                printf("Please enter more digits, then press #\n");
                4-i;
            }

        }
    }


}



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


