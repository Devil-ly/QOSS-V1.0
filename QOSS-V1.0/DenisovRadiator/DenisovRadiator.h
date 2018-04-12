#ifndef DENISOVRADIATOR_H
#define DENISOVRADIATOR_H
/*****************************************
******This Class Define a Denisov Radiator
******Part of QOSS******by Ming Jin*******
********************jinmingaps@163.com****
******************************************/
//This is a QThread for Computation

#pragma once
#include <Qthread>
//#include "showDenisov.h"
#include <cmath>
#include <omp.h>	//openmp acceleration

#include <complex>
//io for debug
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include "immintrin.h"	//AVX acceleration
//#include <ionamip>
/* This class defines the Denisov Radiator */
/* This class describe the design process of Denisov Radiator*/
//First without
using namespace std;


class DenisovRadiator: public QThread
{
	Q_OBJECT
public:
	DenisovRadiator();
	~DenisovRadiator();

//private slots:	//信号槽

private:
	//define functions

//public slots:


public:
	//basic parameters
	double frequency;
	double radius;
	//Centeral mode
	int m;
	int n;

	//Mode Span
	int mspan;
	int nspan;
	int* mindex;
	int* nindex;
	

	//parameter for turbulent geometry
	double delBeta1;	//扰动参数-扰动1
	double delBeta2;	//扰动参数-扰动2
	int l1;		//扰动参数-扰动1
	int l2;		//扰动参数-扰动2
	double z0_1;	//扰动段中心位置-扰动1
	double z0_2;	//扰动段中心位置-扰动2
	double ls_1;	//过渡段长度-扰动1
	double ls_2;	//过渡段长度-扰动2
	double lc_1;	//中心扰动段长度-扰动1
	double lc_2;	//中心扰动段长度-扰动2
	double Hz;
	double dz;
	int Nz;

	//Paint Tangential Field
	int Nx;
	int Ny;
	vector<vector<complex<double>>> Ex;
	vector<vector<complex<double>>> Ey;
	//Paint Surface Current
	int Nphi;
	int Nheight;
	int Nsparse;
	vector<double> J_line;
	vector<vector<double>> J;

	double mag1;	//扰动强度1
	double mag2;	//扰动强度2
	fstream Denisovlog;

	void SetupDenisov(double _freq, double _radius, int _m, int _n);
	void SetupDenisovParas(double _delBeta1, double _delBeta2, int _l1, int _l2, double _z0_1, double _z0_2, double _ls_1, double _ls_2, double _lc_1, double _lc_2, double _Hz, double _dz, int _Nz, double _mag1, double _mag2);
	void GetDenisovTurbulence(vector<double>& _sig1, vector<double>& _sig2);
	void SetupModeField(int _Nx, int _Ny);
	void SetTangentialEField(int _Nx, int _Ny);
	void GetTangentialEField(vector<vector<complex<double>>> &_Ex, vector<vector<complex<double>>> &_Ey, int _Nx, int _Ny);
	void SetSurfaceCurrent(int _Nphi, int _Sparse);
	void GetSurfaceCurrent(vector<vector<double>> &_J, int _Nphi, int _Nheight);
	void GetSurfaceCurrentLine(vector<vector<double>> &_J, int _Nphi, int _Nposition);

	vector<double> Zz;	
	vector<double> sig1;	
	vector<double> sig2;	
	vector<double> dsig1;	
	vector<double> dsig2;	

	vector< double> Coes9;

protected:
	void run();

private:
	volatile bool stopped;

public: signals:
	void SendValue(double _out);
	void SendCoefficients(double _CoeTotal, double _CoeMain, double _CoeNeighbor, double _CoeCorner, int _nn);
	void SendText(QString Message);
	void SendBtnClose(void);
	void SendBtnOpen(void);
	void SendFieldUpdate(void);
	void SendCurrentUpdate(void);
};


#endif // DENISOV