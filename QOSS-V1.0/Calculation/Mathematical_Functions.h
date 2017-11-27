#pragma once
//这是一个计算阶乘以及的专用函数
#ifndef MATHEMATICAL FUNCTIONS_H
#define MATHEMATICAL FUNCTIONS_H

#include <iostream>
#include <cmath>
#include <complex>//复数类，C++自带
#include <vector>//vector用于存放数组与矩阵数据  
#include <string>

#include "../util/Constant_Var.h"

using namespace std;

double fact(int n);//已测试，阶乘函数没问题


//返回M阶Bessel函数的一阶导数
double D1_J(int m, double z);


//返回场分布值H_phi(复数形式的，默认为右旋)
complex<double> H_phi(double Kz0, double Kr0, int m0, double r0, double Phi0);

	
complex<double> H_r(double Kz0, double Kr0, int m0, double r0, double Phi0);


//返回场分布值E_phi(复数形式)
complex<double> E_phi(double Kz0, double Kr0, int m0, double r0,
	                  double Phi0, double Omega0, double Beta0);

	
//返回场分布值E_r(复数形式)
complex<double> E_r(double Kz0, double Kr0, int m0, double r0,
	double Phi0, double Omega0, double Beta0);


//返回场分布值H_z(复数形式)
complex<double> H_z(double Kr0, int m0, double r0, double Phi0);
	

//下面是左旋的TE场分布

//返回场分布值H_phi_LeftRo(复数形式)
complex<double> H_phi_LeftRo(double Kz0, double Kr0, int m0, double r0, double Phi0);


//返回场分布值H_r_LeftRo(复数形式)
complex<double> H_r_LeftRo(double Kz0, double Kr0, int m0, double r0, double Phi0);

	
//返回场分布值E_phi_LeftRo(复数形式)
complex<double> E_phi_LeftRo(double Kz0, double Kr0, int m0, double r0,
	                         double Phi0, double Omega0, double Beta0); 
	
//返回场分布值E_r_LeftRo(复数形式)
complex<double> E_r_LeftRo(double Kz0, double Kr0, int m0, double r0,
	                       double Phi0, double Omega0, double Beta0);

//返回场分布值H_z_LeftRo(复数形式)
complex<double> H_z_LeftRo(double Kr0, int m0, double r0, double Phi0);


#endif // 
//20171026已经验证