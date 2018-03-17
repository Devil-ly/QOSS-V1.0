/*
Head for 3D-FDTD
PEC Core for Radiator
v 1.0
Author: Ming Jin, Jinmingaps@163.com
Key feature:
PEC Core
Single Aperture Waveguide Excitation output
Single DFT pair Record
CA CB Iteration Control, Compatible with PML

*/
#pragma once
#include<vector>
#include<complex>
#include<cmath>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<cstdlib>
#include<omp.h>
#include<immintrin.h>

#include"GEMS_Memory.h"
#include"Global_Vars.h"
//#include"Communication.h"

using namespace::std;
#ifndef FDTD_H  // 全大写
#define FDTD_H

class CFDTD{
public:
	CFDTD(void);
	~CFDTD(void);
private:

public:
	clock_t start;
	clock_t com_start;
	clock_t com_period;
	clock_t com_periodu;
	clock_t com_end;
	string Despath;
	string FileoutPath;
	string ReadPath;

	//General Computation Paras:
	//Time Step
	int nn;
	int stopnn;
	float dt;
	int NN;	//NN*dt * 4 = T;
	int Step2Show;
	
	//Control Flag
	int timemode; //0 Single Frequency 1 Bandwidth Frequency
	int computed;
	int model;
	float MaxOld,MaxNew; //for judge

	//Polarization
	int Pol;
/*	//Time Source
	int timemode;
*/
//Frequency
	float freq;	float lambda;	float ww;	float wwdt;
	
	float BW;		int Nfreq;
	float sfreq;		float efreq;
	float* freqs;
	
	float* lambdas;	
	float* wws;
	float* wwdts;
	
	//Pulse
	float tao;	//time
	float ObliqueShift;	//time
	float* timeline;
	int Nt;

	//Field unknowns: X- Y- Z- direction
	float ds; 
	float dx, dy, dz;
	int Nx_model, Ny_model, Nz_model;
	int Nx, Ny, Nz;
	int N_spa; //Space Left must be larger than num_pml;
	int Nx_cal,Ny_cal,Nz_cal;
	int Rx_cal_AVX;

	//Excitation Port 2D Aperture
	int Nz_exc;//Z position in index
	int Nx_exc;	int Ny_exc; //X Y Length in index
	int Px_exc;	int Py_exc; //X Y position index corner

	//Port Record
	complex<float>*** Ex_DFT_Port;	complex<float>*** Ey_DFT_Port;
	complex<float>*** Hx_DFT_Port;	complex<float>*** Hy_DFT_Port;
	int** PortMap;

	complex<float>** Ex_Port;	complex<float>** Ey_Port;
	complex<float>** Hx_Port;	complex<float>** Hy_Port;

	float** Ex_AP_mag;	float** Ex_AP_rad;	float** Ex_AP_cos;	float** Ex_AP_sin;
	float** Ey_AP_mag;	float** Ey_AP_rad;	float** Ey_AP_cos;	float** Ey_AP_sin;

	float** Hx_AP_mag;	float** Hx_AP_rad;	float** Hx_AP_cos;	float** Hx_AP_sin;
	float** Hy_AP_mag;	float** Hy_AP_rad;	float** Hy_AP_cos;	float** Hy_AP_sin;

	//PMLs:
	int num_pml;
	//ADEPML sigma
	float* sigmaEx; float* sigmaHx;
	float* sigmaEy; float* sigmaHy;
	float* sigmaEz; float* sigmaHz;
	//For PML-3D Storage
	//Z-direction
	float*** fxz;	float*** fyz;
	float*** gxz;	float*** gyz;
	//Y-direction
	float*** fxy;	float*** fzy;
	float*** gxy;	float*** gzy;
	//X-direction
	float*** fyx;	float*** fzx;
	float*** gyx;	float*** gzx;

//Model & Fields datas:
	
	//Field Update direct load in Eps
	int*** EpsMap;	//judge cell type-3D
	float*** Epsr;		float*** Epsi;		float*** Esig;
	//Calculation Parameter
	__declspec( align( 32 ) ) float*** CA_Ex;		
	__declspec( align( 32 ) ) float*** CB_Ex;		
	__declspec( align( 32 ) ) float*** CA_Ey;
	__declspec( align( 32 ) ) float*** CB_Ey;		
	__declspec( align( 32 ) ) float*** CA_Ez;		
	__declspec( align( 32 ) ) float*** CB_Ez;
	
	//Update Fields;
	__declspec( align( 32 ) ) float*** Ex;
	__declspec( align(32)) float*** Dx;
	__declspec( align( 32 ) ) float*** Ey;
	__declspec(align(32)) float*** Dy;
	__declspec( align( 32 ) ) float*** Ez;
	__declspec(align(32)) float*** Dz;
	__declspec( align( 32 ) ) float*** Hx;		
	__declspec( align( 32 ) ) float*** Hy;		
	__declspec( align( 32 ) ) float*** Hz;

//DFT Cut _single Frequency 2D Cut Complex Data in Real and Imag
	__declspec( align( 32 ) ) float** Ex_xoz_real;	
	__declspec( align( 32 ) ) float** Ex_xoz_imag;
	__declspec( align( 32 ) ) float** Ey_xoz_real;
	__declspec( align( 32 ) ) float** Ey_xoz_imag;
	__declspec( align( 32 ) ) float** Ez_xoz_real;
	__declspec( align( 32 ) ) float** Ez_xoz_imag;
	__declspec( align( 32 ) ) float** Hx_xoz_real;
	__declspec( align( 32 ) ) float** Hx_xoz_imag;
	__declspec( align( 32 ) ) float** Hy_xoz_real;
	__declspec( align( 32 ) ) float** Hy_xoz_imag;
	__declspec( align( 32 ) ) float** Hz_xoz_real;
	__declspec( align( 32 ) ) float** Hz_xoz_imag;

	//DFT
	int Nx_DFT;
	int Ny_DFT;
	int Nz_DFT;
	int Px_DFT_0;
	int Px_DFT_1;
	int Py_DFT_0;
	int Py_DFT_1;
	int Pz_DFT_1;
//DFT box _Frequency Sweep 1D Cut
	//_z1
	complex<float>*** Ex_z1;	complex<float>*** Hx_z1;		
	complex<float>*** Ey_z1;	complex<float>*** Hy_z1;
	//_x0
	complex<float>*** Ey_x0;	complex<float>*** Hy_x0;
	complex<float>*** Ez_x0;	complex<float>*** Hz_x0;
	//_x1
	complex<float>*** Ey_x1;	complex<float>*** Hy_x1;
	complex<float>*** Ez_x1;	complex<float>*** Hz_x1;
	//_y0
	complex<float>*** Ex_y0;	complex<float>*** Hx_y0;
	complex<float>*** Ez_y0;	complex<float>*** Hz_y0;
	//_y1
	complex<float>*** Ex_y1;	complex<float>*** Hx_y1;
	complex<float>*** Ez_y1;	complex<float>*** Hz_y1;

	//存储惠更斯面数据
	complex<float>* HuygensBoxData;
	

//Parallel:
	//total num
	int threadNum;
	//Omp Division Calculation Domain
	int* Nzs_cal;	int* Nzn_cal;
	int* Nys_cal;	int* Nyn_cal;
	//Omp Division Pml Domain
	int* Ns_pml;	int* Nn_pml;
	//Omp Division total domain
	int* Nxs;	int* Nxn;
	//Omp Division total domain
	int* Nys;	int* Nyn;
	//Omp Division total domain
	int* Nzs;	int* Nzn;
	//Omp Division model
	int* Nxs_model;		int* Nxn_model;
	int* Nys_model;		int* Nyn_model;
	int* Nzs_model;		int* Nzn_model;

//Omp Division frequency
	int* Nfs;	int* Nfn;
	
	
//Position
//for Eps for Hy_inc
	float* xx;	float* yy;	float* zz;
//for Ey_inc
	float* xd;	float* yd;	float* zd;

	float* Nor;					//Power Normalization - incident power - Port

	//Far-Field Calcualtion no use
	int Ntheta;	int Nphi;
	float* thetas;	float* phis;
	int* phi_s;		int* phi_n;	//Omp Division
								//Multi-Frequency 2-D Span
	complex<float>*** FarPhi;
	complex<float>*** FarTheta;

	complex<float>*** FoutPhi;
	complex<float>*** FoutTheta;
	complex<float>*** FoutPhi_inc;
	complex<float>*** FoutTheta_inc;
	


private:
	//设置回调函数
	void(*returnFloat)(float, void*);
	void *user;
	fstream* Logfile;
public:
	//Load model Paras
	void Initial(int _threadNum, int _NN, int _Nx_model, int _Ny_model, int _Nz_model, int _Nspa, int _num_pml, float _dt, float _dx, float _dy, float _dz, float _Frequency);
	//Set Memory
	void MemoryAllocate(int _timemode, int _Nfreq, float _BW);
	//Set Excitation Port
	void SetExcPort(int _Nz_exc, int _Nx_exc, int _Ny_exc, int _Px_exc, int Py_exc, complex<float>** _Ex_Port, complex<float>** _Ey_Port, complex<float>** _Hx_Port, complex<float>** _Hy_Port);
	void SetupModel(int*** _EpsMap, float*** _Esig);
	void Judge();
	void Update();
	void UpdateE();
	void UpdateH();
	void BoundaryE_AVX();
	void BoundaryH_AVX();
	void AddSourceE();
	void AddSourceH();
	void TestPointSource();
	void Output();
	void Record();
	//void Farfield();
	void Cleanup();
	void CleanupCom();
	void CleanupRecord();
	void SetReturnFloat(void(*returnFloat)(float, void*), void* _user);
	void SetLogfile(fstream* logAdd);

	//ADE-PML Function
	inline void ADEPML(float H1H, float H1L, float H2H, float H2L, float ds1, float ds2, float sigma1, float sigma2, float &k1, float &k2, float &D);
	inline void ADEPML_AVX(__m256 H1H, __m256 H1L, __m256 H2H, __m256 H2L, __m256 dti, __m256 ds1, __m256 ds2, __m256 sigma1, __m256 sigma2, __m256 &k1, __m256 &k2, __m256 &D);
	//Source Injection in Time
	float Timesourcep(float time,int init);
	float Timesource(float time,int flag);
	float Gaussian(float w0, float x);

};
#endif // FDTD_H