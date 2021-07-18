#ifndef FFT_H
#define FFT_H

#include <stdint.h>
#include "complexInt.h"

// discrete fourier transform
uint8_t fftlog2(uint8_t n);
uint8_t reverse_bits(uint8_t i);
void fft(ComplexInt *a, uint8_t n);


void dft(ComplexInt *a, uint8_t n);
#endif // FFT_H

