#pragma once
//define Radiator Structure
//length Structure
#include<omp.h>
#include<cmath>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<cstdlib>
#include<complex>
#include"GEMS_Memory.h"
#include"Global_Vars.h"
#include "geofun.h"

using namespace::std;

#ifndef RADIATOR_H  // 全大写
#define RADIATOR_H

class CRadiator {

	public:
		CRadiator(void);
		CRadiator(
		int _Kind = 1,	//1是低阶TE模式;2是高阶TE模式；3是矩形波导模式
		int _Type = 1,	//模式 TE 1 或TM 2；
		int _Rotation = 0,	//左旋右旋 左旋为1，右旋为2；
		int _m = 0,
		int _n = 1,
		int Nx = 100,
		int Ny = 100,
		float _lambda = 3e-3,	//波长
		float _Radius = 20e-3,	//圆波导半径
		float _Lc = 20e-3,		//圆电模切口高度
		float _Lp = 10e-3,		//圆电模传播段高度
		float _wa = 8e-3,		//矩形波导长边
		float _wb = 4e-3,		//矩形波导短边
		float _Hp = 8e-3,		//传播段高度
		float _Ha = 20e-3,		//辐射段高度
		float _aa = 20e-3,		//辐射口面长边
		float _bb = 15e-3		//辐射口面短边
	);//默认构造函数

	~CRadiator(void) {
		// 若有分配了内存记得释放
		// if(p) {
		//   delete p;
		//   p = NULL;
		// }	
		//position
		Free_1D(px); px = NULL;
		Free_1D(py); py = NULL;
		Free_1D(pz); pz = NULL;
		Free_3D(EpsMap, Nz_model);	EpsMap = NULL;
		Free_3D(Esig, Nz_model);	Esig = NULL;
	};
	//重设低阶辐射器参数
	void ResetRadiator_RoundL(float _lambda, float _Radius, int _Nx, int _Ny, int _N_spa,float _Lc, float _Lp);
	//设置低阶辐射器镜面
	void SetFirstMirror_RoundL(float _F);
	//重设高阶辐射器参数
	void ResetRadiator_RoundH(int _Rotation,float _lambda, float _Radius, int _Nx, int _Ny, int _N_spa, float _Lc, float _Lp);

	//确立低阶圆电辐射器参数
	void SetRadiator_RoundL( float _Lc, float _Lp);
	//确立高阶圆电辐射器参数
	void SetRadiator_RoundH(int _Rotation, float lambda, float Radius, float Lc, float Lp);
	//建立块块网格数组(非共型部分，低阶圆电辐射器)
	void GenerateCellArray_RoundL(void);
	//建立块块网格数据(非公型部分，高阶圆电辐射器)
	void GenerateCellArray_RoundH(void);
	//建立块块网格数据的中间函数
	float RFxy(float x, float y);
	//建立块块网格函数的中间函数
	bool RPECStructure(float x, float y, float z);

	//建立块块网格数组(非共型部分，矩形波导部分)
	void GenerateCellArray_Rec(void);
	//建立块块网格数组(非共型部分，矩形辐射器阵列部分)
	void GenerateCellArray_RecArray(int Nunit);

	//确立矩波导辐射器参数
	void SetRadiator_Rectan(int Type, int m, int n, float lambda, float wa, float wb, float Hp, float Ha, float aa, float bb );
	//设置Logfile
	void SetLogfile(fstream* logAdd);
private:	//data

	int Kind;		//辐射器样式
	int Type;		//辐射器激励模式	
	int Rotation;	//圆电左旋右旋
	int m;
	int n;
	int Nx;			//波导模式场X方向采样点数
	int Ny;			//波导模式场Y方向采样点数
	float lambda;	//电波长
	float Radius;	//圆波导半径
	float Lc;		//圆电模切口高度
	float Lp;		//圆电模传播段高度
	float wa;		//矩形波导长边
	float wb;		//矩形波导短边
	float Hp;		//传播段高度
	float Ha;		//辐射段高度
	float aa;		//辐射口面长边
	float bb;		//辐射口面短边
	float ds;		//剖分精度

	fstream* Logfile;

public:
	int Nx_model;	//Model Size
	int Ny_model;	//Model Size	
	int Nz_model;	//Model Size

	int Px_model;	//Start Position of Excitation Port
	int Py_model;	//Start Position of Excitation Port
	int Pz_model;	//Start Position of Excitation Port
	//position
	float* px;
	float* py;
	float* pz;
	//uniform mesh	//三个方向可能不一样哦，要区分开
	float dx;		//
	float dy;
	float dz;
	//non uniform mesh 暂不用
	float* dx_nu;
	float* dy_nu;
	float* dz_nu;

	int*** EpsMap;	//Mark	
	float*** Esig;	//Passed into non-conformal FDTD
	int N_spa;

	float MirrorHeight; //First Mirror Height

	//define geometry functions

};
#endif // TRACELIGHT_H

