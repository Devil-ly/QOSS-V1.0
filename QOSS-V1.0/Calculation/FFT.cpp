#include "FFT.h"

FFT::FFT()
{
}

FFT::~FFT()
{
}

void FFT::FFT_2D(double **InputR, double **InputI, double **OutputR, double **OutputI, int N, int M)
{
	fftw_complex *in, *out;
	fftw_plan p;
	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * M);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * M);

	// 输入数据in赋值
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			in[i + j * N][0] = InputR[i][j];
			in[i + j * N][1] = InputI[i][j];
		}
	}

	p = fftw_plan_dft_2d(N, M, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p); // 执行变换
	fftw_destroy_plan(p);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			OutputR[i][j] = out[i + j * N][0] / N / M;
			OutputI[i][j] = out[i + j * N][1] / N / M;
		}
	}
	fftw_free(in);
	fftw_free(out);
}

void FFT::IFFT_2D(double **InputR, double **InputI, double **OutputR, double **OutputI, int N, int M)
{
	fftw_complex *in, *out;
	fftw_plan p;
	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * M);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * M);

	// 输入数据in赋值
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			in[i + j * N][0] = InputR[i][j];
			in[i + j * N][1] = InputI[i][j];
		}
	}

	p = fftw_plan_dft_2d(N, M, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(p); // 执行变换
	fftw_destroy_plan(p);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			OutputR[i][j] = out[i + j * N][0];
			OutputI[i][j] = out[i + j * N][1];
		}
	}
	fftw_free(in);
	fftw_free(out);
}

void FFT::FFT_2D(std::complex<double> **Input, std::complex<double> **Output, int N, int M)
{
	fftw_complex *in, *out;
	fftw_plan p;
	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * M);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * M);

	// 输入数据in赋值
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			in[i + j * N][0] = Input[i][j].real();
			in[i + j * N][1] = Input[i][j].imag();
		}
	}

	p = fftw_plan_dft_2d(N, M, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p); // 执行变换
	fftw_destroy_plan(p);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			Output[i][j] = std::complex <double>(out[i + j * N][0],
				out[i + j * N][1]);
		}
	}
	fftw_free(in);
	fftw_free(out);
}

void FFT::IFFT_2D(std::complex<double> **Input, std::complex<double> **Output, int N, int M)
{
	fftw_complex *in, *out;
	fftw_plan p;
	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * M);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * M);

	// 输入数据in赋值
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			in[i + j * N][0] = Input[i][j].real();
			in[i + j * N][1] = Input[i][j].imag();
		}
	}

	p = fftw_plan_dft_2d(N, M, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(p); // 执行变换
	fftw_destroy_plan(p);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			Output[i][j] = std::complex <double>(out[i + j * N][0] / N / M,
				out[i + j * N][1] / N / M);
		}
	}
	fftw_free(in);
	fftw_free(out);
}