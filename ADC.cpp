#include "ADC.h"

/*!
 * @brief  ADC constructor
 */
__ADC__::__ADC__()
{
    /* Empty */
}

/*!
 * @brief  ADC destructor
 */
__ADC__::~__ADC__()
{
    /* Empty */
}

/*!
 * @brief  Begins ADC implementation
 * @param  samples
 *         The amount of samples used to calculate the arithmetic average in read() function
 * @return Returns 0 if the begin() function was called more than once before the end() function, otherwise returns 1
 */
const uint8_t __ADC__::begin(const uint8_t samples)
{
    if (this->began)
        return (0);
    this->began = 1;

    
    this->setSamples(samples);                 // Set the sample rate
    this->setReference(ADC_DEFAULT_REFERENCE); // Set default ADC voltage reference
    this->setPrescaler(ADC_DEFAULT_PRESCALER); // Set default ADC frequency prescaler

    #if defined (__AVR_ATmega328P__) || \
        defined (__AVR_ATmega328PB__)
    ADCSRA = ADCSRA | (1 << ADEN); // Enable ADC
    #endif
}

/*!
 * @brief  Begins ADC implementation
 */
const uint8_t __ADC__::begin(void)
{
    return (this->begin(ADC_DEFAULT_SAMPLES));
}

/*!
 * @brief  Sets the sample rate used to calculate the arithmetic average of function read()
 */
void __ADC__::setSamples(const uint8_t samples)
{
    this->samples = samples;
}

/*!
 * @brief  Sets the ADC reference voltage source
 * @param  reference
 *         The ADC reference voltage source
 */
void __ADC__::setReference(const uint8_t reference)
{
    switch (reference)
    {
        #if defined (__AVR_ATmega328P__) || \
            defined (__AVR_ATmega328PB__)
        case ADC_REFERENCE_AREF:
            ADMUX = ADMUX & ~((1 << REFS1) | (1 << REFS0));
            break;
        case ADC_REFERENCE_AVCC:
            ADMUX = (ADMUX & ~(1 << REFS1)) | (1 << REFS0);
            break;
        case ADC_REFERENCE_INTERNAL:
            ADMUX = ADMUX | (1 << REFS1) | (1 << REFS0);
            break;
        #endif
        default:
            break;
    }
}

/*!
 * @brief  Sets the ADC frequency prescaler
 * @param  reference
 *         The ADC frequency prescaler
 */
void __ADC__::setPrescaler(const uint8_t prescaler)
{
    switch (prescaler)
    {
        #if defined (__AVR_ATmega328P__) || \
            defined (__AVR_ATmega328PB__)
        case 2:
            ADCSRA = (ADCSRA & ~((1 << ADPS2) | (1 << ADPS1))) | (1 << ADPS0);
            break;
        case 4:
            ADCSRA = ((ADCSRA & ~(1 << ADPS2)) | (1 << ADPS1)) & ~(1 << ADPS0);
            break;
        case 8:
            ADCSRA = (ADCSRA & ~(1 << ADPS2)) | ((1 << ADPS1) | (1 << ADPS0));
            break;
        case 16:
            ADCSRA = (ADCSRA | (1 << ADPS2)) & ~((1 << ADPS1) | (1 << ADPS0));
            break;
        case 32:
            ADCSRA = ((ADCSRA | (1 << ADPS2)) & ~(1 << ADPS1)) | (1 << ADPS0);
            break;
        case 64:
            ADCSRA = (ADCSRA | ((1 << ADPS2) | (1 << ADPS1))) & ~(1 << ADPS0);
            break;
        case 128:
            ADCSRA = ADCSRA | ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
            break;
        #endif
        default:
            break;
    }
    
}

/*!
 * @brief  Reads an ADC channel
 * @param  channel
 *         The ADC channel to be read
 * @return Returns the 10 bit value of the ADC channel
 */
const uint16_t __ADC__::read(const uint8_t channel)
{
    uint32_t result = 0;

    #if defined (__AVR_ATmega328P__) || \
        defined (__AVR_ATmega328PB__)
    ADMUX = (ADMUX & 0xF0) | (channel & 0x07); // Set the channel
    #endif

    for (uint8_t sample = 0; sample < this->samples; sample++)
    {
        #if defined (__AVR_ATmega328P__) || \
            defined (__AVR_ATmega328PB__)
        ADCSRA = ADCSRA | (1 << ADSC);  // Start the conversion
        while(!(ADCSRA & (1 << ADIF))); // Wait until conversion has ended
        result += ADC;
        #endif
    }
    return (result / this->samples); /* Return result */
}

/*!
 * @brief  Reads the bandgap reference voltage of MCU using a specialised ADC channel connected to that
 * @return The bandgap reference voltage of MCU in millivolts
 */
uint16_t __ADC__::VCC(void)
{
    uint32_t result = 0;

    #if defined (__AVR_ATmega328P__) || \
        defined (__AVR_ATmega328PB__)
    ADMUX = (ADMUX & 0xF0) | ((1 << MUX3) | (1 << MUX2) | (1 << MUX1)); /* Set V(BG) (Voltage Bandgap) */
    #endif

    for (uint8_t sample = 0; sample < this->samples; sample++)
    {
        #if defined (__AVR_ATmega328P__) || \
            defined (__AVR_ATmega328PB__)
        ADCSRA = ADCSRA | (1 << ADSC);  // Start the conversion
        while(!(ADCSRA & (1 << ADIF))); // Wait until conversion has ended
        result += ADC;
        #endif
    }

    result /= this->samples;
    #if defined (__AVR_ATmega328P__) || \
        defined (__AVR_ATmega328PB__)
    result = 1125300L / result; // Divide result by precalculated value */
    #endif
    return (result);
}

/*!
 * @brief  Ends the ADC implementation
 */
void __ADC__::end(void)
{
    if (!this->began)
        return;
    this->began = 0;

    this->setSamples(0);
    #if defined (__AVR_ATmega328P__) || \
        defined (__AVR_ATmega328PB__)
    ADCSRA = ADCSRA & ~(1 << ADEN); // Disable ADC
    #endif
}

#if defined (HAS_ADC)
__ADC__ A2D = __ADC__();
#endif
