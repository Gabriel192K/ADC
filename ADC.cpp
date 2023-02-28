#include "ADC.h"

/*********************************************
Function: __ADC__()
Purpose:  Constructor to __ADC__ class
Input:    ADC registers
Return:   None
*********************************************/
__ADC__::__ADC__(volatile uint16_t* adcr, volatile uint8_t* adcsra,\
                 volatile uint8_t* adcsrb, volatile uint8_t* admux)
{
    this->adcr = adcr; this->adcsra = adcsra;
    this->adcsrb = adcsrb; this->admux = admux;
}

/*********************************************
Function: ~__ADC__()
Purpose:  Destructor to ~__ADC__ class
Input:    None
Return:   None
*********************************************/
__ADC__::~__ADC__()
{
    /* Empty */
}

/*********************************************
Function: begin()
Purpose:  Start ADC module
Input:    None
Return:   None
*********************************************/
void __ADC__::begin(void)
{
    this->setReference(AVCC); /* Set default reference */
    this->setPrescaler(128);  /* Set default frequency prescaler */
    this->setSampleRate(100); /* Set default sample rate */

    #if defined (__AVR_ATmega328P__)
    *this->adcsra |= (1 << ADEN); /* Enable ADC */
    #endif
}

/*********************************************
Function: setPrescaler()
Purpose:  Set ADC frequency prescaler
Input:    Prescaler
Return:   None
*********************************************/
void __ADC__::setPrescaler(uint8_t prescaler)
{
    #if defined (__AVR_ATmega328P__)
    *this->adcsra &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); /* Clear all prescaler bits */
    switch (prescaler)
    {
        case 2  : *this->adcsra |= (1 << ADPS0);                               break;
        case 4  : *this->adcsra |= (1 << ADPS1);                               break;
        case 8  : *this->adcsra |= (1 << ADPS1) | (1 << ADPS0);                break;
        case 16 : *this->adcsra |= (1 << ADPS2);                               break;
        case 32 : *this->adcsra |= (1 << ADPS2) | (1 << ADPS0);                break;
        case 64 : *this->adcsra |= (1 << ADPS2) | (1 << ADPS1);                break;
        case 128: *this->adcsra |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); break;
        default :                                                              break;
    }
    #endif
}

/*********************************************
Function: setReference()
Purpose:  Set ADC reference voltage
Input:    Reference
Return:   None
*********************************************/
void __ADC__::setReference(uint8_t reference)
{
    #if defined (__AVR_ATmega328P__)
    *this->admux &= ~((1 << REFS1) | (1 << REFS0)); /* Clear all reference bits */
    switch (reference)
    {
        case AREF    :                                              break; /* No bit setting needed */
        case AVCC    : *this->admux |= (1 << REFS0);                break;
        case INTERNAL: *this->admux |= (1 << REFS1) | (1 << REFS0); break;
        default      :                                              break;
    }
    #endif
}

/*********************************************
Function: setSampleRate()
Purpose:  Set ADC sample rate
Input:    Sample rate
Return:   None
*********************************************/
void __ADC__::setSampleRate(uint8_t sampleRate)
{
    this->sampleRate = sampleRate;
}

/*********************************************
Function: read()
Purpose:  Read ADC channel
Input:    Channel
Return:   Value of the channel
*********************************************/
uint16_t __ADC__::read(uint8_t channel)
{
    uint32_t result = 0;                  /* Variable to store result */
    channel %= ADC_CHANNELS;              /* Select channel within range of available channels */
    #if defined (__AVR_ATmega328P__)
    *this->admux = (*this->admux & 0xF0) | channel; /* Set channel */
    #endif

    for (uint8_t sample = 0; sample < this->sampleRate; sample++)
    {
        #if defined (__AVR_ATmega328P__)
        *this->adcsra |= (1 << ADSC);          /* Start conversion */
        while(!(*this->adcsra & (1 << ADIF))); /* Wait until conversion has ended */
        result += *this->adcr;                 /* Sum samples */
        #endif
    }

    return (result / this->sampleRate); /* Return result */
}

/*********************************************
Function: VCC()
Purpose:  Read VCC of the ADC
Input:    None
Return:   Value of VCC in mV
*********************************************/
uint16_t __ADC__::VCC(void)
{
    uint32_t result = 0;
    #if defined (__AVR_ATmega328P__)
    *this->admux = (*this->admux & 0xF0) | ((1 << MUX3) | (1 << MUX2) | (1 << MUX1)); /* Set V(BG) (Voltage Bandgap) */
    #endif

    for (uint8_t sample = 0; sample < this->sampleRate; sample++)
    {
        #if defined (__AVR_ATmega328P__)
        *this->adcsra |= (1 << ADSC);          /* Start conversion */
        while(!(*this->adcsra & (1 << ADIF))); /* Wait until conversion has ended */
        result += *this->adcr;                 /* Sum samples */
        #endif
    }

    result /= this->sampleRate;
    #if defined (__AVR_ATmega328P__)
    result = 1125300L / result;  /* Divide result by precalculated value */
    #endif
    return (result); /* Return result in mV */
}

void __ADC__::end(void)
{
    #if defined (__AVR_ATmega328P__)
        *this->adcsra &= ~(1 << ADEN); /* Enable ADC */
    #endif
}

#if defined(__AVR_ATmega328P__)
__ADC__ ADC = __ADC__(&ADCR, &ADCSRA, &ADCSRB, &ADMUX);
#endif
