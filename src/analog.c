#include "analog.h"
#include "constants.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint16_t analogBuf[SAMPLE_SIZE];
volatile uint8_t analogSize;
uint8_t analogCurrentChannel;

// fmax = 2093kHz, sample = 64.
// Do0 = 32.7 Hz = freq_fondamental
// ==> sample duration = 1/32.7 = 30.58 ms
// ==> sample rate = 64 => sample freq = 1/(0.003058/64) = 2092.8 KHz
// Shanon-nyquist: fechantillonage = 2*freqMax / sample = 65.4 Hz
void analogInit(void)
{
	// ADC Control Status Register A
	ADCSRA =
		(1 << ADEN) | // ADC Enable
		(1 << ADSC) | // Start first convertion
		(1 << ADATE)| // ADC Auto trigger enable
		(1 << ADIF) | // Clear Interrupt flags
		(1 << ADIE) | // ADC interrupt enable
		(1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // 64 prescalar
	ADCSRB =
		(0 << BIN)  | // Bipolar input mode disable
		(1 << IPR)  | // Input polarity reverse (PB3 > PB4)
		(0 << ADTS2) | (1 << ADTS1) | (1 << ADTS0); // Auto trigger on timer0 match A
	DIDR0 = 0; // Digital input disable

	// Use timer0 compA as analog trigger.
	// Trigger frequency is 2083.3 KHz with F_CPU = 1MHz
	TCCR0A =
		(0 << COM0A1) | (0 << COM0A0) | // not output on comp A
		(0 << COM0B1) | (0 << COM0B0) | // not output on compB
		(1 << WGM01)  | (0 << WGM00); // compare match CTC
	TCCR0B =
		(0 << FOC0A) | (0 << FOC0B) |
		(0 << WGM02) |
		(0 << CS02) | (1 << CS01) | (1 << CS00); // 64
	TCNT0 = 0;
	OCR0A = 60;
	OCR0B = 0;
	TIMSK = (1 << OCIE0A) | (0 << OCIE0B) | (0 << TOIE0); // no timer interrupt

	analogCurrentChannel = 0;
	analogChangeChannel(0);
	analogResetBuf();
}

void analogChangeChannel(int8_t inc)
{
	if (inc > 0 && analogCurrentChannel < 5) analogCurrentChannel++;
	else if (inc < 0 && analogCurrentChannel > 0) --analogCurrentChannel;

	// Warning v0 at Pb3, is 650 mv, v0 at PB4 is 750 mv. Pb4 - Pb3 may be negative. Even 20 as gain, do not use them
	// Using 1.1 v as analog refe may be overflow
	// 0 : double amplified (PB3), 1.1V ref, gain 1
	// 1 : double amplified (PB3), Vcc  ref, gain 1
	// 2 : single amplified (PB4), 1.1V ref, gain 1
	// 3 : single amplified (PB4), Vcc  ref, gain 1 (strong signal)
	switch (analogCurrentChannel)
	{
		case 0:
			ADMUX =
				(0 << REFS2) | (1 << REFS1) | (0 << REFS0) | // 1.1V
				(0 << ADLAR) | //no left adjust
				(0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (1 << MUX0); // PB3
			break;
		case 1:
			ADMUX =
				(0 << REFS2) | (0 << REFS1) | (0 << REFS0) | // Vcc
				(0 << ADLAR) | //no left adjust
				(0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (1 << MUX0); // PB3
			break;
		case 2:
			ADMUX =
				(0 << REFS2) | (1 << REFS1) | (0 << REFS0) | // 1.1V
				(0 << ADLAR) | //no left adjust
				(0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0); // PB4
			break;
		case 3:
			ADMUX =
				(0 << REFS2) | (0 << REFS1) | (0 << REFS0) | //Vcc
				(0 << ADLAR) | //no left adjust
				(0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0); // PB4
			break;
		default:
			ADMUX =
				(0 << REFS2) | (0 << REFS1) | (0 << REFS0) | //Vcc
				(0 << ADLAR) | //no left adjust
				(1 << MUX3) | (1 << MUX2) | (0 << MUX1) | (1 << MUX0); // Gnd
			break;
	}
}

void analogResetBuf(void)
{
	ANALOG_STOP();
	TCNT0 = 0;
	analogSize = 0;
	analogBuf[SAMPLE_SIZE - 1] = 0xFFFF; // set not ready yet
	ANALOG_START();
}

uint8_t analogIsBufReady(void)
{
	uint8_t res = 0;
	ANALOG_STOP();
	res = analogBuf[SAMPLE_SIZE - 1] != 0xFFFF;
	ANALOG_START();
	return res;
}

ISR(ADC_vect)
{
	analogBuf[analogSize++] = ADC;
	analogSize &= (SAMPLE_SIZE - 1);
}

ISR(TIM0_COMPA_vect)
{
	// Do nothing. Just set timer interrupt flags
}
