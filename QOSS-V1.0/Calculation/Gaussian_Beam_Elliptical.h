#pragma once
//这是一个计算椭圆高斯波束分布的专用函数
//该高斯波束沿着Z轴正方向传播，束腰位置在Z=0处
//by XD 20180305

#ifndef GAUSSIAN_BEAM_ELLIPTICAL_H
#define GAUSSIAN_BEAM_ELLIPTICAL_H

#include <iostream>
#include <cmath>
#include <complex>//复数类，C++自带
#include <string>
#include "../util/Constant_Var.h"
#include "../util/Vector3.h"

using namespace std;

//返回波束传播Z距离后的束腰值W(Wx和Wy公式一样)
double Gauss_Omega_Elliptical(double frequency0, double w0, double z0);

//返回波束传播Z距离后的曲率半径R(Wx和Wy公式一样)
double Gauss_R_Elliptical(double frequency0, double w0, double z0);

//返回相位值
double Phi_Elliptical(double frequency0, double w0x, double w0y, double z0);

//返回圆高斯波束在坐标点P（x,y,z）的复数主极化值
complex<double> Gauss_E_Elliptical(double frequency0, double w0x, double w0y, Vector3 P);

#endif