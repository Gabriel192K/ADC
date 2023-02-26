/*
 * Demo.cpp
 *
 * Created: 2/23/2023 5:13:53 PM
 * Author : Gabriel
 */ 

#include "ADC\ADC.h"
#include "USART\USART.h"
#include "Time\Time.h"

int main(void)
{
    USART.begin(9600);
    Time.begin();
    USART.printP(PSTR("ADC Demo Code\n\n"));
    ADC.begin();
    while (1) 
    {
        for (uint8_t i = 0; i < 7; i++)
        {
            USART.printf("ADC[%hhu] is: %d\n", i, ADC.read(i));
        }
        USART.printf("VCC is: %dmV\n", ADC.VCC());
        USART.println();
        Time.delayMillis(1000);
    }
}

