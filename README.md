# ADC
ADC Implementation for AVR platform

## Key features
- polling method used
- hardware oriented ADC implementation (already specified usable channels in case of ATmega328P you can use all channels but 2 channels are shared with the TWI bus so keep that in mind)
- sample rate, reference and prescaler configurable
- capable of high accuracy VCC reading of the chip
- begin and end functions for easy on and off operations

## Tested on
- ATmega328P
