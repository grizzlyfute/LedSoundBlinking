#include "fft.h"
#include "analog.h"
#include "constants.h"
#include <avr/io.h>
//#include <avr/sleep.h>
#include <avr/interrupt.h>

int main (void)
{
	cli();
	// Use static to see real ram consumtion
	static ComplexInt fftBuffer[SAMPLE_SIZE]; // 2*2*64 = 256 of ram
	static uint8_t k = 0, n = 0;
	analogInit();
	DDRB = LED_ALL; // do not enable pull up
	PORTB = LED_ALL; // all off

	PRR =
		(1 << PRTIM1) | // Disable timer1 (20%)
		(0 << PRTIM0) | // Enable timer0 (2%)
		(1 << PRUSI)  | // Disable USI (2%)
		(0 << PRADC)  ; // Enable ADC (5%)
	MCUCR =
		(0 << BODS)  | // Do not touch bod in sleep mode for now
		(0 << PUD)   | // No internal pull up
		(0 << SE)    | // Not enabling sleep now
		(0 << SM1)   | (0 << SM0) | // sleep mode on idle
		(0 << BODSE) | // Bod sleep disable
		(0 << ISC01) | (0 << ISC00); // int0 (not enabled)
	sei();

	for (;;)
	{
		n = 1;
		// Analog management
		if (n > 0 && analogIsBufReady())
		{
			ANALOG_STOP();
			for (k = 0; k < SAMPLE_SIZE; k++)
			{
				fftBuffer[k].real = 0;
				fftBuffer[k].imag = analogBuf[k];
			}

			// If signal is too strong, change to low amplification chanel.
			// If two weak, switch to high amplification channel
			fftBuffer[0].real = fftBuffer[0].imag;
			fftBuffer[1].real = fftBuffer[0].imag;
			for (k = 0; k < SAMPLE_SIZE; k++)
			{
				// variance seem be more signifiant than amplitude ...
				if (fftBuffer[k].imag > fftBuffer[0].real) fftBuffer[0].real = fftBuffer[k].imag; // max
				if (fftBuffer[k].imag < fftBuffer[1].real) fftBuffer[1].real = fftBuffer[k].imag; // min
			}
			fftBuffer[1].real = fftBuffer[0].real - fftBuffer[1].real;
			// saturate(0) or too munch amplitude (1)
			if (fftBuffer[0].real > 950 || fftBuffer[1].real > 950)
			{
				analogChangeChannel(+1);
			}
			// We heard nothing
			else if (fftBuffer[1].real < 20)
			{
				if (analogCurrentChannel <= 0)
				{
					n = 0;
					PORTB |= LED_ALL;
				}
				analogChangeChannel(-1);
			}

			// Analog will continue working on background
			ANALOG_START();
			analogResetBuf();

			fftBuffer[0].real = 0;
			fftBuffer[1].real = 0;
		}
		else n = 0;

		// signal analysis
		if (n > 0)
		{
			// Compute notes
			fft(fftBuffer, SAMPLE_SIZE);

			const uint8_t DIVIDER = 8;
			// Analysis fourier
			for (n = 0; n < SAMPLE_SIZE; n += SAMPLE_SIZE / DIVIDER)
			{
				fftBuffer[n / DIVIDER].imag = 0;
				for (k = n; k < n + SAMPLE_SIZE / DIVIDER; k++)
				{
					// jump over DC composante (0Hz)
					if (n == 0 && k == 0) continue;
					fftBuffer[n / DIVIDER].imag += fftBuffer[k].real / (SAMPLE_SIZE / DIVIDER);
				}
			}
			n = 0;
			for (k = 0; k < DIVIDER; k++)
			{
				if (fftBuffer[k].imag > fftBuffer[n].imag) n = k;
			}

			// Led managemet
			// Switch color according to the note
			PORTB |= LED_ALL;
			if (fftBuffer[n].imag >= 128)
			{
				n %= 3;
				if (n == 2)
				{
					PORTB &= ~LED_BLUE;
				}
				else if (n == 1)
				{
					PORTB &= ~LED_RED;
				}
				else if (n == 0)
				{
					PORTB &= ~LED_GREEN;
				}
				n = 0;
			}
		}

		// Sleeping with active noise reduction will stop io clock, so timer0, so adc
//		sleep_bod_disable();
//		sleep_enable();
//		sleep_cpu(); //wake adc interrupt
//		sleep_disable();
	} // for(;;)
return 1;
}
