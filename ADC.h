#ifndef __ADC_H__
#define __ADC_H__

/* Dependecies */
#include "Core\Core.h"

#if defined (__AVR_ATmega328P__)
    #define ADC_CHANNELS (uint8_t)7
    #define AREF         (uint8_t)0
    #define AVCC         (uint8_t)1
    #define INTERNAL     (uint8_t)2
#endif

class __ADC__
{
    public:
        __ADC__(volatile uint16_t* adcr, volatile uint8_t* adcsra,\
                volatile uint8_t* adcsrb, volatile uint8_t* admux);
        ~__ADC__();
        void     begin        (void);
        void     setPrescaler (uint8_t prescaler);
        void     setReference (uint8_t reference);
        void     setSampleRate(uint8_t sampleRate);
        uint16_t read         (uint8_t channel);
        uint16_t VCC          (void);
        void     end          (void);
    private:
        uint8_t sampleRate;
        volatile uint16_t* adcr;
        volatile uint8_t* adcsra;
        volatile uint8_t* adcsrb;
        volatile uint8_t* admux;
};

#if defined(__AVR_ATmega328P__)
extern __ADC__ ADC;
#else
#error "No ADC implementation"
#endif

#endif
