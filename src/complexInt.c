#include "complexInt.h"

#ifdef TESTMODE
#define PROGMEM
#define pgm_read_byte(x) *(x)
#else // !TESTMODE
#include <avr/pgmspace.h>
#endif // TESTMODE

// 127 * sin (i * 2 * M_PI / 256)
const int8_t sin_pcm[256] PROGMEM =
{
   0,    3,    6,    9,   12,   15,   18,   21,
   24,   27,   30,   33,   36,   39,   42,   45,
   48,   51,   54,   57,   59,   62,   65,   67,
   70,   73,   75,   78,   80,   82,   85,   87,
   89,   91,   94,   96,   98,  100,  102,  103,
   105,  107,  108,  110,  112,  113,  114,  116,
   117,  118,  119,  120,  121,  122,  123,  123,
   124,  125,  125,  126,  126,  126,  126,  126,
   127,  126,  126,  126,  126,  126,  125,  125,
   124,  123,  123,  122,  121,  120,  119,  118,
   117,  116,  114,  113,  112,  110,  108,  107,
   105,  103,  102,  100,   98,   96,   94,   91,
   89,   87,   85,   82,   80,   78,   75,   73,
   70,   67,   65,   62,   59,   57,   54,   51,
   48,   45,   42,   39,   36,   33,   30,   27,
   24,   21,   18,   15,   12,    9,    6,    3,
   0,   -3,   -6,   -9,  -12,  -15,  -18,  -21,
  -24,  -27,  -30,  -33,  -36,  -39,  -42,  -45,
  -48,  -51,  -54,  -57,  -59,  -62,  -65,  -67,
  -70,  -73,  -75,  -78,  -80,  -82,  -85,  -87,
  -89,  -91,  -94,  -96,  -98, -100, -102, -103,
  -105, -107, -108, -110, -112, -113, -114, -116,
  -117, -118, -119, -120, -121, -122, -123, -123,
  -124, -125, -125, -126, -126, -126, -126, -126,
  -127, -126, -126, -126, -126, -126, -125, -125,
  -124, -123, -123, -122, -121, -120, -119, -118,
  -117, -116, -114, -113, -112, -110, -108, -107,
  -105, -103, -102, -100,  -98,  -96,  -94,  -91,
  -89,  -87,  -85,  -82,   -80,  -78,  -75,  -73,
  -70,  -67,  -65,  -62,   -59,  -57,  -54,  -51,
  -48,  -45,  -42,  -39,   -36,  -33,  -30,  -27,
  -24,  -21,  -18,  -15,   -12,   -9,   -6,   -3,
};

void cplx_sub(ComplexInt *r, ComplexInt *a, ComplexInt *b)
{
  r->real = a->real - b->real;
  r->imag = a->imag - b->imag;
}

void cplx_add(ComplexInt *r, ComplexInt *a, ComplexInt *b)
{
  r->real = a->real + b->real;
  r->imag = a->imag + b->imag;
}

void cplx_rotation(ComplexInt *r, uint8_t arg)
{
  // (a+i*b)*exp(i*x)
  // (a+i*b)*(cos(x) + i*sin(x))
  // a*cos(x) + i*a*sin(x) + i*b*cos(x) - b*sin(x)
  // (a*cos(x) - b*sin(x)) + i*(a*sin(x) + b*cos(x))

  int32_t sine, cosine;
  int16_t real;
  int32_t tmp; // avoiding integer overflow
  sine = pgm_read_byte(&sin_pcm[arg]);
  // 8 bits ensure modulo. cos (arg) = sin (arg + PI/2)
  arg += 64;
  cosine = pgm_read_byte(&sin_pcm[arg]);

  // Force cast to avoid integer overflow
  tmp = (int32_t)r->real * cosine - (int32_t)r->imag * sine;
  tmp /= 127L;
  real = tmp; // Do not applying mask 0xFFFF to save sign bit

  tmp = (int32_t)r->real * sine + (int32_t)r->imag * cosine;
  tmp /= 127L;

  r->imag = tmp;
  r->real = real;
}

int16_t cplx_norm(ComplexInt *x)
{
	int16_t ret;
	// using norm 1
	if (x->real > 0) ret = x->real;
	else ret = -x->real;

	if (x->imag > 0)
	{
		if (ret + x->imag >= ret) ret += x->imag;
		else ret = 32767;
	}
	else
	{
		if (ret - x->imag >= ret) ret -= x->imag;
		else ret = 32767;
	}
	return ret;
}
