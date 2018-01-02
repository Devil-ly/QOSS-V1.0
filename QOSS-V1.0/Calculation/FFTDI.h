
/**************************************************************************************
* version 1.0 2016/11/7
* Designer liyun
* E-mail 465120258@qq.com
* Fuction： 
* 计算斜面传播
* 输入 Ex Ey 得到目标面的Ex Ey Ez Hx Hy Hz
* 需要输入参数：频率（默认10.65e9）、目标距离、计算点数N*M、
目标面倾斜角（x轴方向）、ds（默认波长/3.5）
***************************************************************************************/
#pragma once
#ifndef CALCUlATION_H
#define CALCULATION_H

#include <cmath>
#include <complex>
#include "../util/GEMS_Memory.h"
#include "../util/Constant_Var.h"
#include "FFT.h"

class FFTDI
{
public:
	FFTDI(double f = 10.65e9, double z0 = 1,int N = 361, int M = 361);
	~FFTDI();

	void Initialization(); 
	void FreeCal();
	void Setds(double ds1); //设置ds

	//设置输入并补0
	void SetInput(std::complex<double> ** ExIn, std::complex<double> ** EyIn);

	void StartCal();

	//输出Ex Ey Ez Hx Hy Hz 并调用FreeCal
	void output(std::complex<double> ** Ex, std::complex<double> ** Ey, std::complex<double> ** Ez, 
		std::complex<double> ** Hx, std::complex<double> ** Hy, std::complex<double> ** Hz );

private:
	//定义复数的乘
	void MulComp(const double r1, const double i1, const double r2, const double i2, double & outR, double & outI);
	std::complex<double> MulComp(std::complex<double> & In, double r2, double i2);
	//插值函数
	double InserVal(const double x0, const double yUp, const double y0, const double yDown);

	//计算传达函数
	void Calh0();

	//对输入场进行旋转
	void Rotate();

	//对输入场进行插值
	//void InserExEy();

private:
	double f; // 频率 默认 10.65e9
	double lamda; // 波长
	double k;
	//double theta;
	//double theta_h0;
	double ds;
	double z0; //目标距离

	int N, M; //计算的点数 一般设为奇数 默认360
	int N2, M2; // N2 = 2 * N -1

	//传递函数
	std::complex<double> ** HExy, ** HEz_x0, **HEz_y0;
	std::complex<double> ** HHx_x0, ** HHx_y0,
		** HHy_x0, ** HHy_y0,
		** HHz_x0, ** HHz_y0;

	//补0后的输入
	std::complex<double> ** Ex0, ** Ey0;

	// 帕斯维尔定理
	double 	PowerHExy, PowerFFTHExy;
	double 	PowerHEz_x0, PowerFFTHEz_x0;
	double 	PowerHEz_y0, PowerFFTHEz_y0;
	double 	PowerHHx_x0, PowerFFTHHx_x0;
	double 	PowerHHx_y0, PowerFFTHHx_y0;
	double 	PowerHHy_x0, PowerFFTHHy_x0;
	double 	PowerHHy_y0, PowerFFTHHy_y0;
	double 	PowerHHz_x0, PowerFFTHHz_x0;
	double 	PowerHHz_y0, PowerFFTHHz_y0;

	//Ex1 Ey1 Ez1 Hx1 Hy1 Hz1
	std::complex<double> ** Ex1, ** Ey1, **Ez1;
	std::complex<double> ** Hx1, ** Hy1, **Hz1;
};



#endif // !CALCUlATION_H
