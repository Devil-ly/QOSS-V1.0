#include "Mathematical_Functions.h"

double fact(int n)
	{
		double s = 1.0;
		if (n > 0) {
			for (int i = 1; i <= n; i++)
			{
				s = s*i;
			}
		}
		else s = 1.0;
		return s;
	}

double D1_J(int m, double z)
{
	double a = 0.5*((_jn((m - 1), z)) - ((_jn((m + 1), z))));
	//金铭学长告诉的简便公式
	return a;
}

//共轭
complex<float> conjf(complex<float> x)
{
	complex<float> result;
	result = complex<float>(x.real(), -x.imag());
	return result;
}

complex<double> conjd(complex<double> x)
{
	complex<double> result;
	result = complex<double>(x.real(), -x.imag());
	return result;
}

complex<double> H_phi(double Kz0, double Kr0, int m0, double r0, double Phi0)
{
	complex <double> a1(0, -m0*Phi0);//-j*m0*Phi0
	complex <double> a2(0, 1);//j
	complex <double> a;
	a = -Kz0*m0 / Kr0 / Kr0 / r0*_jn(m0, Kr0*r0)*exp(a1) / a2 / 2.0;

	return a;

	//返回场分布值H_phi(复数形式)
}

complex<double> H_r(double Kz0, double Kr0, int m0, double r0, double Phi0)
{
	complex <double> a1(0, -m0*Phi0);//-j*m0*Phi0
	complex <double> a;
	a = -Kz0 / Kr0 *D1_J(m0, Kr0*r0)*exp(a1) / 2.0;

	return a;
}

complex<double> E_phi(double Kz0, double Kr0, int m0, double r0, double Phi0, double Omega0, double Beta0)
{
	complex <double> a;
	a = -Omega0*Mu0 / Beta0*H_r(Kz0, Kr0, m0, r0, Phi0);
	return a;
}

complex<double> E_r(double Kz0, double Kr0, int m0, double r0, double Phi0, double Omega0, double Beta0)
{
	complex <double> a;
	a = Omega0*Mu0 / Beta0*H_phi(Kz0, Kr0, m0, r0, Phi0);
	return a;
}

complex<double> H_z(double Kr0, int m0, double r0, double Phi0)
{
	complex <double> a1(0, -m0*Phi0);//-j*m0*Phi0
	complex <double> a;
	a = _jn(m0, Kr0*r0)*exp(a1) / 2.0;
	return a;

	//E_z=0
}

complex<double> H_phi_LeftRo(double Kz0, double Kr0, int m0, double r0, double Phi0)
{
	complex <double> a1(0, m0*Phi0);//j*m0*Phi0
	complex <double> a2(0, 1);//j
	complex <double> a;
	a = Kz0*m0 / Kr0 / Kr0 / r0*_jn(m0, Kr0*r0)*exp(a1) / 2.0 / a2;

	return a;
}

complex<double> H_r_LeftRo(double Kz0, double Kr0, int m0, double r0, double Phi0)
{
	complex <double> a1(0, m0*Phi0);//j*m0*Phi0
	complex <double> a;
	a = -Kz0 / Kr0 * D1_J(m0, Kr0*r0)*exp(a1) / 2.0;

	return a;
}

complex<double> E_phi_LeftRo(double Kz0, double Kr0, int m0, double r0, double Phi0,
	                         double Omega0, double Beta0)
{
	complex <double> a;
	a = -Omega0*Mu0 / Beta0*H_r_LeftRo(Kz0, Kr0, m0, r0, Phi0);
	return a;
}

complex<double> E_r_LeftRo(double Kz0, double Kr0, int m0, double r0, double Phi0, 
	                       double Omega0, double Beta0)
{
	complex <double> a;
	a = Omega0*Mu0 / Beta0*H_phi_LeftRo(Kz0, Kr0, m0, r0, Phi0);
	return a;
}

complex<double> H_z_LeftRo(double Kr0, int m0, double r0, double Phi0)
{
	complex <double> a1(0, m0*Phi0);//j*m0*Phi0
	complex <double> a;
	a = _jn(m0, Kr0*r0)*exp(a1) / 2.0;
	return a;

	//E_z_LeftRo=0
}


