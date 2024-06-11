#ifndef __ADC_H__
#define __ADC_H__

/* Dependecies */
#include <avr/io.h> /* Memory */

/* Macros */
#define ADC_REFERENCE_AREF     (const uint8_t)0
#define ADC_REFERENCE_AVCC     (const uint8_t)1
#define ADC_REFERENCE_INTERNAL (const uint8_t)2
#define ADC_DEFAULT_SAMPLES    (const uint8_t)10
#define ADC_DEFAULT_REFERENCE  ADC_REFERENCE_AVCC
#define ADC_DEFAULT_PRESCALER  (const uint8_t)128

class __ADC__
{
    public:
        __ADC__();
        ~__ADC__();
        const uint8_t  begin        (const uint8_t samples);
        const uint8_t  begin        (void);
        void           setSamples   (const uint8_t samples);
        void           setReference (const uint8_t reference);
        void           setPrescaler (const uint8_t prescaler);
        const uint16_t read         (const uint8_t channel);
        const uint16_t VCC          (void);
        void           end          (void);
    private:
        uint8_t began;
        uint8_t samples;
};

#if defined (__AVR_ATmega328P__) || \
    defined (__AVR_ATmega328PB__)
#define HAS_ADC
extern __ADC__ A2D;
#endif

#endif
