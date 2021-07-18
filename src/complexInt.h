#ifndef COMPLEXINT_H
#define COMPLEXINT_H

#include <stdint.h>

typedef struct ComplexInt
{
	int16_t real;
	int16_t imag;
} ComplexInt;

// r = a + b
void cplx_sub(ComplexInt *r, ComplexInt *a, ComplexInt *b);
// r = a - b
void cplx_add(ComplexInt *r, ComplexInt *a, ComplexInt *b);
// r = r*exp(iarg)
void cplx_rotation(ComplexInt *r, uint8_t arg);

int16_t cplx_norm(ComplexInt *x);
#endif // COMPLEXINT_H
