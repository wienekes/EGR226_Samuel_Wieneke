/************************************************************************************
 * Author: Samuel Wieneke
 * Course: EGR 226 - 902
 * Date: 3/17/2021
 * Project: Lab 9
 * File:
 * Description: This program will allow the user to increment or decrement the LED using
 * pushbuttons
 *
 *
 ************************************************************************************/


#include "msp.h"
#include <stdio.h>

void SysTick_Init (void);
void SysTick_delay (uint16_t delay);
void pin_init (void);
void PORT1_IRQHandler (void);
void setupInterrupts (void);
void sevenSegment (uint32_t intervalCnt);


volatile uint32_t intervalCnt=0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init();
    pin_init();




    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
    __enable_interrupt();

    setupInterrupts();
    while (1)
    {

    }

}



/****| sevenSegment | *****************************************
 * Brief: This function is used for deciding which number will be displayed
 * on the seven segment LED
 * param: uint32_t intervalCnt
 * return:
 * N/A
 *************************************************************/
void sevenSegment (uint32_t intervalCnt)
{
    if(intervalCnt == 1)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 0
        P4->OUT |= BIT3; //a
        P4->OUT |= BIT4; //b
        P4->OUT |= BIT5; //c
        P4->OUT |= BIT6; //d
        P4->OUT |= BIT7; //e
        P5->OUT |= BIT4; //f
    }

    if(intervalCnt == 2)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 1
        P4->OUT |= BIT4; //b
        P4->OUT |= BIT5; //c
    }

    if(intervalCnt == 3)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 2
        P4->OUT |= BIT3; //a
        P4->OUT |= BIT4; //b
        P4->OUT |= BIT6; //d
        P4->OUT |= BIT7; //e
        P5->OUT |= BIT5; //g
    }

    if(intervalCnt == 4)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 3
        P4->OUT |= BIT3; //a
        P4->OUT |= BIT4; //b
        P4->OUT |= BIT5; //c
        P4->OUT |= BIT6; //d
        P5->OUT |= BIT5; //g
    }

    if(intervalCnt == 5)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 4
        P4->OUT |= BIT4; //b
        P4->OUT |= BIT5; //c
        P5->OUT |= BIT4; //f
        P5->OUT |= BIT5; //g
    }

    if(intervalCnt == 6)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 5
        P4->OUT |= BIT3; //a
        P4->OUT |= BIT5; //c
        P4->OUT |= BIT6; //d
        P5->OUT |= BIT4; //f
        P5->OUT |= BIT5; //g

    }

    if(intervalCnt == 7)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 6
        P4->OUT |= BIT3; //a
        P4->OUT |= BIT5; //c
        P4->OUT |= BIT6; //d
        P4->OUT |= BIT7; //e
        P5->OUT |= BIT4; //f
        P5->OUT |= BIT5; //g
    }

    if(intervalCnt == 8)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 7
        P4->OUT |= BIT3; //a
        P4->OUT |= BIT4; //b
        P4->OUT |= BIT5; //c
    }

    if(intervalCnt == 9)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 8
        P4->OUT |= BIT3; //a
        P4->OUT |= BIT4; //b
        P4->OUT |= BIT5; //c
        P4->OUT |= BIT6; //d
        P4->OUT |= BIT7; //e
        P5->OUT |= BIT4; //f
        P5->OUT |= BIT5; //g
    }

    if(intervalCnt == 10)
    {
        //clear all lights
        P4->OUT &= ~BIT3; //a
        P4->OUT &= ~BIT4; //b
        P4->OUT &= ~BIT5; //c
        P4->OUT &= ~BIT6; //d
        P4->OUT &= ~BIT7; //e
        P5->OUT &= ~BIT4; //f
        P5->OUT &= ~BIT5; //g

        //turn on lights for 9
        P4->OUT |= BIT3; //a
        P4->OUT |= BIT4; //b
        P4->OUT |= BIT5; //c
        P5->OUT |= BIT4; //f
        P5->OUT |= BIT5; //g
    }
}

/****| pin_init | *****************************************
 * Brief: This function is used for the initialization
 * of the pins for the 7 segment LED
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void pin_init (void)
{
    //a
    P4->SEL1 &= ~BIT3; // configure P4.3 as simple I/O
    P4->SEL0 &= ~BIT3;
    P4->DIR |= BIT3; // P4.3 set as output pin */

    //b
    P4->SEL1 &= ~BIT4; // configure P4.4 as simple I/O
    P4->SEL0 &= ~BIT4;
    P4->DIR |= BIT4; // P4.4 set as output pin */

    //c
    P4->SEL1 &= ~BIT5; // configure P4.5 as simple I/O
    P4->SEL0 &= ~BIT5;
    P4->DIR |= BIT5; // P4.5 set as output pin */

    //d
    P4->SEL1 &= ~BIT6; // configure P4.6 as simple I/O
    P4->SEL0 &= ~BIT6;
    P4->DIR |= BIT6; // P4.6 set as output pin */

    //e
    P4->SEL1 &= ~BIT7; // configure P4.7 as simple I/O
    P4->SEL0 &= ~BIT7;
    P4->DIR |= BIT7; // P4.7 set as output pin */

    //f
    P5->SEL1 &= ~BIT4; // configure P5.4 as simple I/O
    P5->SEL0 &= ~BIT4;
    P5->DIR |= BIT4; // P5.4 set as output pin */

    //g
    P5->SEL1 &= ~BIT5; // configure P5.5 as simple I/O
    P5->SEL0 &= ~BIT5;
    P5->DIR |= BIT5; // P5.5 set as output pin */

}

/****| setupInterrupts | *****************************************
 * Brief: This function is used for the initialization
 * of the buttons in interrupt form
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void setupInterrupts()
{
    // Blue
    P1->SEL1 &= ~BIT6; // configure P1.6 as simple I/O
    P1->SEL0 &= ~BIT6;
    P1->DIR &= ~BIT6; // P1.6 set as output pin */
    P1->REN |= BIT6;
    P1->OUT |= BIT6;
    P1->IES |= BIT6;
    P1->IE |= BIT6;
    P1->IFG &= ~BIT6;

    // Red
    P1->SEL1 &= ~BIT7; // configure P1.7 as simple I/O
    P1->SEL0 &= ~BIT7;
    P1->DIR &= ~BIT7; // P1.7 set as output pin */
    P1->REN |= BIT7;
    P1->OUT |= BIT7;
    P1->IES |= BIT7;
    P1->IE |= BIT7;
    P1->IFG &= ~BIT7;


}

/****| PORT1_IRQHandler | *****************************************
 * Brief: the handler for the port 1 interrupts
 *
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void PORT1_IRQHandler ()
{
    if(P1->IFG & BIT6)
    {
        intervalCnt++;
        if(intervalCnt ==11)
        {
            intervalCnt =1;
        }
        sevenSegment(intervalCnt);
        SysTick_delay(250);
    }

    P1->IFG &= ~BIT6;

    if(P1->IFG & BIT7)
    {
        intervalCnt--;
        if(intervalCnt == 0)
        {
            intervalCnt = 10;
        }
        sevenSegment(intervalCnt);
        SysTick_delay(250);

    }
    P1->IFG &= ~BIT7;


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
    SysTick->LOAD = 3000000; //  1 sec reload value
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
