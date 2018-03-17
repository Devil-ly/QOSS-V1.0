#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>//包含贝塞尔函数
#include "Gaussian_Beam_Circular.h"//圆高斯波束定义
#include "BeamPropagation.h"//口面场电磁计算程序，使用高斯波束分解拟合新算法
//vector用于存放数组与矩阵数据，已在BeamPropagation.h中申明
#include "PhaseUnwrap.h"//相位矩阵解折叠函数
#include "../util/Vector3.h"
//Vector3.h中已经申明过Vector3.h了
//由于上面的文件中已经引用过"Constant_Val.h"，则下面不需要再添加include申明了
//#include "Constant_Val.h"//数学常量H文件

using namespace std;
/*
*	created by XD 2018/02/05
*   单位默认为米
*   function 生成相位修正面文件
*/

#ifndef PHASECORRECTION_H // 全大写
#define PHASECORRECTION_H

namespace calculation//还是在Calculation命名空间中

{ // 命令空间 避免命名重复

	class PhaseCorrection
	{
	public:

		PhaseCorrection
			(double _Frequency = 44.0e9,//频率
				double _Lamda = C_Speed / 44.0e9,//波长
				double _Target_W0 = 0.01,//目标高斯束腰宽度
				int _N_Mirror = 101,//定义N*N的修正面矩阵
				int _N_InField = 101,//定义相位修正面入射口面场的离散点N_Input_Field*N_Input_Field
				double _InField_ds=0.001
				);

		~PhaseCorrection();

		//参数设置（先运行）
		void Set(double _Frequency, double _Target_W0, int _N_Mirror, int _N_InField,
			vector <vector <Vector3>> &_Initial_Mirror,
			vector <vector <complex<double>>> &_InField_EX,
			vector <vector <complex<double>>> &_InField_EY,
			double _InField_ds);

		//基本坐标系参数设置（在此之前要先运行Set函数）
		void SetCoordinate(Vector3 _InField_Position, Vector3 _InField_X, Vector3 _InField_Y, Vector3 _InField_Z,
			Vector3 _Mirror_Position, Vector3 _Mirror_X, Vector3 _Mirror_Y, Vector3 _Mirror_Z,
			Vector3 _Reflect_Point, Vector3 _Reflect_Point_Normal);

		//设置需要修正的镜面
		void SetMirror(vector <vector <Vector3>> &_Mirror );

		//单次修正
		vector <vector <Vector3>> Single_Correction();


	private:
		double Frequency;//频率
		double Lamda;//波长
		double Target_W0;//目标高斯波束的束腰
		double D_in;//输入口面场到反射面的距离（中心点到中心点）
		double D_out;//输出窗口到反射面的距离（中心点到中心点）
		//不能手动设置因为修正时需要另行确定
		int N_Mirror;//镜面的网格数量（N_Mirror*N_Mirror）
	    int N_InField;//输入口面场的网格数量（N_Input_Field*N_Input_Field）
		vector <vector <Vector3>> Initial_Mirror;//定义初始镜面的点矩阵
		//点矩阵的点坐标都是局部坐标系Mirror_X, Mirror_Y, Mirror_Z下的
		vector <vector <complex<double>>> InField_EX,InField_EY;
		//定义输入场的场分布(InField_X和InField_Y方向分量)（从-x,-y到x,y）
		vector <vector <complex<double>>> InField_EX1,InField_EY1;
		//调整后的输入场的场分布(InField_X1和InField_Y1方向分量)
		double InField_ds;//定义输入场的点间隔
		Vector3 InField_Position, InField_X, InField_Y, InField_Z;//输入场坐标系中心点位置以及局部坐标系
		Vector3 InField_X1, InField_Y1, InField_Z1;//输入场中心点位置以及局部坐标系（主极化为InField_Y1）
		Vector3 Mirror_Position, Mirror_X, Mirror_Y, Mirror_Z;//镜面坐标系中心点位置以及局部坐标系
		Vector3 OutField_Position, OutField_X, OutField_Y, OutField_Z;//出射场的坐标系中心点位置以及局部坐标系
		Vector3 Reflect_Point, Reflect_Point_Normal;//定义反射点及该点的法向
		double Theta;//主射线在反射面反射点上的反射角度

		//以上坐标系的表示都是全局坐标系坐标

		

	};
}

#endif // TRACELIGHT_H