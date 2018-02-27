#pragma once
#ifndef FFT_H
#define FFT_H

#include "fftw3.h"
#include "../util/GEMS_Memory.h"
#include <complex>

class FFT
{
public:
	FFT();
	~FFT();
	void FFT_2D(double **InputR, double **InputI, double **OutputR, double **OutputI, int N, int M);
	void IFFT_2D(double **InputR, double **InputI, double **OutputR, double **OutputI, int N, int M);
	
	void FFT_2D(std::complex<double> **Input, std::complex<double> **Output, int N, int M);
	void IFFT_2D(std::complex<double> **Input, std::complex<double> **Output, int N, int M);

private:

};


#endif
