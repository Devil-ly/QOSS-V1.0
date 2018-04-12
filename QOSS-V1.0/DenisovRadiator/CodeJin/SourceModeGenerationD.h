#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <math.h>//包含贝塞尔函数
#include "Mathematical_Functions.h"//包含阶乘、贝塞尔函数的一阶导数等基本数学小函数
#include "Vector3.h"
//Vector3.h中已经申明过Vector3.h了
//由于上面的文件中已经引用过"Constant_Val.h"，则下面不需要再添加include申明了
//#include "Constant_Val.h"//数学常量H文件
using namespace std;
/*
*	created by XD 2017/10/20
	modified by JM 2017 12
	为Denisov提供波导模式参数 将进行精简
*   单位默认为米
*   function 创建圆波导TEmn模式以及矩形波导的场分布
*/

#ifndef SOURCEMODEGENERATIOND_H  // 全大写
#define SOURCEMODEGENERATIOND_H

namespace calculation
{ // 命令空间 避免命名重复

	class SourceModeGenerationD //类名保持和头文件名字相同
	{
	public:
		SourceModeGenerationD
		   (int _SourceKind=1,//这是种类，1是低阶TE模式；2是高阶TE模式；3是矩形波导模式
			int _SourceType=1,//这是TE模式或者TM模式，TE为1，TM为2
            int _Rotation=0,//左右旋，不存在的为0，左旋为1，右旋为2
			int _m=0 ,//TEmn
			int _n=1 ,//TEmn
			double _Frequency=44.0e9,//频率
			double _Radius=64.0e-3,//波导半径
			double _Height=0,//矩形波导的长度
			double _Width=0,//矩形波导的宽度,长度要大于宽度
			int _N=250//采样点
		);// 默认构造函数

		// 若有继承 一定要加virtual
		~SourceModeGenerationD() {
			// 若有分配了内存记得释放
			// if(p) {
			//   delete p;
			//   p = NULL;
			// }
		};//析构函数
	
		void SetSource_Circular(int sourcekind, int sourcetype, int rotation,
			                    int m0, int n0, double frequency, double radius);
		//设置圆波导模式参数

		void SetSource_Rectangle(int sourcekind, int sourcetype, int m0, int n0,
			                     double frequency, double height, double width);
		//设置矩形波导模式参数

		void SetOutputProperty(int n1);
		//设置输出的矩形场分布参数初始化
		//n1是指设定采样点数目，只能是偶数，因为采样点不能包含原点，否则公式无法计算

		bool SetWaveguideMode(int m0, int n0);
		//简便设置M和N的函数
		
		bool FieldCalculation_Circular();
		//设置圆波导模式场输出,n只能是偶数，因为采样点不能包含原点，否则公式无法计算

		bool FieldCalculation_CircularT();
		//计算FDTD仿真需要的切向场分布

		bool FieldCalculation_Rectangle();
		//设置矩形波导模式场输出,n只能是偶数，因为采样点不能包含原点，否则公式无法计算
		//已完成20171106

		bool SetFieldAmplitude(double K);//设置场强归一化最大幅值（EX EY EZ中的最大值）

		bool GetCircularWaveguideProperty(double &phi1,double &theta1,double &Rc,double &Lc);
		//获得圆波导诸如布里渊角与焦散圆半径等参数并输出
		//phi1是布里渊角；theta1是方向矢量在横向的投影与波导切线的夹角
		//Rc是焦散圆的半径
		//20171109完成并验证正确

		bool GetCircularConverterLauncherTriangle_TE0n(Vector3 &P1,Vector3 &P2, Vector3 &P3);
		//李赟画二维示意图中的用于画辐射器三角形的三个点

		bool GetCircularSystemOpticalPath_TE0n_2Mirrors(Vector3 &P4,
			Vector3 &V1, double &Length1, double &theta1,
			Vector3 &V2, double &Length2, double &theta2,
			Vector3 &V3);
		//李赟画二维示意图中的用于画辐射器三角形的光路设置函数(双镜面)

		bool GetCircularSystemOpticalPath_TE0n_3Mirrors(Vector3 &P4,
			Vector3 &V1, double &Length1, double &theta1,
			Vector3 &V2, double &Length2, double &theta2,
			Vector3 &V3, double &Length3, double &theta3,
			Vector3 &V4);
		//李赟画二维示意图中的用于画辐射器三角形的光路设置函数(三镜面)

		bool GetCircularSystemOpticalPath_TE0n_4Mirrors(Vector3 &P4,
			Vector3 &V1, double &Length1, double &theta1,
			Vector3 &V2, double &Length2, double &theta2,
			Vector3 &V3, double &Length3, double &theta3,
			Vector3 &V4, double &Length4, double &theta4,
			Vector3 &V5);
		//李赟画二维示意图中的用于画辐射器三角形的光路设置函数(四镜面)

		void GetEX(vector <vector <complex <double>>> &EX0);//输出EX
		void GetHX(vector <vector <complex <double>>> &HX0);//输出HX
		void GetEY(vector <vector <complex <double>>> &EY0);//输出EY
		void GetHY(vector <vector <complex <double>>> &HY0);//输出HY
		void GetEZ(vector <vector <complex <double>>> &EZ0);//输出EZ
		void GetHZ(vector <vector <complex <double>>> &HZ0);//输出HZ
		bool GetJ_R(vector<complex<double>> &_HPhi, vector<complex<double>> &_Hz, int _Nphi);//输出HPhi




		// 若有分配内存 最好重写赋值构造和重载 = 
		//TraceLight(const TraceLight& traceLight) {
//
	//	}
	//	TraceLight operator = (const TraceLight& traceLight) {
//
	//	}

		/*
		* 简单函数功能介绍 + 参数说明 以及返回参数说明
		*/
	//	void fun() {};

	private:

		int SourceKind;//这是种类，1是低阶TE模式；2是高阶TE模式；3是矩形波导模式
		int SourceType;//这是TE模式或者TM模式，TE为1，TM为2
		int Rotation;//左右旋，不存在的为0，左旋为1，右旋为2
		int m;//TEmn
		int n;//TEmn
		double Frequency;//频率
		double Radius;//波导半径
		double Height;//矩形波导的长度，一般长度大于宽度
		double Width;//矩形波导的宽度
		int N;//离散点采样间个数
		double ds;//离散点采样间距
		double Lc;//Cut 高度
		vector <vector <complex <double>>>  EX,HX,EY,HY,EZ,HZ;
		
		vector <complex<double>> HPhi_r;
		vector <complex<double>> Hz_r;
		//定义输出矩阵复数形式（6个参量矩阵得到场分布）

	};

}

#endif // TRACELIGHT_H