#include "msp.h"
#include <stdio.h>

void adcsetup(void);
void SysTick_Init (void);
void SysTick_delay (uint16_t delay);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    SysTick_Init();
    adcsetup();
    fflush(stdin);

    int result;
    float nADC, temp;

    while(1)
    {
        ADC14->CTL0 |=1; //start a conversion
        while(!ADC14->IFGR0); // wait until conversion complete
        result = ADC14->MEM[5]; // immediately store value in a variable
        nADC = (result*3.3) / 16384;
        printf ("Voltage: %f\n", nADC);
        nADC = nADC*1000;
        temp = (nADC-500)/10;
        printf("Temperature in Celcius: %f\n", temp);
        SysTick_delay(1000); // wait for next value- using Systick function
    }

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

void SysTick_delay (uint16_t delay)
{ // Systick delay function
    SysTick->LOAD = ((delay * 3000) - 1); //delay for 1 msecond per delay value
    SysTick->VAL = 0; // any write to CVR clears it
    while ( (SysTick->CTRL & 0x00010000) == 0); // wait for flag to be SET
}
