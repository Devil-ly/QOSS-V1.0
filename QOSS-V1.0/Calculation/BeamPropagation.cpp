#include "BeamPropagation.h"

//返回传播z长度后的高斯波束W值
void Field_Split(double frequency0, double ds, int N0,
	vector <vector <complex <double>>> &E0,
	int &Actual_SplitTimes, vector <vector <double>> &Split_Info)
{
	double lamda = C_Speed / frequency0;//计算波长

	complex <double> j0(0, 1);//定义虚数单位j0

	//输入必须是N0*N0矩阵,与李赟商量好再说
	vector <vector <complex<double>>> E1(N0, vector<complex<double>>(N0, 0));
	vector <vector <double>> AbsE1(N0, vector<double>(N0, 0));
	vector <vector <double>> PhaseE1(N0, vector<double>(N0, 0));

	double Whole_Power0 = 0;//计算口面场的总能量,初始化为0

	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
		{
			{
				//complex<double> mycomplex(2.000, 2);
				//AbsE0[i][j] = abs(mycomplex);

				E1[i][j] = E0[i][j];

				AbsE1[i][j] = abs(E0[i][j]);

				PhaseE1[i][j] = arg(E0[i][j]);

				Whole_Power0 = Whole_Power0 + AbsE1[i][j] * AbsE1[i][j];
			}
		}//得到相位和幅值分布(E1用于消减)
	//20180306检查至此，没问题
	//20180307取消E0赋值，不用它，只用E1

	//下面开始将场分布分解为一系列的高斯波束

	//首先来定义分解信息
	int Max_SplitTimes = 300;//定义最大分解次数

	Actual_SplitTimes = 0;//将实际分解次数初始化为0

	Split_Info.resize(Max_SplitTimes);
	for (int i = 0; i < Max_SplitTimes; i++)
	{
		Split_Info[i].resize(15);
	}//Split_Info是分解总信息（Max_SplitTimes*15），其中单次分解包含15个数字信息

	double Remained_Power_Ratio = 0.002;//定义剩余能量阈值，剩余能量低于0.2%时停止分解

	//为方便测试，添加一个小数组
	vector <double> Power_Remained(Max_SplitTimes);

	//20180307将一些大矩阵提前定义，节省时间
	//P1是以MaxP点为中心的场分布坐标系
	vector <vector <Vector3>> P1(N0, vector<Vector3>(N0, 0));

	for (int k = 0; k < Max_SplitTimes; k++)
	{
		//寻找场强最大值与位置
		double MaxE = 0;//初始化
		int MaxP_i = 0;//maxposition
		int MaxP_j = 0;

		//首先找到场强最大值,然后找到场强最大值所在的ii,jj
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					if (AbsE1[i][j] > MaxE)
					{
						MaxE = AbsE1[i][j];
						MaxP_i = i;
						MaxP_j = j;
					}

				}
			}

		//下面依据幅度最大值点的等相面
		Vector3 Ni, Nj;

		//先计算Ni
		double a = 0, b = 0;
		if (MaxP_i > 0 & MaxP_i < N0 - 1)
		{
			a = PhaseE1[MaxP_i - 1][MaxP_j];
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i - 1][MaxP_j])>0.75 * 2.0 * Pi)
				a = PhaseE1[MaxP_i - 1][MaxP_j] + 2.0 * Pi;
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i - 1][MaxP_j]) < -0.75 * 2.0 * Pi)
				a = PhaseE1[MaxP_i - 1][MaxP_j] - 2.0 * Pi;//消除相位折叠

			b = PhaseE1[MaxP_i + 1][MaxP_j];
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i + 1][MaxP_j]) > 0.75 * 2.0 * Pi)
				b = PhaseE1[MaxP_i + 1][MaxP_j] + 2.0 * Pi;
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i + 1][MaxP_j]) < -0.75 * 2.0 * Pi)
				b = PhaseE1[MaxP_i + 1][MaxP_j] - 2.0 * Pi;//消除相位折叠
		}

		if (MaxP_i == 0)
		{
			a = PhaseE1[MaxP_i][MaxP_j];
			b = PhaseE1[MaxP_i + 2][MaxP_j];
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i + 2][MaxP_j]) > 0.75 * 2.0 * Pi)
				b = PhaseE1[MaxP_i + 2][MaxP_j] + 2.0 * Pi;
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i + 2][MaxP_j]) < -0.75 * 2.0 * Pi)
				b = PhaseE1[MaxP_i + 2][MaxP_j] - 2.0 * Pi;//消除相位折叠
		}

		if (MaxP_i == N0 - 1)
		{
			a = PhaseE1[MaxP_i - 2][MaxP_j];
			b = PhaseE1[MaxP_i][MaxP_j];
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i - 2][MaxP_j]) > 0.75 * 2.0 * Pi)
				b = PhaseE1[MaxP_i][MaxP_j] - 2.0 * Pi;
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i - 2][MaxP_j]) < -0.75 * 2.0 * Pi)
				b = PhaseE1[MaxP_i][MaxP_j] + 2.0 * Pi;//消除相位折叠
		}

		if (((b - a) / 2.0 / Pi*lamda) < (2.0*ds))
			Ni = Vector3(cos(Pi / 2.0 - acos((b - a) / 2.0 / Pi*lamda / 2.0 / ds)),
				0,
				sin(Pi / 2.0 - acos((b - a) / 2.0 / Pi*lamda / 2.0 / ds)));

		if (((b - a) / 2.0 / Pi*lamda) >= (2.0*ds))
			Ni = Vector3(1, 0, 0);
		//Ni计算完成

		//下面计算Nj向量
		double c = 0, d = 0;
		if (MaxP_j > 0 & MaxP_j < N0 - 1)
		{
			c = PhaseE1[MaxP_i][MaxP_j - 1];
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i][MaxP_j - 1])>0.75 * 2.0 * Pi)
				c = PhaseE1[MaxP_i][MaxP_j - 1] + 2.0 * Pi;
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i][MaxP_j - 1]) < -0.75 * 2.0 * Pi)
				c = PhaseE1[MaxP_i][MaxP_j - 1] - 2.0 * Pi;//消除相位折叠

			d = PhaseE1[MaxP_i][MaxP_j + 1];
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i][MaxP_j + 1]) > 0.75 * 2.0 * Pi)
				d = PhaseE1[MaxP_i][MaxP_j + 1] + 2.0 * Pi;
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i][MaxP_j + 1]) < -0.75 * 2.0 * Pi)
				d = PhaseE1[MaxP_i][MaxP_j + 1] - 2.0 * Pi;//消除相位折叠
		}

		if (MaxP_j == 0)
		{
			c = PhaseE1[MaxP_i][MaxP_j];
			d = PhaseE1[MaxP_i][MaxP_j + 2];
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i][MaxP_j + 2]) > 0.75 * 2.0 * Pi)
				d = PhaseE1[MaxP_i][MaxP_j + 2] + 2.0 * Pi;
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i][MaxP_j + 2]) < -0.75 * 2.0 * Pi)
				d = PhaseE1[MaxP_i][MaxP_j + 2] - 2.0 * Pi;//消除相位折叠
		}

		if (MaxP_j == N0 - 1)
		{
			c = PhaseE1[MaxP_i][MaxP_j - 2];
			d = PhaseE1[MaxP_i][MaxP_j];
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i][MaxP_j - 2]) > 0.75 * 2.0 * Pi)
				d = PhaseE1[MaxP_i][MaxP_j] - 2.0 * Pi;
			if ((PhaseE1[MaxP_i][MaxP_j] - PhaseE1[MaxP_i][MaxP_j - 2]) < -0.75 * 2.0 * Pi)
				d = PhaseE1[MaxP_i][MaxP_j] + 2.0 * Pi;//消除相位折叠
		}

		if (((d - c) / 2.0 / Pi*lamda) < (2.0*ds))
			Nj = Vector3(0,
				cos(Pi / 2.0 - acos((d - c) / 2.0 / Pi*lamda / 2.0 / ds)),
				sin(Pi / 2.0 - acos((d - c) / 2.0 / Pi*lamda / 2.0 / ds)));

		if (((d - c) / 2.0 / Pi*lamda) >= (2.0*ds))
			Nj = Vector3(0, 1, 0);
		//Nj计算完成

		//下面开始计算最大场强点的传播方向向量并归一化
		Vector3 MaxDirection;
		MaxDirection = Vector3(-Ni.z / Ni.x, -Nj.z / Nj.y, 1);
		MaxDirection.Normalization();

		//新的局部坐标系的坐标轴向量
		Vector3 Ix, Iy, Iz;
		Iz = MaxDirection;
		Iy = Vector3(0,
			MaxDirection.z / pow((MaxDirection.y*MaxDirection.y + MaxDirection.z*MaxDirection.z),0.5),
			-MaxDirection.y / pow((MaxDirection.y*MaxDirection.y + MaxDirection.z*MaxDirection.z), 0.5));
		Ix = Iy.Cross(Iz);//叉积

		//P1是以MaxP点为中心的场分布坐标系，同时倾斜
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					P1[i][j] = Vector3(ds*(i - (N0 - 1) / 2), ds*(j - (N0 - 1) / 2), 0) -
						Vector3(ds*(MaxP_i - (N0 - 1) / 2), ds*(MaxP_j - (N0 - 1) / 2), 0);
					P1[i][j] = Vector3(P1[i][j].Dot(Ix),
						P1[i][j].Dot(Iy),
						P1[i][j].Dot(Iz));
				}
			}

		//下面开始估算束腰
		double Dx_limit, Dy_limit;
		if (MaxP_i > 0 & MaxP_i < N0 - 1)
		{
			Dx_limit = ds*(N0 - 1 - MaxP_i);
			if (Dx_limit > ds*(MaxP_i - 0))
				Dx_limit = ds*(MaxP_i - 0);//两者相比取小的
		}
		else  Dx_limit = 3 * ds;

		if (MaxP_j > 0 & MaxP_j < N0 - 1)
		{
			Dy_limit = ds*(N0 - 1 - MaxP_j);
			if (Dy_limit > ds*(MaxP_j - 0))
				Dy_limit = ds*(MaxP_j - 0);//两者相比取小的
		}
		else  Dy_limit = 3 * ds;

		double W0x_max = Vector3(Dx_limit / 2.5, 0, 0).Dot(Ix);
		W0x_max = fabs(W0x_max);//fabs才适用于小数的求绝对值

		double W0y_max = Vector3(0, Dy_limit / 2.5, 0).Dot(Iy);
		W0y_max = fabs(W0y_max);//fabs才适用于小数的求绝对值

		//下面开始计算X方向和Y方向的束腰
		//从峰值点向两边看去，寻找第一个谷值
		int ii0 = 0;
		if (MaxP_i == 0) ii0 = 0;
		else
		{
			for (int k = 0; k < MaxP_i; k++)
			{
				ii0 = k;
				if (AbsE1[MaxP_i - k - 1][MaxP_j]>AbsE1[MaxP_i - k][MaxP_j])
					break;//一找到就跳出循环
			}
		}

		int ii1 = MaxP_i;
		if (MaxP_i == N0 - 1) ii0 = N0 - 1;
		else
		{
			for (int k = MaxP_i; k < N0 - 1; k++)
			{
				ii1 = k;
				if (AbsE1[k + 1][MaxP_j]>AbsE1[k][MaxP_j])
					break;//一找到就跳出循环
			}
		}

		int jj0 = 0;
		if (MaxP_j == 0) jj0 = 0;
		else
		{
			for (int k = 0; k < MaxP_j; k++)
			{
				jj0 = k;
				if (AbsE1[MaxP_i][MaxP_j - k - 1]>AbsE1[MaxP_i][MaxP_j - k])
					break;//一找到就跳出循环
			}
		}

		int jj1 = MaxP_j;
		if (MaxP_j == N0 - 1) jj0 = N0 - 1;
		else
		{
			for (int k = MaxP_j; k < N0 - 1; k++)
			{
				jj1 = k;
				if (AbsE1[MaxP_i][k + 1]>AbsE1[MaxP_i][k])
					break;//一找到就跳出循环
			}
		}
		//先算Wx
		double Dx1 = fabs(Vector3(ii0*ds, 0, 0).Dot(Ix));
		double Dx2 = fabs(Vector3((ii1 - MaxP_i)*ds, 0, 0).Dot(Ix));
		double Wx1 = sqrt(Dx1*Dx1 / (1.0e-10 - log(AbsE1[MaxP_i - ii0][MaxP_j] / AbsE1[MaxP_i][MaxP_j])));
		double Wx2 = sqrt(Dx2*Dx2 / (1.0e-10 - log(AbsE1[ii1][MaxP_j] / AbsE1[MaxP_i][MaxP_j])));
		double Wx;
		if (fabs(Wx1) > 1.0e-5 & fabs(Wx2) > 1.0e-5)
		{
			Wx = Wx1;
			if (Wx1 > Wx2)
				Wx = Wx2;//取较小束腰值
		}
		else Wx = Wx1 + Wx2;

		if (Wx > W0x_max) Wx = W0x_max;

		//再算Wy
		double Dy1 = fabs(Vector3(0, jj0*ds, 0).Dot(Iy));
		double Dy2 = fabs(Vector3(0, (jj1 - MaxP_j)*ds, 0).Dot(Iy));
		double Wy1 = sqrt(Dy1*Dy1 / (1.0e-10 - log(AbsE1[MaxP_i][MaxP_j - jj0] / AbsE1[MaxP_i][MaxP_j])));
		double Wy2 = sqrt(Dy2*Dy2 / (1.0e-10 - log(AbsE1[MaxP_i][jj1] / AbsE1[MaxP_i][MaxP_j])));
		double Wy;
		if (fabs(Wy1) > 1.0e-5 & fabs(Wy2) > 1.0e-5)
		{
			Wy = Wy1;
			if (Wy1 > Wy2)
				Wy = Wy2;//取较小束腰值
		}
		else Wy = Wy1 + Wy2;

		if (Wy > W0y_max) Wy = W0y_max;

		//先确定相位与幅值的比例系数
		double Delta_Phase = PhaseE1[MaxP_i][MaxP_j]
			- arg(Gauss_E_Elliptical(frequency0, Wx, Wy, P1[MaxP_i][MaxP_j]));
		double Amplitude_Ratio = MaxE 
			/ abs(Gauss_E_Elliptical(frequency0, Wx, Wy, P1[MaxP_i][MaxP_j]))
			/ (Iy.Dot(Vector3(0, 1, 0)));

		//下面开始将此次分解的信息输入到Split_Info中
		Split_Info[k][0] = MaxP_i; Split_Info[k][1] = MaxP_j;
		Split_Info[k][2] = Ix.x; Split_Info[k][3] = Ix.y; Split_Info[k][4] = Ix.z;
		Split_Info[k][5] = Iy.x; Split_Info[k][6] = Iy.y; Split_Info[k][7] = Iy.z;
		Split_Info[k][8] = Iz.x; Split_Info[k][9] = Iz.y; Split_Info[k][10] = Iz.z;
		Split_Info[k][11] = Wx; Split_Info[k][12] = Wy;
		Split_Info[k][13] = Amplitude_Ratio; Split_Info[k][14] = Delta_Phase;
		//Split_Info.push_back(Split_Info_Single);//将Split_Info_Single推入Split_Info中

		Actual_SplitTimes = k + 1;//实际分解次数+1

		double Whole_Power1 = 0;//计算更新后的口面场总能量,初始化为0

		//下面开始从原始场分布中消去此次分解出的高斯波束
		//并进行剩余能量积分与分解阈值判定
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					//减去生成的第一个椭圆高斯分布（高斯分布根据原有场分布匹配相位和幅度）
					E1[i][j] = E1[i][j]
						- Amplitude_Ratio*exp(j0*Delta_Phase)
						*Gauss_E_Elliptical(frequency0, Wx, Wy, P1[i][j])
						*(Iy.Dot(Vector3(0,1,0)));//高斯波束场强在坐标轴Y方向的分量

					AbsE1[i][j] = abs(E1[i][j]);
					PhaseE1[i][j] = arg(E1[i][j]);
					//在减去该椭圆高斯波束分布的基础上更新E1(消去一次)

					Whole_Power1 = Whole_Power1 + abs(E1[i][j])*abs(E1[i][j]);
				}
			}

		Power_Remained[k] = Whole_Power1 / Whole_Power0;

		if (Power_Remained[k] <= Remained_Power_Ratio)
			break;//剩余能量小于Remained_Power_Ratio时，停止分解

		if (isnan(Power_Remained[k]))
		{
			Actual_SplitTimes = k;
			break;//分解出现NaN时，直接跳出
		}
			

	}
}

	//下面求点P处的场强值
complex<double> Calculate_SinglePoint(double frequency0, double ds, int N0,
	                                  int Actual_SplitTimes, vector <vector <double>> &Split_Info,
	                                  Vector3 P)
{   
	complex <double> j0(0, 1);//定义虚数单位j0

	complex<double> E_P=0;
	for (int k = 0; k < Actual_SplitTimes; k++)
	{
		int ii = Split_Info[k][0];
		int jj = Split_Info[k][1];

		Vector3 Gauss_Position,Axis_X, Axis_Y, Axis_Z;
		Gauss_Position=Vector3(ds*(ii - (N0 - 1) / 2), ds*(jj - (N0 - 1) / 2), 0);
		Axis_X = Vector3(Split_Info[k][2], Split_Info[k][3], Split_Info[k][4]);
		Axis_Y = Vector3(Split_Info[k][5], Split_Info[k][6], Split_Info[k][7]);
		Axis_Z = Vector3(Split_Info[k][8], Split_Info[k][9], Split_Info[k][10]);

		Vector3 P1;//求出P点在斜高斯波束中心坐标系下的坐标
		P1 = Vector3((P - Gauss_Position).Dot(Axis_X),
			         (P - Gauss_Position).Dot(Axis_Y),
			         (P - Gauss_Position).Dot(Axis_Z));
		
		double Wx = Split_Info[k][11]; double Wy = Split_Info[k][12];
		double Amplitude_Ratio = Split_Info[k][13]; double Delta_Phase = Split_Info[k][14];

		E_P = E_P + Gauss_E_Elliptical(frequency0, Wx, Wy, P1) * Amplitude_Ratio * exp(j0 * Delta_Phase);
	}

	return E_P;
}
