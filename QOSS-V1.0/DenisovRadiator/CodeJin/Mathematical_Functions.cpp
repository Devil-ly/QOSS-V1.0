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


double rootdbessel(int m, int n)	//Find the nth root of mth order derivate bessel function of the first kind
{
	//由夏冬编写于圆波导模式场的生成-SourceModeGeneration.cpp中
	//截取出来，作为独立的函数 金铭
	const int MaxLoopNumber = 10000;//最大循环次数
	const double step0 = 0.1; //粗扫的步进
	const double step1 = step0 / MaxLoopNumber;//第一次细扫的步进

	int n0 = 0;//第n0个导数根的初始化
	int i0 = 0;//找到了第n个导数根后的区间标记
	for (int i = 1; i <= MaxLoopNumber; i++)//特意不从0开始
	{
		if ((D1_J(m, i*step0) >= 0.0 && D1_J(m, (i + 1)*step0) <= 0.0)
			|| (D1_J(m, i*step0) <= 0.0 && D1_J(m, (i + 1)*step0) >= 0.0))
		{
			n0 = n0 + 1;//开始计数
			i0 = i;//记下该区间
		}
		if (n0 == n) break;//找到第n个导数根后直接跳出循环，此时导数根落在[i0*ds，(i0+1)*ds]区间上

	}

	//开始第一次细扫[i0*step0，(i0+1)*step0]区间,间隔更小
	int j0 = 0;//找到了第n个导数根后的区间标记
	for (int j = 0; j <= MaxLoopNumber; j++)//从0开始
	{

		if ((D1_J(m, j*step1 + i0*step0) >= 0.0 && D1_J(m, (j + 1)*step1 + i0*step0) <= 0.0)
			|| (D1_J(m, j*step1 + i0*step0) <= 0.0 && D1_J(m, (j + 1)*step1 + i0*step0) >= 0.0))
		{
			j0 = j;//找到了这个区间，并记下该区间下标
			break;//找到第n个导数根后直接跳出循环，此时导数根落在[i*ds，(i+1)*ds]区间上
		}
	}

	double Xmn = 0.5*((j0*step1 + i0*step0) + ((j0 + 1)*step1 + i0*step0));
	//20171026已经验证，完全符合MathCad计算结果 夏冬Good！
	return Xmn;
}

double rootbessel(int m, int n)	//Find the nth root of mth order bessel function of the first kind
{
	//在rootdbessel函数基础上改写	金铭
	const int MaxLoopNumber = 10000;//最大循环次数
	const double step0 = 0.1; //粗扫的步进
	const double step1 = step0 / MaxLoopNumber;//第一次细扫的步进

	int n0 = 0;//第n0个导数根的初始化
	int i0 = 0;//找到了第n个导数根后的区间标记
	for (int i = 1; i <= MaxLoopNumber; i++)//特意不从0开始
	{
		if ((jn(m, i*step0) >= 0.0 && jn(m, (i + 1)*step0) <= 0.0)
			|| (jn(m, i*step0) <= 0.0 && jn(m, (i + 1)*step0) >= 0.0))
		{
			n0 = n0 + 1;//开始计数
			i0 = i;//记下该区间
		}
		if (n0 == n) break;//找到第n个导数根后直接跳出循环，此时导数根落在[i0*ds，(i0+1)*ds]区间上

	}

	//开始第一次细扫[i0*step0，(i0+1)*step0]区间,间隔更小
	int j0 = 0;//找到了第n个导数根后的区间标记
	for (int j = 0; j <= MaxLoopNumber; j++)//从0开始
	{

		if ((jn(m, j*step1 + i0*step0) >= 0.0 && jn(m, (j + 1)*step1 + i0*step0) <= 0.0)
			|| (jn(m, j*step1 + i0*step0) <= 0.0 && jn(m, (j + 1)*step1 + i0*step0) >= 0.0))
		{
			j0 = j;//找到了这个区间，并记下该区间下标
			break;//找到第n个导数根后直接跳出循环，此时导数根落在[i*ds，(i+1)*ds]区间上
		}
	}

	double Xmn = 0.5*((j0*step1 + i0*step0) + ((j0 + 1)*step1 + i0*step0));
	//20171026已经验证，完全符合MathCad计算结果 Good！
	return Xmn;
}

double kzmnTE(int m, int n, double lambda, double radius) {
	double kz;
	double k0 = 2 * Pi / lambda;
	double X0 = rootdbessel(m,n);
	double kr = X0 / radius;
	kz = sqrt(k0*k0 - kr*kr);
	return kz;
}