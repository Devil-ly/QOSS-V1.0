#pragma once
//这是一个计算矩形口面电磁场分布的传播程序，采用最近开发的离散高斯波束分解拟合算法
//输入为矩形场分布，并以该举行擦汗那个分布平面作为局部坐标系
//默认主极化Ey，输入矩阵格式按此来生成
//函数包括求单点主极化场分布和多个点的场分布
//by XD 20180303
//20180307测试正确，by XD
//20180314再次测试斜高斯传播，正确,by XD

#ifndef BEAMPROPAGATION_H
#define BEAMPROPAGATION_H

#include <iostream>
#include <cmath>
#include <math.h>
#include <complex>//复数类，C++自带
#include <string>
#include "../util/Constant_Var.h"
#include <vector>//vector用于存放数组与矩阵数据
#include "../util/Vector3.h"
#include "Gaussian_Beam_Elliptical.h"


using namespace std;

//分解口面场，返回分解次数Actual_SplitTimes和分解出的高斯波束信息矩阵Split_Info
void Field_Split(double frequency0, double ds,int N0,
	                        vector <vector <complex <double>>> &E0,
	                        int &Actual_SplitTimes, vector <vector <double>> &Split_Info);

//在运行完Field_Split程序后得到Actual_SplitTimes和Split_Info的基础上，
//计算口面场传播至单个点P的场强
complex<double> Calculate_SinglePoint(double frequency0, double ds,int N0,
	                        int Actual_SplitTimes, vector <vector <double>> &Split_Info,
	                        Vector3 P);

#endif
