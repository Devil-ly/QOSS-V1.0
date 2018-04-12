#include "../DenisovRadiator/CodeJin/Mathematical_Functions_Jin.h"


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