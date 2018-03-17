#pragma once
//这是一个计算高斯波束分布的专用函数
//该高斯波束沿着Z轴正方向传播，束腰W0位置在Z=0处
//by XD 20180205

#ifndef GAUSSIAN_BEAM_CIRCULAR_H
#define GAUSSIAN_BEAM_CIRCULAR_H

#include <iostream>
#include <cmath>
#include <complex>//复数类，C++自带
#include <string>
#include "../util/Constant_Var.h"
#include "../util/Vector3.h"

using namespace std;

//返回传播z长度后的高斯波束W值
double Gauss_Omega_Circular(double frequency0, double w0, double z0);

//返回圆高斯波束在P（x,y,z）的复数主极化值
complex<double> Gauss_E_Circular(double frequency0, double w0, Vector3 P);

//返回圆高斯波束在P（x,y,z）的弧度形式的未折叠相位值
double Gauss_Phase_Circular(double frequency0, double w0, Vector3 P);

#endif
