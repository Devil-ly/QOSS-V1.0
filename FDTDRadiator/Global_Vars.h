//Float Version
#pragma once
#include<complex>
#include<fstream>
const float C_Speedf		= 2.997956e8f ;
const float C_Speed_Invf	= 0.3335605e-8f ;
const float Eps0f			= 8.854817187e-12f ;
const float Eps0_Invf		= 1.12932879232e11f;

const float Mu0f			= 1.256637016e-06f ;
const float Mu0_Invf		= 7.957747442e5f ;

const float PIf				= 3.141592654f ;
const float Eta0f			= 376.733341f ;
const float Eta0_Invf		= 0.002654397f ;
const float Sig_PECf		= 1.0e20f ;
const float SmallNumberf	= 0.00001f ;

const int AVX_Count = 8;
const int SSE_Count = 4;

//圆电模式辐射器结构参量
extern double PHId;
extern double THETAd;
extern double RCd;

extern float PHI;
extern float THETA;
extern float RC;

//Computation Domain Size
extern int Domian_Nx;
extern int Domian_Ny;
extern int Domain_Nz;
extern int Domian_num_pml;

//Model Array Data Array
//extern int *** EFlag;
//extern float *** EpsR;
//extern float *** EpsI;
