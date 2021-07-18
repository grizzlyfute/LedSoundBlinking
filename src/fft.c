/* dft.cpp
 * This is an implementation of the Cooley-Tukey recursive FFT algorithm.
 */
#include "fft.h"
#include "complexInt.h"
#include "constants.h"

uint8_t fftlog2(uint8_t n)
{
	uint8_t r = 0;
	while (n > 1)
	{
		r++;
		n >>= 1;
	}
	return r;
}

uint8_t reverse_bits(uint8_t i)
{
	register uint8_t mask = 0x55;
	i = ((i & mask) << 1) | ((i >> 1) & mask);
	mask = 0x33;
	i = ((i & mask) << 2) | ((i >> 2) & mask);
	mask = 0x0f;
	i = ((i & mask) << 4) | ((i >> 4) & mask);
	return i;
}

// https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
// https://equilibriumofnothing.wordpress.com/2013/10/14/algorithm-iterative-fft/
// This is an itterative version of fast fourier transform Cooley-Tuckey (o(n ln(n)))
void fft(ComplexInt *a, uint8_t n)
{
	uint8_t offset, step, k, s;
	uint8_t w, wn;
	ComplexInt t;
	const uint8_t logn = fftlog2(n);

	for (k = 0; k < n; k++)
	{
		a[reverse_bits(k) >> (8 - logn)].real = a[k].imag;
		a[k].imag = 0;
	}

	for (s = 0; s < logn; s++)
	{
		step = 1 << s; // 2^s
		wn = -256 / (2*step); // + 256 / (2*step) for inverse
		for (offset = 0; offset < n; offset += 2*step)
		{
			w = 0;
			for (k = offset; k < offset + step; k++)
			{
				t = a[k + step];
				cplx_rotation(&t, w);
				cplx_sub(&a[k + step], &a[k], &t);
				cplx_add(&a[k], &a[k], &t);
				w += wn;
			}
		}
	}

	for (k = 0; k < n; k++)
	{
		// divide by n if inverse
		a[k].real = cplx_norm(&a[k]);
		a[k].imag = 0;
	}
}

/*
// Cooley-Tuckey recursive way
void swap(ComplexInt *a, uint8_t x, uint8_t y)
{
	checkStack();
	ComplexInt w;
	w = a[y];
	a[y] = a[x];
	a[x] = w;
}

// Separate even/odd elements to lower/upper halves of array respectively.
// Can not use internal buffer of SAMPLE_SIZE/2 because not enought memory on attiny85
// For each index, pull element wich will be at this index by doing invertion until the element reach.
// exlpe : 0 2 [1] 3 4 5 6 7
// => 0 2 [1] 4 3 5 6 7
// => 0 2 [4] 1 3 5 6 7
// separate(0 1 2 3 4 5 6 7) -> 0 2 4 6 1 3 5 7
void separate(ComplexInt *a, uint8_t n)
{
	checkStack();
	uint8_t i = 0, j = 0;

	// o((n/2*n/2)/2)) = o(n^2/8)
	for (i = 1; i < n; i++)
	{
		for (j = 2*i; j > i; --j)
		{
			swap(a, j - 1, j);
		}
	}
}

// N must be a power-of-2, or bad things will happen.
//
// N input samples in X[] are FFT'd and results left in X[].
// Because of Nyquist theorem, N samples means
// only first N/2 FFT results in X[] are the answer.
// (upper half of X[] is a reflection with no new information).
void fft2(ComplexInt* data, uint8_t n)
{
	uint8_t k;
	ComplexInt w;
	// n < 2: bottom of recursion.
	// Do nothing here, because already X[0] = x[0]
	if (n >= 2)
	{
		n /= 2;
		separate(data, n);  // all evens to lower half, all odds to upper half
		fft2(data,   n);   // recurse even items
		fft2(data+n, n);   // recurse odd  items
		// combine results of two half recursions
		for (k = 0; k < n; k++)
		{
			// w is the "twiddle-factor"
			// e = data[k], o = data[k + n], w = exp(-2*M-PI*k*i/(2*n))
			// data[k] = e + o * w
			// data[k+n] = e - o * w
			w = data[k+n];

			// aproximating sin,cos produce "noise" on fft result
			cplx_rotation(&w, -128 * k / n);
			cplx_minus(&data[k+n], &w, &data[k]); // data[k+n] = w - data[k]
			cplx_plus (&data[k  ], &w, &data[k]); // data[k] = w + data[k]
		}
	}

//	if (n == SAMPLE_SIZE)
//	{
//		for (k = 0; k < n; k++)
//		{
//			// divide by n if inverse
//			data[k].real = cplx_norm(&data[k]);
//			data[k].imag = 0;
//		}
//	}
}
*/

/*
// discrete fourrier transform naive algorithm (o(n^2))
// Can not use standard fast fourier transform (Cooley-Tuckey)
// due to insuficient memory resource of attiny
void dft(ComplexInt *a, uint8_t n)
{
	uint16_t dataIn[SAMPLE_SIZE];
	uint16_t dataOut[SAMPLE_SIZE];

	// S(z) = sum(k=0, k<n, input[k]*exp(-2*i*M_PI*z*k/n)); // Discrete
	ComplexInt w, sum = {.real = 0, .imag = 0};
	uint8_t j, k;
	for (k = 0; k < n; k++) dataIn[k] = a[k].imag;

	for (k = 0; k < n; k++)
	{
		sum.real = 0;
		sum.imag = 0;
		for (j = 0; j < n; j++)
		{
			w.real = dataIn[j];
			w.imag = 0;
			// w = w*exp (-(256/(2*pi))*2*pi*k*j/n)
			cplx_rotation(&w, (-256 * j * k / n) & 0xFF);
			cplx_add(&sum, &w, &sum);
		}
		dataOut[k] = cplx_norm(&sum);
	}

	for (k = 0; k < n; k++) a[k].real = dataOut[k];

	// Sinv(z) = sum(k=0, k<n, input[k]*exp(2*i*M_PI*z*k/n)); // Inverse
}
*/
