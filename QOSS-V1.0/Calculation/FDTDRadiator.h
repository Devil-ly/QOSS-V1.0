#pragma once
#include <stdexcept>
#include <cmath>
#include <ctime>
#include <fstream>
#include <vector>
#include <complex>
#include "Position3D.h"	//公用
#include "Matrix4D.h"	//公用
#include "Vector3D.h"	//公用
//删去不必要的 -20180321

using namespace std;

//This Part Set the FDTD Computation for a Radiator
class _declspec(dllexport) FDTDRadiator //整个类都写成了接口
{
public:
	FDTDRadiator();
	~FDTDRadiator();
	//This Function Set Up the LowOrder Vlasov Radiator Computation
	void SetUpLowOrderVlasovRadiator(int _WG_m, int _WG_n, double _Frequency, double _Radius, double _F, int _Ns, int _OmpNum);
	void SetUpAperturePlane(Position3D _AperturePosition, Vector3D _ApertureDirection, Vector3D _UDirection, Vector3D _VDirection, int _Nu, int _Nv, double _Lu, double _Lv);
	void SetInt(int input);
	void run();
	void SetpFun(void(*pFun)(int));
	void SetReturnInt(void(*returnint)(int, void*), void *_user);  // 注册回调函数
	void SetReturnFloat(void(*returnFloat)(float, void*), void*_user);// 注册回调函数
	void WriteApertureDataToFile(const char* _filename);
	void GetProFieldE(vector<vector<complex<double>>>& _Eu, vector<vector<complex<double>>>& _Ev, int _Nu, int _Nv);
	void LoadProFieldE(const char* _filename, vector<vector<complex<double>>>& _Eu, vector<vector<complex<double>>>& _Ev, int _Nu, int _Nv);

private:
	// 回调函数指针
	void(*pFun)(int);
	//回调函数指针
	void(*returnInt)(int, void*);
	void(*returnFloat)(float, void*);

	void *user; // 回调函数的类指针

	void SelectDT(float _dx, float _dy, float _dz); 
public:
	//For test
	int number;

	//Waveguidemode
	int SourceKind;	//1：低阶TE模式；2：高阶TE模式；3：矩形波导模式
	int SourceType;	//1：TE模式；2：TM模式；
	int Rotation;	//0：不存在；1：左旋；2：右旋；
	int WG_m;		//TE mn
	int WG_n;		//TE mn
	double Frequency;	//频率 Hz
	double Radius;	//圆波导半径
	double F;		//Focus lengh for the First Mirror Low order TE case
	double WG_A;	//矩波导长边
	double WG_B;	//矩波导短边
	double WG_H;	//波导段长度
	double AP_A;	//开口段长边
	double AP_B;	//开口段短边
	double AP_H;	//开口段长度
	int Ns;			//采样点数
	int Ns1;

	//出射口面参数
	Position3D AperturePosition;
	Vector3D ApertureDirection;
	Vector3D UDirection;
	Vector3D VDirection;
	int Nu;
	int Nv;
	float Lu;
	float Lv;
	vector<vector<complex<float>>> Eu;
	vector<vector<complex<float>>> Ev;
	char* FILENAME;


	//圆电模式辐射器结构参量
	double PHId;
	double THETAd;
	double Rcd;
	double Lcd;
	float PHI;
	float THETA;
	float Rc;
	float Lc;
	float Lp;
	float ds;



	//建立画模型用  全局变量
	int Domian_Nx;
	int Domian_Ny;
	int Domain_Nz;
	int Domian_num_pml;

	//描述模型的参数数组
	///int*** EFlag;
	///float *** EpsR;
	///float *** EpsI;

	float cdt;
	float Requireddt;
	int NN;
	int N_spa = 10;
	int OmpNum = 4;

	//fstream logfile;


};

