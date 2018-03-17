#include "FDTDRadiator.h"
#include "Radiator.h"	//Setup Radiator Model
#include "FDTD.h"		//Setup FDTD Computation
#include "ApertureRadiation.h"	//SetupHuygensRadiation
#include "SourceModeGenerationT.h"	//Setup Source Model Aperture
#include "Global_Vars.h"
#include "Constant_Val.h"

using namespace calculation;
#define FDTDRADIATOR  _declspec(dllexport)
FDTDRadiator::FDTDRadiator() {
	returnInt = NULL;
	returnFloat = NULL;
	user = NULL;
}
FDTDRadiator::~FDTDRadiator() {

}

void FDTDRadiator::SetInt(int input) 
{
	number = input;
}

void FDTDRadiator::SetpFun(void(*pFun)(int)) 
{
	this->pFun = pFun;
}
void FDTDRadiator::SetReturnInt(void(*returnInt)(int, void*), void *_user)
{
	this->returnInt = returnInt;
	this->user = _user;
}
void FDTDRadiator::SetReturnFloat(void(*returnFloat)(float, void*), void *_user)
{
	this->returnFloat = returnFloat;
	this->user = _user;
}

void FDTDRadiator::GetProFieldE(vector<vector<complex<float>>>& _Eu, vector<vector<complex<float>>>& _Ev, int _Nu, int _Nv)
{
	for (int u = 0; u < _Nu; u++) {
		for (int v = 0; v < _Nv; v++) {
			_Eu[u][v] = Eu[u][v];
			_Ev[u][v] = Ev[u][v];
		}
	}
}



void FDTDRadiator::SelectDT(float _dx, float _dy, float _dz){
	
	NN = 5;
	float tempdt;
	//三个方向的间距中取最小的dt
	Requireddt = _dx / C_Speedf / 2.0;
	tempdt = _dy / C_Speedf / 2.0; if (Requireddt > tempdt) Requireddt = tempdt;
	tempdt = _dz / C_Speedf / 2.0; if (Requireddt > tempdt) Requireddt = tempdt;
	cdt = 1 / Frequency / (NN*4.0);
	while (cdt > Requireddt) {
		NN++;
		cdt = 1 / Frequency / (NN*4.0);
	}

}


void FDTDRadiator::SetUpLowOrderVlasovRadiator(int _WG_m, int _WG_n, double _Frequency, double _Radius, double _F, int _Ns, int _OmpNum) {
	//Set I Load Paras
	WG_m = _WG_m;
	WG_n = _WG_n;
	Frequency = _Frequency;
	Radius = _Radius;
	F = _F;
	Rotation = 0;
	SourceKind = 1;//低阶模式
	SourceType = 1;//TE模式
	Ns = _Ns;
	OmpNum = _OmpNum;
	//logfile.open("./FDTDRadiator.log", ios::out);
}

void FDTDRadiator::SetUpAperturePlane(Position3D _AperturePosition, Vector3D _ApertureDirection, Vector3D _UDirection, Vector3D _VDirection, int _Nu, int _Nv, double _Lu, double _Lv) {
	AperturePosition = _AperturePosition;
	ApertureDirection = _ApertureDirection;
	UDirection = _UDirection;
	VDirection = _VDirection;
	Nu = _Nu;	Nv = _Nv;
	Lu = float(_Lu);	Lv = float(_Lv);
	//再申请个内存吧 先U再V便于理解
	Eu.resize(Nu);	Ev.resize(Nu);
	for (int u = 0; u < Nu; u++) {
		Eu[u].resize(Nv);
		Ev[u].resize(Nv);
	}
	//归零初始化
	for (int u = 0; u < Nu; u++) {
		for (int v = 0; v < Nv; v++) {
			Eu[u][v] = complex<float>(0.0, 0.0);
			Ev[u][v] = complex<float>(0.0, 0.0);
		}
	}
}
void FDTDRadiator::SetProFieldFileName(const char* _filename) {
	Free_1D(FILENAME); FILENAME = NULL;
	int length = strlen(_filename);
	FILENAME = Allocate_1D(FILENAME, length);
	for (int i = 0; i < length; i++) { FILENAME[i] = _filename[i]; }
}

void FDTDRadiator::WriteApertureDataToFile() {
	FILE* Binwrite;
	Binwrite = fopen(FILENAME, "wb");
	float temp;
	fwrite(&Nu, sizeof(int), 1, Binwrite);
	fwrite(&Nv, sizeof(int), 1, Binwrite);
	temp = Lu / (Nu-1);
	fwrite(&temp, sizeof(float), 1, Binwrite);
	temp = Lv / (Nv-1); //dv
	fwrite(&temp, sizeof(float), 1, Binwrite);
	temp = float(Frequency);
	fwrite(&temp, sizeof(float), 1, Binwrite);	//还得改哦
	for(int v=0; v<Nv; v++){
		for (int u=0; u<Nu; u++) {
			fwrite(&Eu[u][v], sizeof(complex<float>), 1, Binwrite);
		}
	}
	for (int v = 0; v<Nv; v++) {
		for (int u = 0; u<Nu; u++) {
			fwrite(&Ev[u][v], sizeof(complex<float>), 1, Binwrite);
		}
	}
	fclose(Binwrite);

}


void FDTDRadiator::run() {
	//低阶TE模式
	fstream logfile;
	logfile.open("./FDTDRadiator.log", ios::out);
	logfile << "***This is the log file for FDTD Radiator Running.***" << endl;
	//这个数据是为了为FDTD提供输入的场分布
	complex<float>** Ex_Port;	Ex_Port = NULL;
	complex<float>** Ey_Port;	Ey_Port = NULL;
	complex<float>** Hx_Port;	Hx_Port = NULL;
	complex<float>** Hy_Port;	Hy_Port = NULL;
	if (returnInt) // 如果没有注册则不回调
	{
		returnInt(0, user);
	}

	//准备发射端口的场分布
	if (SourceKind == 1 && SourceType == 1) {
		//Part I Set Port Paras
		SourceModeGenerationT Port;
		Port.SetSource_Circular(SourceKind, SourceType, Rotation, WG_m, WG_n, Frequency, Radius);
		Port.SetOutputProperty(Ns);
		Port.FieldCalculation_CircularT();

		//Buffer for Read Port Fields 
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
		//cout << " Excitation Port Field Generated" << endl;
		logfile << " Excitation Port Field Generated" << endl;
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
		//cout << " Output Excitation Port Field done!" << endl;
		logfile << " Output Excitation Port Field done!" << endl;
		Ex0.~vector(); Ey0.~vector(); Hx0.~vector(); Hy0.~vector();
		//Ready to Feed FDTD
		Port.GetCircularWaveguideProperty(PHId, THETAd, Rcd, Lcd);
		//Check Parameters OK
		Rc = float(Rcd);
		Lc = float(Lcd);
		Lp = Lc / 4.0;
		//cout << " Cut Length Lc: " << Lc*1.0e3<<"mm " << endl;
		logfile<< " Cut Length Lc: " << Lc*1.0e3 << "mm " << endl;

		Port.~SourceModeGenerationT();

	}
	//Part II Setup Model
	
	//建立辐射器的剖分
	//SetUp Radiator Mesh
	CRadiator Radiator(SourceKind,SourceType,0,WG_m,WG_n,Ns,Ns,C_Speedf/float(Frequency),float(Radius),Lc,Lp,0,0,0,0,0,0);
	Radiator.SetLogfile(&logfile);
	if (SourceKind == 1 && SourceType == 1) {//低阶TE圆电辐射器
		Radiator.ResetRadiator_RoundL(float(C_Speed / Frequency), float(Radius), Ns, Ns, N_spa, Lc, Lp);
		Radiator.GenerateCellArray_RoundL();
		Radiator.SetFirstMirror_RoundL(F); //待查验
		//cout << " LowOrder Radiator and Mirror Parameters:" << endl;
		//cout << "   Radius:" << Radius << "m, Cut Propagation Section Length: " << Lc << "m, Mirror Height: " << Radiator.MirrorHeight << "m, Mirror Focus Length:" << F << "m." << endl;
		//cout << "   Model Domain Size, Nx: " << Radiator.Nx_model << ", Ny: " << Radiator.Ny_model << ", Nz: " << Radiator.Nz_model << endl;
		//cout << "   Leads to " << Radiator.Nx_model*Radiator.Ny_model*Radiator.Nz_model / 1.0e6 << " Mcells" << endl;
		logfile << " LowOrder Radiator and Mirror Parameters:" << endl;
		logfile << "   Radius:" << Radius << "m, Cut Propagation Section Length: " << Lc << "m, Mirror Height: " << Radiator.MirrorHeight << "m, Mirror Focus Length:" << F << "m." << endl;
		logfile << "   Model Domain Size, Nx: " << Radiator.Nx_model << ", Ny: " << Radiator.Ny_model << ", Nz: " << Radiator.Nz_model << endl;
		logfile << "   Leads to " << Radiator.Nx_model*Radiator.Ny_model*Radiator.Nz_model / 1.0e6 << " Mcells" << endl;
	}
	//选取DT 使其恰好是中心频率的4倍整数分之一
	SelectDT(Radiator.dx,Radiator.dy,Radiator.dz);
	//cout << "  Discrete interval in X: " << Radiator.dx << "m, in Y: " << Radiator.dy << "m, in Z: " << Radiator.dz << "m." << endl;
	//cout << "  Discrete interval in lambda, X: " << Radiator.dx*Frequency / C_Speedf << ", Y: " << Radiator.dy*Frequency / C_Speedf << ", Z: " << Radiator.dz*Frequency / C_Speedf << "." << endl;
	//cout << "  Required max dt: " << Requireddt << ", Used dt: " << cdt << ", T contains" << 1.0 / cdt / Frequency << "dt. Ref:" << NN * 4 << endl;
	logfile << "  Discrete interval in X: " << Radiator.dx << "m, in Y: " << Radiator.dy << "m, in Z: " << Radiator.dz << "m." << endl;
	logfile << "  Discrete interval in lambda, X: " << Radiator.dx*Frequency / C_Speedf << ", Y: " << Radiator.dy*Frequency / C_Speedf << ", Z: " << Radiator.dz*Frequency / C_Speedf << "." << endl;
	logfile << "  Required max dt: " << Requireddt << ", Used dt: " << cdt << ", T contains" << 1.0 / cdt / Frequency << "dt. Ref:" << NN * 4 << endl;



	//不同的频点-高阶模的交散半径不一样 最好逐点频计算
	//不同的频点-低阶模的变化要小一些 可以带宽计算
	CFDTD FDTD;	
	FDTD.SetLogfile(&logfile);			//设置logfile
	FDTD.SetReturnFloat(returnFloat, user);	//设置回调函数
	ApertureRadiation HuygensPro;	
	HuygensPro.SetLogfile(&logfile);	//设置logfile
	HuygensPro.SetReturnFloat(returnFloat, user);	//设置回调函数
	Position3D SourceCenter;



	//ParameterPass
	FDTD.Initial(OmpNum, NN, Radiator.Nx_model, Radiator.Ny_model, Radiator.Nz_model, N_spa, 8, cdt, Radiator.dx, Radiator.dy, Radiator.dz, Frequency);
	//int _threadNum, int _NN, int _Nx_model, int _Ny_model, int _Nz_model, int _Nspa, int _num_pml, float _dt, float _dx, float _dy, float _dz, float _Frequency

	FDTD.MemoryAllocate(0, 1, 0);
	//int _timemode, int _Nfreq, float _BW
	
	//SetUpHuygens - Freq;
	HuygensPro.SetUpFrequency(FDTD.Nfreq, FDTD.freq, FDTD.BW);
	HuygensPro.SetUpPropagatedAperture(AperturePosition, ApertureDirection, UDirection, VDirection, Lu, Lv, Nu, Nv);

	int Nx_exc = Ns;	int Ny_exc = Ns;
	//!!!!!!!!!!!!!待修改
	int Nz_exc = Radiator.Pz_model;
	int Px_exc = Radiator.Px_model + N_spa;
	int Py_exc = Radiator.Py_model + N_spa;
	//Feed Excitation from Waveguide Port

	//Load PEC Structure From Radiator
	FDTD.SetupModel(Radiator.EpsMap, Radiator.Esig);
	//int*** _EpsMap, float*** _Esig
	FDTD.SetExcPort(Nz_exc, Nx_exc, Ny_exc, Px_exc, Py_exc, Ex_Port, Ey_Port, Hx_Port, Hy_Port);
	//int _Nz_exc, int _Nx_exc, int _Ny_exc, int _Px_exc, int Py_exc, complex<float>** _Ex_Port, complex<float>** _Ey_Port, complex<float>** _Hx_Port, complex<float>** _Hy_Port

	//SetUpHuygens - Position 注意：低阶辐射器本身朝-X方向，一级镜朝+X方向
	SourceCenter.setX(-(Px_exc - N_spa*2)*Radiator.dx*0.5);
	SourceCenter.setY(0.0);
	SourceCenter.setZ(FDTD.Nz_DFT*FDTD.dz*0.5 - Lp - N_spa*ds);
	//cout << "SourceCenter, X, Y, Z, m: " << SourceCenter.X() << " " << SourceCenter.Y() << " " << SourceCenter.Z() << endl;
	//cout << "Huygens Box Span, X, Y, Z, mm: " << FDTD.Nx_DFT*FDTD.dx << " " << FDTD.Ny_DFT*FDTD.dy << " " << FDTD.Nz_DFT*FDTD.dz << endl;
	logfile << "SourceCenter, X, Y, Z, m: " << SourceCenter.X() << " " << SourceCenter.Y() << " " << SourceCenter.Z() << endl;
	logfile << "Huygens Box Span, X, Y, Z, mm: " << FDTD.Nx_DFT*FDTD.dx << " " << FDTD.Ny_DFT*FDTD.dy << " " << FDTD.Nz_DFT*FDTD.dz << endl;
	HuygensPro.SetUpHuygens(SourceCenter, FDTD.Nx_DFT, FDTD.Ny_DFT, FDTD.Nz_DFT, FDTD.dx, FDTD.dy, FDTD.dz);
		
	//getchar();
	if (returnInt)
	{
		returnInt(1, user);//开始FDTD 计算
	}
	FDTD.Update();
	if (returnInt)
	{
		returnInt(2, user);//开始Huygens外推计算
	}
	HuygensPro.Propagation5FaceBox(Eu, Ev, FDTD.HuygensBoxData, OmpNum, 0);
	WriteApertureDataToFile();
	//SetupHuygensParameters
	// HuygensPro.SetUpHuygens()

	//cout << "Calculation Completed. ENTER to exit. " << endl;
	logfile << "Calculation Complete." << endl;
	logfile << "Clean Memories" << endl;
	if (returnInt)
	{
		returnInt(3, user);//完成计算

	}
	FDTD.~FDTD();
	HuygensPro.~ApertureRadiation();
	logfile.close();
	Radiator.~CRadiator();

	//写回调位置！！！！！！！！！！！！！！！！！

}