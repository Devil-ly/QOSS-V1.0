#include "Gaussian_Beam_Elliptical.h"

double Gauss_Omega_Elliptical(double frequency0, double w0, double z0)
{
	double lamda = C_Speed / frequency0;
	double w = w0*pow(1.0 + pow((lamda*z0 / Pi / w0 / w0), 2), 0.5);

	return w;
}

//返回波束传播Z距离后的曲率半径R(Wx和Wy公式一样)
double Gauss_R_Elliptical(double frequency0, double w0, double z0)
{
	double lamda = C_Speed / frequency0;
	double R1 = z0 / (z0*z0 + pow(Pi*w0*w0 / lamda, 2));

	return R1;
}

//返回相位值
double Phi_Elliptical(double frequency0, double w0x, double w0y, double z0)
{
	double lamda = C_Speed / frequency0;
	double phi = 0.5*(atan(lamda*z0 / Pi / w0x / w0x) + atan(lamda*z0 / Pi / w0y / w0y));

	return phi;
}

//返回圆高斯波束在坐标点P（x,y,z）的复数主极化值
complex<double> Gauss_E_Elliptical(double frequency0, double w0x, double w0y, Vector3 P)
{
	double lamda = C_Speed / frequency0;
	double k = 2 * Pi / lamda;
	complex <double> j(0, 1);//定义虚数单位j

	complex<double> E = pow(2.0 / Pi / Gauss_Omega_Elliptical(frequency0, w0x, P.z) / Gauss_Omega_Elliptical(frequency0, w0y, P.z), 0.5)
		*exp(-P.x * P.x / pow(Gauss_Omega_Elliptical(frequency0, w0x, P.z), 2))
		*exp(-P.y * P.y / pow(Gauss_Omega_Elliptical(frequency0, w0y, P.z), 2))
		*exp(-j*k*P.z)
		*exp(-j*k*P.x * P.x / 2.0 *Gauss_R_Elliptical(frequency0, w0x, P.z))
		*exp(-j*k*P.y * P.y / 2.0 *Gauss_R_Elliptical(frequency0, w0y, P.z))
		*exp(j*Phi_Elliptical(frequency0, w0x, w0y, P.z));

	return E;
}