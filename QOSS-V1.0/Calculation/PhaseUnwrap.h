#pragma once
//这是一个对相位分布矩阵进行相位修正的程序，采用Matlab程序Version5.m中的步骤
//输入相位单位是弧度
//by XD 20180308
//20180310通过测试

#ifndef PHASEUNWRAP_H
#define PHASEUNWRAP_H

#include <iostream>
#include <cmath>
#include <math.h>
#include <string>
#include "../util/Constant_Var.h"
#include <vector>//vector用于存放数组与矩阵数据
#include "../util/Vector3.h"


using namespace std;

//一维相位解折叠（二维相位解折叠函数中要用到它）
//输入的Phase矩阵是折叠过的，由arg，即atan2函数得到，范围是（-Pi，Pi]
vector <double> Unwrap_1D(int N0,vector <double> Phase);

//二维相位解折叠函数,对N0*N0相位矩阵进行修正
vector <vector<double>> Unwrap_2D(int N0, vector <vector <double>> Phase);

#endif
