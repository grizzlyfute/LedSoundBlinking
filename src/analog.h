#ifndef ANALOG_H
#define ANALOG_H

#include <stdint.h>

extern volatile uint16_t analogBuf[];
extern volatile uint8_t analogSize;
extern uint8_t analogCurrentChannel;

#define ANALOG_STOP() ADCSRA &= ~(1 << ADIE);
#define ANALOG_START() ADCSRA |= (1 << ADIE);

// Analog work on free running mode. Filling analogWorkinBuf until analogWorkingSize reach SAMPLE_RATE

void analogInit(void);
void analogChangeChannel(int8_t inc);
void analogResetBuf(void);
uint8_t analogIsBufReady(void);

#endif // ANALOG_H
