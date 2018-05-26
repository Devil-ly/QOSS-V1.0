#include "Correlation_Coefficient.h"

void Correlation_Coefficient_Omega(double frequency0, double ds, int N0,
	const vector <vector <complex<double>>> &EX,
	const vector <vector <complex<double>>> &EY,
	int &index, int &MaxE_i, int &MaxE_j, double &W0)
{
	double MaxEX = 0;//初始化
	double MaxEY = 0;
	double MaxE = 0;
	int MaxEX_i = 0;//maxposition_EX
	int MaxEX_j = 0;
	int MaxEY_i = 0;//maxposition_EY
	int MaxEY_j = 0;

	//首先找到场强最大值,然后找到场强最大值所在的ii,jj
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
		{
			{
				
				if (abs(EX[i][j]) > MaxEX)
				{
					MaxEX = abs(EX[i][j]);
					MaxEX_i = i;
					MaxEX_j = j;
				}

				if (abs(EY[i][j]) > MaxEY)
				{
					MaxEY = abs(EY[i][j]);
					MaxEY_i = i;
					MaxEY_j = j;
				}
			}
		}

	MaxE_i = MaxEX_i;
	MaxE_j = MaxEX_j;
	MaxE = MaxEX;
	index = 0;

	if (MaxEY > MaxEX)
	{
		MaxE_i = MaxEY_i;
		MaxE_j = MaxEY_j;
		MaxE = MaxEY;
		index = 1;
	}

	vector <vector <double>> AbsE(N0, vector<double>(N0, 0));//主极化电场幅度值
	if (MaxEX > MaxEY)
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					AbsE[i][j] = abs(EX[i][j]);
				}
			}
	}
	if (MaxEY > MaxEX)
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					AbsE[i][j] = abs(EY[i][j]);
				}
			}
	}

//下面开始匹配最佳束腰大小
		int ii0 = 0;
		for (int k = 0; k < MaxE_i; k++)
		{
			ii0 = k;
			if ((AbsE[MaxE_i - k - 1][MaxE_j] <= (1.0 / Universal_Constant_e*MaxE))
				& (AbsE[MaxE_i - k][MaxE_j]>(1.0 / Universal_Constant_e*MaxE)))
				break;//一找到就跳出循环
		}

		int ii1 = MaxE_i;
		for (int k = MaxE_i; k < N0 - 1; k++)
		{
			ii1 = k;
			if ((AbsE[k + 1][MaxE_j] <= (1.0 / Universal_Constant_e*MaxE))
				& (AbsE[k][MaxE_j]>(1.0 / Universal_Constant_e*MaxE)))
				break;
		}

		int jj0 = 0;
		for (int k = 0; k < MaxE_j; k++)
		{
			jj0 = k;
			if ((AbsE[MaxE_i][MaxE_j - k - 1] <= (1.0 / Universal_Constant_e*MaxE))
				& (AbsE[MaxE_i][MaxE_j - k]>(1.0 / Universal_Constant_e*MaxE)))
				break;//一找到就跳出循环
		}

		int jj1 = MaxE_j;
		for (int k = MaxE_j; k < N0 - 1; k++)
		{
			jj1 = k;
			if ((AbsE[MaxE_i][k + 1] <= (1.0 / Universal_Constant_e*MaxE))
				& (AbsE[MaxE_i][k]>(1.0 / Universal_Constant_e*MaxE)))
				break;
		}

//求出匹配束腰值
		W0 = ds*(ii0 + (ii1 - MaxE_i) + jj0 + (jj1 - MaxE_j)) / 4.0;
}


double Scalar_Correlation_Coefficient(double frequency0, double ds, int N0,
	const vector <vector <complex<double>>> &EX,
	const vector <vector <complex<double>>> &EY)
{
	int index0 = 0; int MaxE_i0 = 0; int MaxE_j0 = 0; double w0 = 0;
	Correlation_Coefficient_Omega(frequency0, ds, N0, EX, EY,
		index0, MaxE_i0, MaxE_j0, w0);//先处理一下

//高斯波束在束腰处的电场幅度值
	vector <vector <complex<double>>> Gauss_E(N0, vector<complex<double>>(N0, 0));
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
		{ 
			{
				Gauss_E[i][j] = Gauss_E_Circular(frequency0, w0,
					Vector3(ds*(i - MaxE_i0), ds*(j - MaxE_j0), 0));
			}
		}

//计算高斯标量系数
	double sum1 = 0; double sum2 = 0; double sum3 = 0;
	double scalar_coefficient = 0;
	if (index0 == 0)//主极化为EX	
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					sum1 = sum1 + abs(EX[i][j])*abs(Gauss_E[i][j]);
					sum2 = sum2 + abs(EX[i][j])*abs(EX[i][j]);
					sum3 = sum3 + abs(Gauss_E[i][j])*abs(Gauss_E[i][j]);
				}
			}
	}

	if (index0 == 1)//主极化为EY	
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					sum1 = sum1 + abs(EY[i][j])*abs(Gauss_E[i][j]);
					sum2 = sum2 + abs(EY[i][j])*abs(EY[i][j]);
					sum3 = sum3 + abs(Gauss_E[i][j])*abs(Gauss_E[i][j]);
				}
			}
	}

	scalar_coefficient = sum1 / pow(sum2*sum3, 0.5);
	return scalar_coefficient;
}


double Vector_Correlation_Coefficient(double frequency0, double ds, int N0,
	const vector <vector <complex<double>>> &EX,
	const vector <vector <complex<double>>> &EY)
{
	int index0 = 0; int MaxE_i0 = 0; int MaxE_j0 = 0; double w0 = 0;
	Correlation_Coefficient_Omega(frequency0, ds, N0, EX, EY,
		index0, MaxE_i0, MaxE_j0, w0);//先处理一下

//高斯波束在束腰处的电场幅度值
	vector <vector <complex<double>>> Gauss_E(N0, vector<complex<double>>(N0, 0));
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
		{
			{
				Gauss_E[i][j] = Gauss_E_Circular(frequency0, w0,
					Vector3(ds*(i - MaxE_i0), ds*(j - MaxE_j0), 0));
			}
		}

	//计算高斯矢量系数
	complex<double> sum1 = 0;complex<double> sum2 = 0;
	double sum3 = 0; double sum4 = 0;
	double vector_coefficient = 0;
	if (index0 == 0)//主极化为EX	
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					sum1 = sum1 + EX[i][j] * Gauss_E[i][j];
					sum2 = sum2 + conj(EX[i][j]) * Gauss_E[i][j];
					sum3 = sum3 + abs(EX[i][j])*abs(EX[i][j]);
					sum4 = sum4 + abs(Gauss_E[i][j])*abs(Gauss_E[i][j]);
				}
			}
	}

	if (index0 == 1)//主极化为EY	
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					sum1 = sum1 + EY[i][j] * Gauss_E[i][j];
					sum2 = sum2 + conj(EY[i][j]) * Gauss_E[i][j];
					sum3 = sum3 + abs(EY[i][j])*abs(EY[i][j]);
					sum4 = sum4 + abs(Gauss_E[i][j])*abs(Gauss_E[i][j]);
				}
			}
	}

	vector_coefficient = abs(sum1 * sum2 / sum3 / sum4);//其实求出来的就是实数
	return vector_coefficient;
}


double Scalar_Correlation_Coefficient_TargetW0(double frequency0, double ds, int N0,
	const vector <vector <complex<double>>> &EX,
	const vector <vector <complex<double>>> &EY,
	double Target_W0)
{
	int index0 = 0; int MaxE_i0 = 0; int MaxE_j0 = 0; double w0 = 0;
	Correlation_Coefficient_Omega(frequency0, ds, N0, EX, EY,
		index0, MaxE_i0, MaxE_j0, w0);//先处理一下,wo不用

//高斯波束在束腰处的电场幅度值
	vector <vector <complex<double>>> Gauss_E(N0, vector<complex<double>>(N0, 0));
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
		{
			{
				Gauss_E[i][j] = Gauss_E_Circular(frequency0, Target_W0,
					Vector3(ds*(i - MaxE_i0), ds*(j - MaxE_j0), 0));
			}
		}

	//计算高斯标量系数
	double sum1 = 0; double sum2 = 0; double sum3 = 0;
	double scalar_coefficient = 0;
	if (index0 == 0)//主极化为EX	
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					sum1 = sum1 + abs(EX[i][j])*abs(Gauss_E[i][j]);
					sum2 = sum2 + abs(EX[i][j])*abs(EX[i][j]);
					sum3 = sum3 + abs(Gauss_E[i][j])*abs(Gauss_E[i][j]);
				}
			}
	}

	if (index0 == 1)//主极化为EY	
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					sum1 = sum1 + abs(EY[i][j])*abs(Gauss_E[i][j]);
					sum2 = sum2 + abs(EY[i][j])*abs(EY[i][j]);
					sum3 = sum3 + abs(Gauss_E[i][j])*abs(Gauss_E[i][j]);
				}
			}
	}

	scalar_coefficient = sum1 / pow(sum2*sum3, 0.5);
	return scalar_coefficient;
}


double Vector_Correlation_Coefficient_TargetW0(double frequency0, double ds, int N0,
	const vector <vector <complex<double>>> &EX,
	const vector <vector <complex<double>>> &EY,
	double Target_W0)
{
	int index0 = 0; int MaxE_i0 = 0; int MaxE_j0 = 0; double w0 = 0;
	Correlation_Coefficient_Omega(frequency0, ds, N0, EX, EY,
		index0, MaxE_i0, MaxE_j0, w0);//先处理一下

//高斯波束在束腰处的电场幅度值
	vector <vector <complex<double>>> Gauss_E(N0, vector<complex<double>>(N0, 0));
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
		{
			{
				Gauss_E[i][j] = Gauss_E_Circular(frequency0, Target_W0,
					Vector3(ds*(i - MaxE_i0), ds*(j - MaxE_j0), 0));
			}
		}

	//计算高斯矢量系数
	complex<double> sum1 = 0; complex<double> sum2 = 0;
	double sum3 = 0; double sum4 = 0;
	double vector_coefficient = 0;
	if (index0 == 0)//主极化为EX	
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					sum1 = sum1 + EX[i][j] * Gauss_E[i][j];
					sum2 = sum2 + conj(EX[i][j]) * Gauss_E[i][j];
					sum3 = sum3 + abs(EX[i][j])*abs(EX[i][j]);
					sum4 = sum4 + abs(Gauss_E[i][j])*abs(Gauss_E[i][j]);
				}
			}
	}

	if (index0 == 1)//主极化为EY	
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					sum1 = sum1 + EY[i][j] * Gauss_E[i][j];
					sum2 = sum2 + conj(EY[i][j]) * Gauss_E[i][j];
					sum3 = sum3 + abs(EY[i][j])*abs(EY[i][j]);
					sum4 = sum4 + abs(Gauss_E[i][j])*abs(Gauss_E[i][j]);
				}
			}
	}

	vector_coefficient = abs(sum1 * sum2 / sum3 / sum4);//其实求出来的就是实数
	return vector_coefficient;
}
