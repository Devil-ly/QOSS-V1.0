#include "PhaseUnwrap.h"

//一维相位解折叠（二维相位解折叠函数中要用到它）
//输入的Phase矩阵是折叠过的，由arg，即atan2函数得到，范围是（-Pi，Pi]
vector <double> Unwrap_1D(int N0, vector <double> Phase)
{
	vector <double> Phase1(N0);
	for (int k = 0; k < N0; k++)
	{
		Phase1[k] = Phase[k];
	}

	for (int k = 0; k < N0-1; k++)
	{
		if ((abs(Phase1[k] - Phase1[k + 1]) >= 0.6 * 2 * Pi) & (Phase1[k] > Phase1[k+1]))
		{
			for (int k1 = k+1; k1 < N0; k1++)
			{
				Phase1[k1] = Phase1[k1] + 2 * Pi;
			}
		}

		if ((abs(Phase1[k] - Phase1[k + 1]) >= 0.6 * 2 * Pi) & (Phase1[k] < Phase1[k + 1]))
		{
			for (int k1 = k + 1; k1 < N0; k1++)
			{
				Phase1[k1] = Phase1[k1] - 2 * Pi;
			}
		}
	}
	return Phase1;
}

//二维相位解折叠函数,对N0*N0相位矩阵进行修正
vector <vector<double>> Unwrap_2D(int N0, vector <vector <double>> Phase)
{
	double Middle = N0 / 2;
	int Middle_Point = floor(Middle);//找到中心点

	//新增加的一块，选择解折叠的过中间点Middle_Point的中脊线时，选用相位跳变次数多的
	vector <vector<double>> Phase1(N0, (vector <double>(N0, 0)));
	int RotationIndex = 0;//如果选择行就是0，选择列就是1
	int Index_i = 0; int Index_j = 0;//初始化
	for(int i = 0; i < N0 - 1; i++)
	{
		if (abs(Phase[Middle_Point][i + 1] - Phase[Middle_Point][i]) >= (2 * 0.6*Pi))
			Index_i = Index_i + 1;//中心点所在行的不连续点个数

		if (abs(Phase[i + 1][Middle_Point] - Phase[i][Middle_Point]) >= (2 * 0.6*Pi))
			Index_j = Index_j + 1;//中心点所在行的不连续点个数
	}

	if (Index_i >= Index_j)//选择中心点所在行
	{
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					Phase1[i][j] = Phase[i][j];//直接复制
				}
			}
	}
	else
	{
		RotationIndex = 1;//选择列，改为1
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					Phase1[i][j] = Phase[j][i];//将原矩阵转秩后复制
				}
			}
	}
	
	//由于二维Vector的矩阵中无法直接访问某一列，因此定义Phase2为Phase1的转秩
	//Phase2的某一行即为Phase1的某一列
	vector <vector<double>> Phase2(N0, (vector <double>(N0, 0)));
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
	{
		{
			Phase2[i][j] = Phase1[j][i];//复制并转秩
		}
	}

	//先进行Middle_Point所在行的解折叠
	vector <double> UnwrappedMiddleRow(N0);
	UnwrappedMiddleRow = Unwrap_1D(N0, Phase1[Middle_Point]);
	//以Phase[Middle_Point][Middle_Point]为基准值校准
	double delta1= UnwrappedMiddleRow[Middle_Point] - Phase1[Middle_Point][Middle_Point];
	for (int k = 0; k < N0; k++)
	{
		UnwrappedMiddleRow[k] = UnwrappedMiddleRow[k] - delta1;
	}

	//下面对Phase的各个列进行解折叠
	vector <vector<double>> Phase3(N0, (vector <double>(N0, 0)));
	vector <vector<double>> Phase4(N0, (vector <double>(N0, 0)));
	for (int k = 0; k < N0; k++)
	{
		Phase3[k] = Unwrap_1D(N0, Phase2[k]);
	}
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
		{
			{
				Phase4[i][j] = Phase3[j][i];//Phase3转秩得到Phase4,Phase4即为对Phase解折叠的结果
			}
		}

	vector <vector<double>> Phase5(N0, (vector <double>(N0, 0)));
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
		{
			{
				double delta2 = Phase4[Middle_Point][j] - UnwrappedMiddleRow[j];
				Phase5[i][j] = Phase4[i][j] - delta2;
				//将各列校正至中间行上
			}
		}
	//至此完成最初的二维相位解折叠，得到略粗糙的解折叠后的二维相位分布Phase5
	
	//下面开始进行后续的平滑
	//QualityMap是寻找每一列上的相位不连续点
	vector <vector<int>> QualityMap(N0, (vector <int>(N0, 0)));
	vector <vector<int>> QualityMap_Expanded(N0, (vector <int>(N0, 0)));
	//先验条件是第1列和N0-1列相位连续，而且中心行上的相位连续（UnwrappedMiddleRow）
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0-1; j++)
		{
			{
				QualityMap[i][j] = 0;//没有不连续就标0
				QualityMap_Expanded[i][j] = 0;//没有不连续就标0

				if(abs(Phase5[i][j + 1] - Phase5[i][j])>=(2*0.6*Pi))
				QualityMap[i][j] = 1;//有不连续就标1
			}
		}

	//去除点范围的扩展
	int BasicRange = 7;//用于不连续点向两边的扩展
	int LittleRange = 5;//用于不连续点向四周的小扩展
	for (int i = 0; i < N0; i++)
		for (int j = 0; j < N0; j++)
		{
			{

				if (QualityMap[i][j] == 1)
				{
					//靠近左边缘
					if (j < BasicRange)//设定阈值
					{
						for (int k = 0; k < j + BasicRange; k++)
						{
							QualityMap_Expanded[i][k] = 1;//扩大不连续范围
						}
					}
					//靠近右边缘
					if (j > ((N0 - 1) - BasicRange))//设定阈值
					{
						for (int k = j - BasicRange; k < N0; k++)
						{
							QualityMap_Expanded[i][k] = 1;//扩大不连续范围
						}
					}
					//在中间
					if ((j >= BasicRange) & (j <= ((N0 - 1) - BasicRange)))//设定阈值
					{
						for (int k1 = j - BasicRange; k1 <= j + BasicRange; k1++)
						{
							QualityMap_Expanded[i][k1] = 1;//扩大不连续范围

							if ((i >= LittleRange) & (i <= ((N0 - 1) - LittleRange)))//向四周的小扩展
							{
								for (int k2 = i - LittleRange; k2 <= i + LittleRange; k2++)
								{
									QualityMap_Expanded[k2][k1] = 1;//向上下扩大不连续范围
								}
							}
						}

					}
				}
					//if语句结束
			}
		}

	for (int i = 0; i < N0; i++)
		{
			QualityMap_Expanded[i][0] = 0;
			QualityMap_Expanded[i][N0-1] = 0;//两侧边缘不存在不连续，不然会影响后续插值
			QualityMap_Expanded[Middle_Point][i] = 0;//中间一行不存在不连续
		}

	//开始插值，分为上下俩部分插值
	//先插0到Middle_Point-1行
	for (int i = 0; i < Middle_Point; i++)
		for (int j = 0; j < N0; j++)
		{
			{
				if (QualityMap_Expanded[i][j] == 1)
				{
					double Dleft = 1;//这个点左边的点QualityMap_Expanded肯定不是1
					double Ddown = 1;//这个点下面的点QualityMap_Expanded肯定不是1，除非这个点在最下面Ddown不存在
					double Dright = 1; double Dup = 1; //这三个是初始化

					for (int k = j + 1; k < N0; k++)
					{
						if (QualityMap_Expanded[i][k] == 0)
						{
							Dright = k - j;//向右找并算出距离
							break;//找到后跳出循环
						}
					}

					for (int k = i + 1; k <= Middle_Point; k++)
					{
						if (QualityMap_Expanded[k][j] == 0)
						{
							Dup = k - i;//向上找并算出距离
							break;//找到后跳出循环
						}
					}

					//如果不连续点在最下面，此时phase5(i,j)由左右上三个点以距离反比插值得到
					if (i == 0)
						Phase5[i][j] = 1/Dleft / (1 / Dleft + 1 / Dright + 1 / Dup)*Phase5[i][j - Dleft]
						             + 1/Dright / (1 / Dleft + 1 / Dright + 1 / Dup)*Phase5[i][j + Dright]
						             + 1/Dup / (1 / Dleft + 1 / Dright + 1 / Dup)*Phase5[i + Dup][j];
					else 
						Phase5[i][j] = 1/Dleft / (1 / Dleft + 1 / Dright + 1 / Dup + 1 / Ddown)*Phase5[i][j - Dleft]
							         + 1/Dright / (1 / Dleft + 1 / Dright + 1 / Dup + 1 / Ddown)*Phase5[i][j + Dright]
							         + 1/Dup / (1 / Dleft + 1 / Dright + 1 / Dup + 1 / Ddown)*Phase5[i + Dup][j]
						             + 1/Ddown / (1 / Dleft + 1 / Dright + 1 / Dup + 1 / Ddown)*Phase5[i - Ddown][j];

					//经过以上插值后，该点的不连续性被消除
					QualityMap_Expanded[i][j] = 0;
				}

			}
		}

	//再插N0-1到Middle_Point+1行
	for (int i = N0-1; i > Middle_Point; i--)
		for (int j = 0; j < N0; j++)
		{
			{
				if (QualityMap_Expanded[i][j] == 1)
				{
					double Dleft = 1;//这个点左边的点QualityMap_Expanded肯定不是1
					double Dup = 1;//这个点下面的点QualityMap_Expanded肯定不是1，除非这个点在最上面Dup不存在
					double Dright = 1; double Ddown = 1; //这三个是初始化

					for (int k = j + 1; k < N0; k++)
					{
						if (QualityMap_Expanded[i][k] == 0)
						{
							Dright = k - j;//向右找并算出距离
							break;//找到后跳出循环
						}
					}

					for (int k = i - 1; k >= Middle_Point; k--)
					{
						if (QualityMap_Expanded[k][j] == 0)
						{
							Ddown = i - k;//向下找并算出距离
							break;//找到后跳出循环
						}
					}

					//如果不连续点在最上面，此时phase5[i][j]由左右下三个点以距离反比插值得到
					if (i == N0 - 1)
						Phase5[i][j] = 1 / Dleft / (1 / Dleft + 1 / Dright + 1 / Ddown)*Phase5[i][j - Dleft]
						             + 1 / Dright / (1 / Dleft + 1 / Dright + 1 / Ddown)*Phase5[i][j + Dright]
						             + 1 / Ddown / (1 / Dleft + 1 / Dright + 1 / Ddown)*Phase5[i - Ddown][j];
					else//否则由左右上下四个点插值得到
						Phase5[i][j] = 1 / Dleft / (1 / Dleft + 1 / Dright + 1 / Dup + 1 / Ddown)*Phase5[i][j - Dleft]
						             + 1 / Dright / (1 / Dleft + 1 / Dright + 1 / Dup + 1 / Ddown)*Phase5[i][j + Dright]
						             + 1 / Dup / (1 / Dleft + 1 / Dright + 1 / Dup + 1 / Ddown)*Phase5[i + Dup][j]
						             + 1 / Ddown / (1 / Dleft + 1 / Dright + 1 / Dup + 1 / Ddown)*Phase5[i - Ddown][j];
					//经过以上插值后，该点的不连续性被消除
					QualityMap_Expanded[i][j] = 0;
				}

			}
		}

//下面看这个矩阵之前有没有被转秩过，标志是RotationIndex，0代表没有被转秩过，1代表被转秩过
	if (RotationIndex == 0)
		return Phase5;
	else 
	{
		vector <vector<double>> Phase6(N0, (vector <double>(N0, 0)));
		for (int i = 0; i < N0; i++)
			for (int j = 0; j < N0; j++)
			{
				{
					Phase6[i][j] = Phase5[j][i];//将解缠绕后的矩阵转秩后返回
				}
			}
		return Phase6;
	}

}
