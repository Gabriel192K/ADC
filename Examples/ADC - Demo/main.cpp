/*
 * ADC - Demo.cpp
 *
 * Created: 1/9/2024 11:46:27 AM
 * Author : rotes
 */ 

/* Dependencies */
#include "ADC/ADC.h"

/* Macros */
#define ADC_CHANNEL (const uint8_t)0

int main(void)
{
    A2D.begin();
    while (1)
    {
        const uint16_t value = A2D.read(ADC_CHANNEL);
        // Use value ...
    }
}

