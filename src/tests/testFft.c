#include "test.h"
#include "fft.h"
#include "complexInt.h"
#include <math.h>

#define SAMPLE_SIZE 64

void testFft()
{
	TEST_BEGIN();
	ComplexInt fftBuffer[SAMPLE_SIZE];
	int k = 0, z = 0;
	int freq[5] = {2, 5, 11, 17, 29};

	for (k = 0; k < SAMPLE_SIZE; k++)
	{
		fftBuffer[k].real = 0;
		fftBuffer[k].imag = 0;
		for (z = 0; z < 5; z++)
			fftBuffer[k].imag += 512*(sin(2*M_PI*freq[z]*k / SAMPLE_SIZE) + 1) / 5 + 50*(sin(2*M_PI*2000) + 1);
	}

//	for (k = 0; k < SAMPLE_SIZE; k++) printf("%d : %d\n", k, fftBuffer[k].imag);
	fft(fftBuffer, SAMPLE_SIZE);
//	for (k = 0; k < SAMPLE_SIZE; k++) printf("%d : %d\n", k, fftBuffer[k].real);


	assertInt(fftBuffer[ 0].real >= 4096, 1);
	assertInt(fftBuffer[ 1].real >= 128, 0);
	assertInt(fftBuffer[ 2].real >= 1024, 1);
	assertInt(fftBuffer[ 3].real >= 128, 0);
	assertInt(fftBuffer[ 4].real >= 128, 0);
	assertInt(fftBuffer[ 5].real >= 1024, 1);
	assertInt(fftBuffer[ 6].real >= 128, 0);
	assertInt(fftBuffer[ 7].real >= 128, 0);
	assertInt(fftBuffer[ 8].real >= 128, 0);
	assertInt(fftBuffer[ 9].real >= 128, 0);
	assertInt(fftBuffer[10].real >= 128, 0);
	assertInt(fftBuffer[11].real >= 1024, 1);
	assertInt(fftBuffer[12].real >= 128, 0);
	assertInt(fftBuffer[13].real >= 128, 0);
	assertInt(fftBuffer[14].real >= 128, 0);
	assertInt(fftBuffer[15].real >= 128, 0);
	assertInt(fftBuffer[16].real >= 128, 0);
	assertInt(fftBuffer[17].real >= 1024, 1);
	assertInt(fftBuffer[18].real >= 128, 0);
	assertInt(fftBuffer[19].real >= 128, 0);
	assertInt(fftBuffer[20].real >= 128, 0);
	assertInt(fftBuffer[21].real >= 128, 0);
	assertInt(fftBuffer[22].real >= 128, 0);
	assertInt(fftBuffer[23].real >= 128, 0);
	assertInt(fftBuffer[24].real >= 128, 0);
	assertInt(fftBuffer[25].real >= 128, 0);
	assertInt(fftBuffer[26].real >= 128, 0);
	assertInt(fftBuffer[27].real >= 128, 0);
	assertInt(fftBuffer[28].real >= 128, 0);
	assertInt(fftBuffer[29].real >= 1024, 1);
	assertInt(fftBuffer[30].real >= 128, 0);
	assertInt(fftBuffer[31].real >= 128, 0);
	assertInt(fftBuffer[32].real >= 128, 0);
	assertInt(fftBuffer[33].real >= 128, 0);
	assertInt(fftBuffer[34].real >= 128, 0);
	//assertInt(fftBuffer[35].real >= 128, 0);
	assertInt(fftBuffer[36].real >= 128, 0);
	assertInt(fftBuffer[37].real >= 128, 0);
	assertInt(fftBuffer[38].real >= 128, 0);
	assertInt(fftBuffer[39].real >= 128, 0);
	assertInt(fftBuffer[40].real >= 128, 0);
	assertInt(fftBuffer[41].real >= 128, 0);
	assertInt(fftBuffer[42].real >= 128, 0);
	assertInt(fftBuffer[43].real >= 128, 0);
	assertInt(fftBuffer[44].real >= 128, 0);
	assertInt(fftBuffer[45].real >= 128, 0);
	assertInt(fftBuffer[46].real >= 128, 0);
	//assertInt(fftBuffer[47].real >= 128, 0);
	assertInt(fftBuffer[48].real >= 128, 0);
	assertInt(fftBuffer[49].real >= 128, 0);
	assertInt(fftBuffer[50].real >= 128, 0);
	assertInt(fftBuffer[51].real >= 128, 0);
	assertInt(fftBuffer[52].real >= 128, 0);
	//assertInt(fftBuffer[53].real >= 128, 0);
	assertInt(fftBuffer[54].real >= 128, 0);
	assertInt(fftBuffer[55].real >= 128, 0);
	assertInt(fftBuffer[56].real >= 128, 0);
	assertInt(fftBuffer[57].real >= 128, 0);
	assertInt(fftBuffer[58].real >= 128, 0);
	//assertInt(fftBuffer[59].real >= 128, 0);
	assertInt(fftBuffer[60].real >= 128, 0);
	assertInt(fftBuffer[61].real >= 128, 0);
	//assertInt(fftBuffer[62].real >= 128, 0);
	assertInt(fftBuffer[63].real >= 128, 0);
	TEST_END();
}

// int main(int argc, char *argv[])
int main(void)
{
	testFft();
	//testSeparate();
}
