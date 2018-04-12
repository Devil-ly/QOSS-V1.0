#include "PhaseCorrection.h"

calculation::PhaseCorrection::PhaseCorrection(double _Frequency, double _Lamda, double _Target_W0,
	int _N_Mirror, int _N_InField, double _InField_ds) :
	Frequency(_Frequency), Lamda(_Lamda), Target_W0(_Target_W0),
	N_Mirror(_N_Mirror), N_InField(_N_InField), InField_ds(_InField_ds)
{
	D_in = 0;
	D_out = 0;
	Theta = 0;
	//初始化
	/*
	Initial_Mirror.resize(N_Mirror); In_Field.resize(N_Input_Field);
	for (int i = 0; i < N_Mirror; i++)
	{
		Initial_Mirror[i].resize(N_Mirror);
	}//矩阵的resize

	for (int i = 0; i < N_Input_Field; i++)
	{
		In_Field[i].resize(N_Input_Field);
	}//矩阵的resize
	*/
}

calculation::PhaseCorrection::~PhaseCorrection()
{
}

//基本参数设置
void calculation::PhaseCorrection::Set(double _Frequency, 
	double _Target_W0, int _N_Mirror, int _N_InField,
	const vector <vector <Vector3>> &_Initial_Mirror,
	const vector <vector <complex<double>>> &_InField_EX,
	const vector <vector <complex<double>>> &_InField_EY,
	double _InField_ds)
{
	Frequency = _Frequency;
	Lamda = C_Speed / Frequency;
	Target_W0 = _Target_W0;

	N_Mirror = _N_Mirror;
	N_InField = _N_InField;
	InField_ds = _InField_ds;

	//李赟传递给我的矩阵就是N*N的，因此不需要resize,直接使用就行了
	Initial_Mirror = _Initial_Mirror;
	InField_EX = _InField_EX;
	InField_EY = _InField_EY;

	/*ofstream outfilex1("PhsCorExEy.txt");
	for (int i = 0; i < 201; i++)
	{
		for (int j = 0; j < 201; j++)
		{
			outfilex1 //<< abs(InField_EX[i][j]) << " " 
				//<< arg(InField_EX[i][j]) << " "
				<< abs(InField_EY[i][j]) << " "
				<< arg(InField_EY[i][j]) << "\n";
		}
	}
	outfilex1.close();*/

	InField_EX1.resize(N_InField); InField_EY1.resize(N_InField);
	for (int i = 0; i < N_InField; i++)
	{
		InField_EX1[i].resize(N_InField);
		InField_EY1[i].resize(N_InField);
	}//矩阵的resize
}

//基本坐标系参数设置（在此之前要先运行Set函数）
void calculation::PhaseCorrection::SetCoordinate(Vector3 _InField_Position, Vector3 _InField_X, Vector3 _InField_Y, Vector3 _InField_Z,
	Vector3 _Mirror_Position, Vector3 _Mirror_X, Vector3 _Mirror_Y, Vector3 _Mirror_Z,
	Vector3 _Reflect_Point, Vector3 _Reflect_Point_Normal)
{
	InField_Position = _InField_Position;
	InField_X = _InField_X;InField_Y = _InField_Y;InField_Z = _InField_Z;

	Mirror_Position = _Mirror_Position;
	Mirror_X = _Mirror_X; Mirror_Y = _Mirror_Y; Mirror_Z = _Mirror_Z;

	Reflect_Point = _Reflect_Point;
	Reflect_Point_Normal = _Reflect_Point_Normal;

	D_in = (_Reflect_Point - InField_Position).Length();//反射点减去入射场中心点，得到D_in
	//D_out = _D_out;

	//求出主射线在反射面反射点上的反射角度Theta值
	Theta = acos(InField_Z.Dot(Reflect_Point_Normal));
	if (Theta > (Pi / 2.0)) Theta = Pi - Theta;//必须保证Theta小于Pi/2

	OutField_Z = InField_Z - Reflect_Point_Normal.operator*(2.0*(InField_Z.Dot(Reflect_Point_Normal)));
	OutField_Y = OutField_Z.Cross(InField_Z);
	OutField_Y.Normalization();
	OutField_X = OutField_Y.Cross(OutField_Z);
	OutField_X.Normalization();//得到输出波束的局部坐标系

	//这里需要涉及到三个面的坐标变换问题，要和李赟商量
	//首先要将镜面的三维坐标转换为输入场的局部坐标系下和输出高斯波束的局部坐标系下
	//其次要保证主极化是Ey,这就意味着需要对输入场进行主极化的坐标变换，生成InField_X1和InField_Y1，而InField_Z不变
	//找到InField_EX和InField_EY最大值的点，确定主极化
	double MaxEX = 0, MaxEY = 0;//初始化
	//int MaxEX_i = 0, MaxEY_i = 0;
	//int MaxEX_j = 0, MaxEY_j = 0;
	int ii = 0, jj = 0;

	//首先找到场强最大值,然后找到场强最大值所在的ii,jj
	for (int i = 0; i < N_InField; i++)
		for (int j = 0; j < N_InField; j++)
		{
			{
				if (abs(InField_EX[i][j]) > MaxEX)
				{
					MaxEX = abs(InField_EX[i][j]);
					//MaxEX_i = i;
					//MaxEX_j = j;
				}

				if (abs(InField_EY[i][j]) > MaxEY)
				{
					MaxEY = abs(InField_EY[i][j]);
					//MaxEY_i = i;
					//MaxEY_j = j;
				}
			}
		}
	//下面开始对比
	//ii = MaxEX_i; jj = MaxEX_j;



	//20180314权宜之计，只取ExEy中的较大值所在坐标系为EY1，用于后续计算
	if (MaxEY > MaxEX)
	{
		//ii = MaxEY_i; jj = MaxEY_j;
		InField_Z1 = InField_Z;//Z坐标不变
		InField_Y1 = InField_Y;
		InField_X1 = InField_X;
		for (int i = 0; i < N_InField; i++)
			for (int j = 0; j < N_InField; j++)
			{
				{
					InField_EX1[i][j] = InField_EX[i][j];
					InField_EY1[i][j] = InField_EY[i][j];
				}
			}
	}

	if (MaxEX >= MaxEY)
	{
		InField_Z1 = InField_Z;//Z坐标不变
		InField_Y1 = InField_X;
		InField_X1 = InField_Y1.Cross(InField_Z1);
		for (int i = 0; i < N_InField; i++)
			for (int j = 0; j < N_InField; j++)
			{
				{
					InField_EY1[i][j] = InField_EX[i][j];
					InField_EX1[i][j] = -InField_EY[i][j];//反方向
				}
			}
	}

	/*
	//后续应将EX和EY分开算，这里是权宜之计
	//下面在这一点完成InField_X1,InField_Y1和InField_Z1的设置
	InField_Z1 = InField_Z;//Z坐标不变
	//将InField_Y1方向设为主极化方向
	InField_Y1 = InField_X.operator*(abs(InField_EX[ii][jj])) 
		       + InField_Y.operator*(abs(InField_EY[ii][jj]));
	InField_Y1.Normalization();//归一化

	InField_X1 = InField_Y1.Cross(InField_Z1);//InField_Y1叉乘InField_Z1得到InField_X1

	//下面生成对应的InField_EX1和InField_EY1
	double RatioX_X1 = InField_X.Dot(InField_X1); double RatioX_Y1 = InField_X.Dot(InField_Y1);
	double RatioY_X1 = InField_Y.Dot(InField_X1); double RatioY_Y1 = InField_Y.Dot(InField_Y1);
	for (int i = 0; i < N_InField; i++)
		for (int j = 0; j < N_InField; j++)
		{
			{
				InField_EX1[i][j] = RatioX_X1*InField_EX[i][j] + RatioY_X1*InField_EY[i][j];
				InField_EY1[i][j] = RatioX_Y1*InField_EX[i][j] + RatioY_Y1*InField_EY[i][j];
			}
		}
	*/
}

//设置需要修正的镜面
void calculation::PhaseCorrection::SetMirror(vector<vector<Vector3>>& _Mirror)
{
	Initial_Mirror = _Mirror;
}

//单次修正,只修正一次（修正后，再设置新的镜面为Initial_Mirror，如此重复）
vector<vector<Vector3>> calculation::PhaseCorrection::Single_Correction()
{
	//首先，根据出射高斯束腰大小与入射波信息得到D_out
	//先分解入射场，并求出其传播至反射面处的束腰值
	int Infield_splittimes = 0;
	vector <vector <double>> Infield_splitinfo;
	Field_Split(Frequency, InField_ds, N_InField, InField_EY1, Infield_splittimes, Infield_splitinfo);
	//只计算主极化InField_EY1

	//下面估算束腰
	vector<Vector3> Target_LineX(N_InField), Target_LineY(N_InField);
	vector<double> Target_LineX_E(N_InField), Target_LineY_E(N_InField);
	for (int i = 0; i < N_InField; i++)
	{
		Target_LineX[i] = Vector3((i - (N_InField - 1) / 2.0)*InField_ds, 0, D_in);
		Target_LineX_E[i] = abs(Calculate_SinglePoint(Frequency, InField_ds, 
			N_InField, Infield_splittimes, Infield_splitinfo, Target_LineX[i]));

		Target_LineY[i] = Vector3(0, (i - (N_InField - 1) / 2.0)*InField_ds, D_in);
		Target_LineY_E[i] = abs(Calculate_SinglePoint(Frequency, InField_ds,
			N_InField, Infield_splittimes, Infield_splitinfo, Target_LineY[i]));

	}



	//找到Target_LineX、Target_LineY的最大值点
	double Target_LineX_MaxE = 0, Target_LineY_MaxE = 0;
	int Target_LineX_MaxPoint, Target_LineY_MaxPoint;
	for (int i = 0; i < N_InField; i++)
	{
		if (Target_LineX_E[i] > Target_LineX_MaxE)
		{
			Target_LineX_MaxE = Target_LineX_E[i];
			Target_LineX_MaxPoint = i;
		}
	}

	for (int j = 0; j < N_InField; j++)
	{
		if (Target_LineY_E[j] > Target_LineY_MaxE)
		{
			Target_LineY_MaxE = Target_LineY_E[j];
			Target_LineY_MaxPoint = j;
		}
	}

	int iix0 = 0;
	for (int k = 0; k < Target_LineX_MaxPoint; k++)
	{
		iix0 = k;
		if ((Target_LineX_E[Target_LineX_MaxPoint - k - 1]<= (1.0 / Universal_Constant_e*Target_LineX_MaxE))
	        & (Target_LineX_E[Target_LineX_MaxPoint - k]>(1.0 / Universal_Constant_e*Target_LineX_MaxE)))
			break;//一找到就跳出循环
	}

	int iix1 = Target_LineX_MaxPoint;
	for (int k = Target_LineX_MaxPoint; k < N_InField - 1; k++)
	{
		iix1 = k;
		if ((Target_LineX_E[k + 1] <= (1.0 / Universal_Constant_e*Target_LineX_MaxE))
			& (Target_LineX_E[k]>(1.0 / Universal_Constant_e*Target_LineX_MaxE)))
			break;
	}

	int jjy0 = 0;
	for (int k = 0; k < Target_LineY_MaxPoint; k++)
	{
		jjy0 = k;
		if ((Target_LineY_E[Target_LineY_MaxPoint - k - 1] <= (1.0 / Universal_Constant_e*Target_LineY_MaxE))
			& (Target_LineY_E[Target_LineY_MaxPoint - k]>(1.0 / Universal_Constant_e*Target_LineY_MaxE)))
			break;//一找到就跳出循环
	}

	int jjy1 = Target_LineY_MaxPoint;
	for (int k = Target_LineY_MaxPoint; k < N_InField - 1; k++)
	{
		jjy1 = k;
		if ((Target_LineY_E[k + 1] <= (1.0 / Universal_Constant_e*Target_LineY_MaxE))
			& (Target_LineY_E[k]>(1.0 / Universal_Constant_e*Target_LineY_MaxE)))
			break;//一找到就跳出循环
	}
	//得到在反射面处的入射场束腰的估算值
	double W_TargetPoint = InField_ds*(iix0 + (iix1 - Target_LineX_MaxPoint)
		                    + jjy0 + (jjy1 - Target_LineY_MaxPoint))/4.0;

	//假定在反射点处，入射波束腰等于反射波束腰，则逆向传播的高斯波束在此处束腰也是W_TargetPoint
	//由此可以根据高斯波束束腰公式反算D_out
	D_out = Pi*Target_W0 / Lamda*pow(W_TargetPoint*W_TargetPoint - Target_W0*Target_W0, 0.5);

	//下面开始高斯波束逆向传播到镜面上的相位
	Vector3 Gauss_Position = Reflect_Point + OutField_Z.operator*(D_out);
	vector <vector<double>> Unwrapped_Gauss_Phase(N_Mirror, vector<double>(N_Mirror, 0));
	for (int i = 0; i < N_Mirror; i++)
		for (int j = 0; j < N_Mirror; j++)
		{
			{
				Vector3 Position1 = Mirror_Position
					+ Mirror_X.operator*(Initial_Mirror[i][j].x)
					+ Mirror_Y.operator*(Initial_Mirror[i][j].y)
					+ Mirror_Z.operator*(Initial_Mirror[i][j].z);

				Vector3 Position2((Position1.operator-(Gauss_Position)).Dot(OutField_X),
					(Position1.operator-(Gauss_Position)).Dot(OutField_Y),
					(Position1.operator-(Gauss_Position)).Dot(OutField_Z));
				Unwrapped_Gauss_Phase[i][j] = Gauss_Phase_Circular(Frequency, Target_W0, Position2);
			}
		}


	//下面求In_Field传到镜面上的相位并对其进行解折叠
	vector <vector<double>> Mirror_Amplitude(N_Mirror, vector<double>(N_Mirror, 0));
	vector <vector<double>> Wrapped_Mirror_Phase(N_Mirror, vector<double>(N_Mirror, 0));
	for (int i = 0; i < N_Mirror; i++)
		for (int j = 0; j < N_Mirror; j++)
		{
			{
				Vector3 Position1 = Mirror_Position
					+ Mirror_X.operator*(Initial_Mirror[i][j].x)
					+ Mirror_Y.operator*(Initial_Mirror[i][j].y)
					+ Mirror_Z.operator*(Initial_Mirror[i][j].z);
				Vector3 Position2((Position1.operator-(InField_Position)).Dot(InField_X1),
					(Position1.operator-(InField_Position)).Dot(InField_Y1),
					(Position1.operator-(InField_Position)).Dot(InField_Z1));
				//20180322测试
				Mirror_Amplitude[i][j]= abs(Calculate_SinglePoint(Frequency, InField_ds, N_InField,
					Infield_splittimes, Infield_splitinfo, Position2));
				Wrapped_Mirror_Phase[i][j] = arg(Calculate_SinglePoint(Frequency, InField_ds, N_InField,
					Infield_splittimes, Infield_splitinfo, Position2));
			}
		}

	vector <vector<double>> Unwrapped_Mirror_Phase(N_Mirror, vector<double>(N_Mirror, 0));
	//检查相位修正Unwrap_2D函数
	ofstream OutFile0;
	OutFile0.open("Original_Distribution.txt");//关联输出文件
	for (int i = 0; i < N_Mirror; i++)
		for (int j = 0; j < N_Mirror; j++)
		{
		  OutFile0 << Mirror_Amplitude[i][j] << " " << Wrapped_Mirror_Phase[i][j] << endl;
		}
	OutFile0.close();//输出完毕，关闭文档
	

	Unwrapped_Mirror_Phase= Unwrap_2D(N_Mirror, Wrapped_Mirror_Phase);
	//完成相位的解折叠

	ofstream OutFile1;
	OutFile1.open("Unwrapped_Mirror_Phase.txt");//关联输出文件
	for (int i = 0; i < N_Mirror; i++)
	for (int j = 0; j < N_Mirror; j++)
   {
	OutFile1 << Unwrapped_Mirror_Phase[i][j] << endl;
	}
	OutFile1.close();//输出完毕，关闭文档
	

	//对于初次修正且只修正一次，可以使用固定入射角Theta进行修正
	vector <vector<double>> Delta(N_Mirror, vector<double>(N_Mirror, 0));
	for (int i = 0; i < N_Mirror; i++)
		for (int j = 0; j < N_Mirror; j++)
		{
			{
				Delta[i][j] = (Unwrapped_Gauss_Phase[i][j] - Unwrapped_Mirror_Phase[i][j])
					*Lamda / 4.0 / Pi / cos(Theta);
			}
		}
	
	double Delta_Middle = Delta[floor(N_Mirror / 2.0)][floor(N_Mirror / 2.0)];
	vector <vector<Vector3>> Corrected_Mirror(N_Mirror, vector<Vector3>(N_Mirror, 0));
	for (int i = 0; i < N_Mirror; i++)
		for (int j = 0; j < N_Mirror; j++)
		{
			{
				Delta[i][j] = Delta[i][j]- Delta_Middle;//使中间的Delta值归零
				Corrected_Mirror[i][j] = Initial_Mirror[i][j] + Vector3(0, 0, Delta[i][j]);
			}
		}
	return Corrected_Mirror;
}






