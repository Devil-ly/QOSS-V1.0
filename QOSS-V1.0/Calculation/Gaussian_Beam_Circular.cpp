#include "BeamPropagation.h"

//返回传播z长度后的高斯波束W值
double Gauss_Omega_Circular(double frequency0, double w0, double z0)
{
	double lamda = C_Speed / frequency0;
	double w = w0*pow(1.0 + pow((lamda*z0 / Pi / w0 / w0), 2), 0.5);

	return w;
}

//返回圆高斯波束在P（x,y,z）的复数主极化值
complex<double> Gauss_E_Circular(double frequency0, double w0, Vector3 P)
{
	double x0 = P.x;
	double y0 = P.y;
	double z0 = P.z;
	double lamda = C_Speed / frequency0;
	double k = 2 * Pi / lamda;

	//由于公式中的R，即曲率半径公式中存在z为分母的情况,为了保险起见改为定义R1=1/R
	double R1 = z0 / (z0*z0 + pow(Pi*w0*w0 / lamda, 2));

	double phi0 = atan(lamda*z0 / Pi / w0 / w0);

	complex <double> j(0, 1);//定义虚数单位j

	complex<double> E = pow(2.0 / Pi / pow(Gauss_Omega_Circular(frequency0, w0, z0), 2), 0.5)
		*exp(-(x0*x0 + y0*y0) / pow(Gauss_Omega_Circular(frequency0, w0, z0), 2))
		*exp(-j*k*z0)
		*exp(-j*k*(x0*x0 + y0*y0) / 2.0 *R1)
		*exp(j*phi0);
	//公式来源：《Gaussian Beam Quasi-optical Propagation and Applications》P25
	return E;
}

//返回圆高斯波束在P（x,y,z）的弧度形式的未折叠相位值
double Gauss_Phase_Circular(double frequency0, double w0, Vector3 P)
{
	double x0 = P.x;
	double y0 = P.y;
	double z0 = P.z;
	double lamda = C_Speed / frequency0;
	double k = 2 * Pi / lamda;

	//由于公式中的R，即曲率半径公式中存在z为分母的情况,为了保险起见改为定义R1=1/R
	double R1 = z0 / (z0*z0 + pow(Pi*w0*w0 / lamda, 2));

	double phi0 = atan(lamda*z0 / Pi / w0 / w0);

	double Phase = -k*z0 - k*(x0*x0 + y0*y0) / 2.0 *R1 + phi0;
	//公式来源：《Gaussian Beam Quasi-optical Propagation and Applications》P25
	return Phase;
}