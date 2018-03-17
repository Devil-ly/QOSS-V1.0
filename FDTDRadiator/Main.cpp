#include "FDTD.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "SourceModeGenerationT.h"
#include "Radiator.h"
#include "Constant_Var.h"


using namespace calculation;
using namespace std;

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

//应该考虑定义为  全局变量
complex<float>** Ex_Port;
complex<float>** Ey_Port;
complex<float>** Hx_Port;
complex<float>** Hy_Port;

//建立画模型用  全局变量
int Domian_Nx;
int Domian_Ny;
int Domain_Nz;
int Domian_num_pml;

//描述模型的参数数组  全局变量
int*** EFlag;
float *** EpsR;
float *** EpsI;

float cdt;
int NN;
int N_spa = 10;
int OmpNum = 4;
SourceModeGenerationT Port;	//Port Exciation
CRadiator Radiator;			//Radiator Model

void SelectDT(void) {
	NN = 5;
	float Requireddt;
	float tempdt;
	//三个方向的间距中取最小的dt
	Requireddt = Radiator.dx / C_Speedf / 2.0;
	tempdt = Radiator.dy / C_Speedf / 2.0; if (Requireddt > tempdt) Requireddt = tempdt;
	tempdt = Radiator.dz / C_Speedf / 2.0; if (Requireddt > tempdt) Requireddt = tempdt;
	cdt = 1 / Frequency / (NN*4.0);
	while (cdt > Requireddt) {
		NN++;
		cdt = 1 / Frequency / (NN*4.0);
	}
	cout << "  Discrete interval in X: " << Radiator.dx << "m, in Y: " << Radiator.dy << "m, in Z: " << Radiator.dz <<"m."<< endl;
	cout << "  Discrete interval in lambda, X: " << Radiator.dx*Frequency / C_Speedf << ", Y: " << Radiator.dy*Frequency / C_Speedf << ", Z: " << Radiator.dz*Frequency / C_Speedf << "." << endl;
	cout << "  Required max dt: " << Requireddt << ", Used dt: " << cdt << ", T contains" << 1.0 / cdt / Frequency << "dt. Ref:" << NN * 4 << endl;

}

int main(void){
	//Units Hz m
	cout <<" 3_D FDTD Editions for Waveguide Radiation"<<endl;
	cout << "Initial Waveguide Mode" << endl;
	//Read Parameter from "input.in" file
	fstream filein;
	filein.open("./input.in", ios::in);
	if (!filein.is_open()) {
		cout << "    ERROR！  input file cannot be found!" << endl;
		cout << " Enter to Exit " << endl;
		getchar();
		exit(-1);
	}
	filein.ignore(100, '\n');
	filein.ignore(100, '\n');
	//First Section for Waveguide Paras
	filein >> SourceKind >> SourceType >> Rotation;			filein.ignore(100, '\n');
	filein >> WG_m >> WG_n;									filein.ignore(100, '\n');
	filein >> Frequency >> Radius;							filein.ignore(100, '\n');
	filein >> F;											filein.ignore(100, '\n');
	filein >> Lp;											filein.ignore(100, '\n');
	filein >> Ns >> Ns1;									filein.ignore(100, '\n');
	filein >> WG_A >> WG_B >> WG_H >> AP_A >> AP_B >> AP_H;	filein.ignore(100, '\n');
	
	filein.ignore(100, '\n');
	//Second Section
	filein >> OmpNum;	filein.ignore(100, '\n');	//
	filein.ignore(100, '\n');

	filein.close();
	//display Flag
	if (SourceKind == 1) { 
		cout << "Low Order Round WG TE mode: ";
		if (SourceType == 1) cout << "TE" << WG_m << WG_n << endl;
		if (SourceType == 2) cout << "TM" << WG_m << WG_n << endl;
		if (F != 0) cout << " With First Mirror! " << endl;
	}
	if (SourceKind == 2) {
		cout << "High Order Round WG TE mode: ";
		if (SourceType == 1) cout << "TE" << WG_m << WG_n << endl;
		if (SourceType == 2) cout << "TM" << WG_m << WG_n << endl;
	}
	if (SourceKind == 3) {
		cout << "Low Order Rectan WG mode: ";
		if (SourceType == 1) cout << "TE" << WG_m << WG_n << endl;
		if (SourceType == 2) cout << "TM" << WG_m << WG_n << endl;
	}
	
	//Generate Waveguide Field
	if (SourceKind == 1 || SourceKind == 2) {
		Port.SetSource_Circular(SourceKind, SourceType, Rotation, WG_m, WG_n, Frequency, Radius);
		Port.SetOutputProperty(Ns);
		Port.FieldCalculation_CircularT();
	}

	//Buffer
	vector<vector<complex<double>>> Ex0;
	vector<vector<complex<double>>> Ey0;
	vector<vector<complex<double>>> Hx0;
	vector<vector<complex<double>>> Hy0;
	Port.GetEX(Ex0);	Port.GetEY(Ey0);
	Port.GetHX(Hx0);	Port.GetHY(Hy0);
	
	//Write WaveGuide Mode Field Distribution into FDTD Buffer
	//Truncate Double into Float
	Ex_Port = Allocate_2D(Ex_Port, Ns, Ns);	Ey_Port = Allocate_2D(Ey_Port, Ns, Ns);
	Hx_Port = Allocate_2D(Hx_Port, Ns, Ns);	Hy_Port = Allocate_2D(Hy_Port, Ns, Ns);
	for (int j = 0; j < Ns; j++) {
		for (int i = 0; i < Ns; i++) {
			Ex_Port[j][i] = complex<float>(float(Ex0[i][j].real()), float(Ex0[i][j].imag()));
			Ey_Port[j][i] = complex<float>(float(Ey0[i][j].real()), float(Ey0[i][j].imag()));
			Hx_Port[j][i] = complex<float>(float(Hx0[i][j].real()), float(Hx0[i][j].imag()));
			Hy_Port[j][i] = complex<float>(float(Hy0[i][j].real()), float(Hy0[i][j].imag()));
		}
	}
	cout << " Excitation Port Field Generated" << endl;
	FILE *WGField;
	WGField = fopen("./WaveguideField.dat", "wb");

	fwrite(&Ns, sizeof(int), 1, WGField);
	for (int j = 0; j < Ns; j++) {
		for (int i = 0; i < Ns; i++) {
			fwrite(&Ex_Port[j][i], sizeof(complex<float>), 1, WGField);	//Ex.real
			fwrite(&Ey_Port[j][i], sizeof(complex<float>), 1, WGField);	//1
			fwrite(&Hx_Port[j][i], sizeof(complex<float>), 1, WGField);	//2
			fwrite(&Hy_Port[j][i], sizeof(complex<float>), 1, WGField);	//3
		}
	}
	fclose(WGField);
	cout << " Output Excitation Port Field done!" << endl;
	Ex0.~vector(); Ey0.~vector(); Hx0.~vector(); Hy0.~vector();
	//Ready to Feed FDTD

	//Calculate Radiator Parameters 
	//Get Radiator Parameters
	Port.GetCircularWaveguideProperty(PHId, THETAd, Rcd,Lcd);
	//Check Parameters OK
	Rc = float(Rcd);
	Lc = float(Lcd);
	//cout << Lc << " " << Lp << endl;
	//Make Radiator Description

	//cout << Radius << endl;
	N_spa = 10;
	if (SourceKind == 1) {
		Radiator.ResetRadiator_RoundL(float(C_Speed / Frequency), float(Radius), Ns, Ns, N_spa, Lc, Lp);
		Radiator.GenerateCellArray_RoundL();
		Radiator.SetFirstMirror_RoundL(F); //待查验
		cout << " LowOrder Radiator and Mirror Parameters:" << endl;
		cout << "   Radius:" << Radius << "m, Cut Propagation Section Length: " << Lc << "m, Mirror Height: " << Radiator.MirrorHeight << "m, Mirror Focus Length:" << F <<"m."<< endl;
		cout << "   Model Domain Size, Nx: " << Radiator.Nx_model << ", Ny: " << Radiator.Ny_model << ", Nz: " << Radiator.Nz_model << endl;
		cout << "   Leads to " << Radiator.Nx_model*Radiator.Ny_model*Radiator.Nz_model / 1.0e6 <<" Mcells" << endl;
	}
	else if (SourceKind == 2) {
		Radiator.ResetRadiator_RoundH(Rotation, float(C_Speed / Frequency), float(Radius), Ns, Ns, N_spa, Lc, Lp);
		Radiator.GenerateCellArray_RoundH();	
	}
	//getchar();
	//AutoMatic Generate Dt  T/Dt = 4*N
	SelectDT();
	//不同的频点-高阶模的交散半径不一样 最好逐点频计算
	//不同的频点-低阶模的变化要小一些 可以带宽计算
	CFDTD FDTD;
	
	//ParameterPass
	FDTD.Initial(OmpNum,NN,Radiator.Nx_model,Radiator.Ny_model,Radiator.Nz_model,N_spa,8,cdt,Radiator.dx,Radiator.dy,Radiator.dz,Frequency);
	//int _threadNum, int _NN, int _Nx_model, int _Ny_model, int _Nz_model, int _Nspa, int _num_pml, float _dt, float _dx, float _dy, float _dz, float _Frequency
	
	FDTD.MemoryAllocate(0,1,0);
	//int _timemode, int _Nfreq, float _BW
	
	int Nx_exc = Ns;	int Ny_exc = Ns;
	//!!!!!!!!!!!!!待修改
	int Nz_exc = Radiator.Pz_model;
	int Px_exc = Radiator.Px_model+N_spa;
	int Py_exc = Radiator.Py_model+N_spa;
	//Feed Excitation from Waveguide Port

	//Load PEC Structure From Radiator
	FDTD.SetupModel(Radiator.EpsMap,Radiator.Esig);
	//int*** _EpsMap, float*** _Esig
	FDTD.SetExcPort(Nz_exc, Nx_exc, Ny_exc, Px_exc, Py_exc, Ex_Port, Ey_Port, Hx_Port, Hy_Port);
	//int _Nz_exc, int _Nx_exc, int _Ny_exc, int _Px_exc, int Py_exc, complex<float>** _Ex_Port, complex<float>** _Ey_Port, complex<float>** _Hx_Port, complex<float>** _Hy_Port
		
	FDTD.Update();
		
	cout <<"Calculation Completed. ENTER to exit. "<<endl;
	getchar();


}


