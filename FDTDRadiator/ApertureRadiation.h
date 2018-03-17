#pragma once
#define EIGEN_VECTORIZE_AVX
#define EIGEN_COMPLEX_AVX
#define EIGEN_ALIGN32 EIGEN_ALIGN_TO_BOUNDARY(32)
#define EIGEN_MAX_STATIC_ALIGN_BYTES 32 
#define EIGEN_MALLOC_ALREDAY_ALIGNED 1
#define EIGEN_DEFAULT_TO_ROW_MAJOR
#ifndef APERTURERADIATION_H
#define APERTURERADIATION_H



//这个是CPU计算版 

#include "immintrin.h"
#include "Constant_Val.h"
#include "Global_Vars.h"
#include "GEMS_memory.h"
#include "omp.h"
#include <vector>
#include <complex>
#include "Vector3D.h"
#include "Position3D.h"
#include "Matrix4D.h"
#include "eigen/Eigen/Core"
#include "eigen/Eigen/Dense" //据说EIGEN不能用在CUDA里
#include "eigen/Eigen/src/Core/Array.h"
//利用李赟更新的 Position位置 Vector向量 和Matrix4D来操作位置 

//This Class is for Calculating Radiated Aperture Field from Huygens Box
//Aperture is defined by its center position, orientation, and aperture size
//Aperture Field is calculated from the Huygens Aperture Field
using namespace std;
using namespace Eigen;


class ApertureRadiation
{
public:
	ApertureRadiation();
	~ApertureRadiation();
public:
	//Position of Center in m
	Position3D SourceCenter;
	int Nx;
	int Ny;
	int Nz;
	float dx;
	float dy;
	float dz;
	//Orientation
	Position3D ApertureCenter;
	Vector3D ApertureDirection;
	Vector3D UDirection;
	Vector3D VDirection;

	//Aperture Size in m
	float Lu;	//Size
	float Lv;	//Size
	int Nu;		//Sampling Number
	int Nv;		//Sampling Number
	float du;	//Interval
	float dv;	//Interval
	
	//频率
	int NumFreq;
	float* Freqs;

	//Propagated Fields from Huygens Box
	void SetUpHuygens(Position3D _SourceCenter, int _Nx, int _Ny, int _Nz, float _dx, float _dy, float _dz);
	void SetUpFrequency(int _NumFreq, float _CenterFreq, float _Bandwidth);
	void SetUpPropagatedAperture(Position3D _ApertureCenter, Vector3D _ApertureDirection, Vector3D _UDirection, Vector3D _VDirection, float _Lu, float _Lv, int _Nu, int _Nv);
	void Propagation5FaceBox(vector<vector<complex<float>>>& _Eu, vector<vector<complex<float>>>& _Ev, complex<float>* _HuygensBoxData, int OmpNum, int _Freqindex);
	
	//Propagated Fields from Approximated Cut
	void SetUpSurfaceCurrent();
	void PropagationCutCurrent();

	//回调函数-返回进度
	void SetReturnFloat(void(*returnFloat)(float, void*), void*);
	//设置Logfile
	void SetLogfile(fstream* logAdd);

private:
	//设置回调函数
	void(*returnFloat)(float, void*);
	void *user;
	fstream* Logfile;
};

#endif