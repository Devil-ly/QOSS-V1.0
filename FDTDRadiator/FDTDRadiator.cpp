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
	FDTDRadiator::returnInt = NULL;
	FDTDRadiator::returnFloat = NULL;
	FDTDRadiator::user = NULL;
	FDTDRadiator::FILENAME = NULL;//20180320
}
FDTDRadiator::~FDTDRadiator() {

}

void FDTDRadiator::SetInt(int input) 
{
	FDTDRadiator::number = input;
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

void FDTDRadiator::GetProFieldE(vector<vector<complex<double>>>& _Eu, vector<vector<complex<double>>>& _Ev, int _Nu, int _Nv)
{
	for (int u = 0; u < _Nu; u++) {
		for (int v = 0; v < _Nv; v++) {
			complex<double> temp;
			temp = complex<double>(double(FDTDRadiator::Eu[u][v].real()), double(FDTDRadiator::Eu[u][v].imag()));
			_Eu[u][v] = temp;
			temp = complex<double>(double(FDTDRadiator::Ev[u][v].real()), double(FDTDRadiator::Ev[u][v].imag()));
			_Ev[u][v] = temp;
		}
	}
}

void FDTDRadiator::LoadProFieldE(const char* _filename,vector<vector<complex<double>>>& _Eu, vector<vector<complex<double>>>& _Ev, int _Nu, int _Nv)
{
	FILE* Binread;
	Binread = fopen(_filename, "rb");
	int tempNu, tempNv;
	fread(&tempNu, sizeof(int), 1, Binread);	//Nu
	fread(&tempNv, sizeof(int), 1, Binread);	//Nv
	float temp;
	fread(&temp, sizeof(float), 1, Binread);	//du
	fread(&temp, sizeof(float), 1, Binread);	//dv
	fread(&temp, sizeof(float), 1, Binread);	//freq
	//注意 文件里是complex<float>的，目标位置是complex<double>的
	complex<float> readbuf;
	for (int v = 0; v<_Nv; v++) {
		for (int u = 0; u<_Nu; u++) {
			fread(&readbuf, sizeof(complex<float>), 1, Binread);
			_Eu[u][v] = complex<double>(double(readbuf.real()), double(readbuf.imag()));

		}
	}
	for (int v = 0; v<_Nv; v++) {
		for (int u = 0; u<_Nu; u++) {
			fread(&readbuf, sizeof(complex<float>), 1, Binread);
			_Ev[u][v] = complex<double>(double(readbuf.real()), double(readbuf.imag()));
		}
	}
	fclose(Binread);

}



void FDTDRadiator::SelectDT(float _dx, float _dy, float _dz){
	
	FDTDRadiator::NN = 5;
	float tempdt;
	//三个方向的间距中取最小的dt
	FDTDRadiator::Requireddt = _dx / C_Speedf * float(0.5);
	tempdt = _dy / C_Speedf * float(0.5); if (FDTDRadiator::Requireddt > tempdt) FDTDRadiator::Requireddt = tempdt;
	tempdt = _dz / C_Speedf * float(0.5); if (FDTDRadiator::Requireddt > tempdt) FDTDRadiator::Requireddt = tempdt;
	FDTDRadiator::cdt = float(1 / FDTDRadiator::Frequency / (NN*4.0));
	while (FDTDRadiator::cdt > FDTDRadiator::Requireddt) {
		FDTDRadiator::NN++;
		FDTDRadiator::cdt = float(1.0 / Frequency / (NN*4.0));
	}

}


void FDTDRadiator::SetUpLowOrderVlasovRadiator(int _WG_m, int _WG_n, double _Frequency, double _Radius, double _F, int _Ns, int _OmpNum) {
	//Set I Load Paras
	FDTDRadiator::WG_m = _WG_m;
	FDTDRadiator::WG_n = _WG_n;
	FDTDRadiator::Frequency = _Frequency;
	FDTDRadiator::Radius = _Radius;
	FDTDRadiator::F = _F;
	FDTDRadiator::Rotation = 0;
	FDTDRadiator::SourceKind = 1;//低阶模式
	FDTDRadiator::SourceType = 1;//TE模式
	FDTDRadiator::Ns = _Ns;
	FDTDRadiator::OmpNum = _OmpNum;
	//logfile.open("./FDTDRadiator.log", ios::out);
}

void FDTDRadiator::SetUpAperturePlane(Position3D _AperturePosition, Vector3D _ApertureDirection, Vector3D _UDirection, Vector3D _VDirection, int _Nu, int _Nv, double _Lu, double _Lv) {
	FDTDRadiator::AperturePosition = _AperturePosition;
	FDTDRadiator::ApertureDirection = _ApertureDirection;
	FDTDRadiator::UDirection = _UDirection;
	FDTDRadiator::VDirection = _VDirection;
	FDTDRadiator::Nu = _Nu;	
	FDTDRadiator::Nv = _Nv;
	FDTDRadiator::Lu = float(_Lu);	
	FDTDRadiator::Lv = float(_Lv);
	//再申请个内存吧 先U再V便于理解
	FDTDRadiator::Eu.resize(Nu);	
	FDTDRadiator::Ev.resize(Nu);
	for (int u = 0; u < Nu; u++) {
		FDTDRadiator::Eu[u].resize(Nv);
		FDTDRadiator::Ev[u].resize(Nv);
	}
	//归零初始化
	for (int u = 0; u < Nu; u++) {
		for (int v = 0; v < Nv; v++) {
			FDTDRadiator::Eu[u][v] = complex<float>(0.0, 0.0);
			FDTDRadiator::Ev[u][v] = complex<float>(0.0, 0.0);
		}
	}
}

void FDTDRadiator::WriteApertureDataToFile(const char* _filename) {
	FILE* Binwrite;
	Binwrite = fopen(_filename, "wb");
	float temp;
	fwrite(&(FDTDRadiator::Nu), sizeof(int), 1, Binwrite);
	fwrite(&(FDTDRadiator::Nv), sizeof(int), 1, Binwrite);
	temp = Lu / (Nu-1);	//du
	fwrite(&temp, sizeof(float), 1, Binwrite);
	temp = Lv / (Nv-1); //dv
	fwrite(&temp, sizeof(float), 1, Binwrite);
	temp = float(Frequency);
	fwrite(&temp, sizeof(float), 1, Binwrite);	//还得改哦 多频的情况
	for(int v=0; v<Nv; v++){
		for (int u=0; u<Nu; u++) {
			fwrite(&(FDTDRadiator::Eu[u][v]), sizeof(complex<float>), 1, Binwrite);
		}
	}
	for (int v = 0; v<Nv; v++) {
		for (int u = 0; u<Nu; u++) {
			fwrite(&(FDTDRadiator::Ev[u][v]), sizeof(complex<float>), 1, Binwrite);
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
		/*
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
		*/
		Ex0.~vector(); Ey0.~vector(); Hx0.~vector(); Hy0.~vector();
		//Ready to Feed FDTD
		Port.GetCircularWaveguideProperty(PHId, THETAd, Rcd, Lcd);
		//Check Parameters OK
		FDTDRadiator::Rc = float(Rcd);
		FDTDRadiator::Lc = float(Lcd);
		FDTDRadiator::Lp = Lc / float(4.0);
		//cout << " Cut Length Lc: " << Lc*1.0e3<<"mm " << endl;
		logfile<< " Cut Length Lc: " << FDTDRadiator::Lc*float(1.0e3) << "mm " << endl;

		Port.~SourceModeGenerationT();

	}
	//Part II Setup Model
	
	//建立辐射器的剖分
	//SetUp Radiator Mesh
	CRadiator Radiator(FDTDRadiator::SourceKind, FDTDRadiator::SourceType,0, FDTDRadiator::WG_m, FDTDRadiator::WG_n, FDTDRadiator::Ns, FDTDRadiator::Ns,C_Speedf/float(Frequency),float(FDTDRadiator::Radius), FDTDRadiator::Lc, FDTDRadiator::Lp,0,0,0,0,0,0);
	Radiator.SetLogfile(&logfile);
	if (FDTDRadiator::SourceKind == 1 && FDTDRadiator::SourceType == 1) {//低阶TE圆电辐射器
		Radiator.ResetRadiator_RoundL(float(C_Speed / FDTDRadiator::Frequency), float(FDTDRadiator::Radius), FDTDRadiator::Ns, FDTDRadiator::Ns, FDTDRadiator::N_spa, FDTDRadiator::Lc, FDTDRadiator::Lp);
		Radiator.GenerateCellArray_RoundL();
		Radiator.SetFirstMirror_RoundL(FDTDRadiator::F); //待查验
		//cout << " LowOrder Radiator and Mirror Parameters:" << endl;
		//cout << "   Radius:" << Radius << "m, Cut Propagation Section Length: " << Lc << "m, Mirror Height: " << Radiator.MirrorHeight << "m, Mirror Focus Length:" << F << "m." << endl;
		//cout << "   Model Domain Size, Nx: " << Radiator.Nx_model << ", Ny: " << Radiator.Ny_model << ", Nz: " << Radiator.Nz_model << endl;
		//cout << "   Leads to " << Radiator.Nx_model*Radiator.Ny_model*Radiator.Nz_model / 1.0e6 << " Mcells" << endl;
		logfile << " LowOrder Radiator and Mirror Parameters:" << endl;
		logfile << "   Radius:" << FDTDRadiator::Radius << "m, Cut Propagation Section Length: " << FDTDRadiator::Lc << "m, Mirror Height: " << Radiator.MirrorHeight << "m, Mirror Focus Length:" << FDTDRadiator::F << "m." << endl;
		logfile << "   Model Domain Size, Nx: " << Radiator.Nx_model << ", Ny: " << Radiator.Ny_model << ", Nz: " << Radiator.Nz_model << endl;
		logfile << "   Leads to " << Radiator.Nx_model*Radiator.Ny_model*Radiator.Nz_model / 1.0e6 << " Mcells" << endl;
	}
	//选取DT 使其恰好是中心频率的4倍整数分之一
	SelectDT(Radiator.dx,Radiator.dy,Radiator.dz);
	//cout << "  Discrete interval in X: " << Radiator.dx << "m, in Y: " << Radiator.dy << "m, in Z: " << Radiator.dz << "m." << endl;
	//cout << "  Discrete interval in lambda, X: " << Radiator.dx*Frequency / C_Speedf << ", Y: " << Radiator.dy*Frequency / C_Speedf << ", Z: " << Radiator.dz*Frequency / C_Speedf << "." << endl;
	//cout << "  Required max dt: " << Requireddt << ", Used dt: " << cdt << ", T contains" << 1.0 / cdt / Frequency << "dt. Ref:" << NN * 4 << endl;
	logfile << "  Discrete interval in X: " << Radiator.dx << "m, in Y: " << Radiator.dy << "m, in Z: " << Radiator.dz << "m." << endl;
	logfile << "  Discrete interval in lambda, X: " << Radiator.dx*FDTDRadiator::Frequency / C_Speedf << ", Y: " << Radiator.dy*FDTDRadiator::Frequency / C_Speedf << ", Z: " << Radiator.dz*FDTDRadiator::Frequency / C_Speedf << "." << endl;
	logfile << "  Required max dt: " << FDTDRadiator::Requireddt << ", Used dt: " << FDTDRadiator::cdt << ", T contains" << 1.0 / FDTDRadiator::cdt / FDTDRadiator::Frequency << "dt. Ref:" << FDTDRadiator::NN * 4 << endl;



	//不同的频点-高阶模的交散半径不一样 最好逐点频计算
	//不同的频点-低阶模的变化要小一些 可以带宽计算
	CFDTD FDTD;	
	FDTD.SetLogfile(&logfile);			//设置logfile
	FDTD.SetReturnFloat(FDTDRadiator::returnFloat, FDTDRadiator::user);	//设置回调函数
	ApertureRadiation HuygensPro;	
	HuygensPro.SetLogfile(&logfile);	//设置logfile
	HuygensPro.SetReturnFloat(FDTDRadiator::returnFloat, FDTDRadiator::user);	//设置回调函数
	Position3D SourceCenter;

	//ParameterPass
	FDTD.Initial(FDTDRadiator::OmpNum, FDTDRadiator::NN, Radiator.Nx_model, Radiator.Ny_model, Radiator.Nz_model, FDTDRadiator::N_spa, 8, FDTDRadiator::cdt, Radiator.dx, Radiator.dy, Radiator.dz, FDTDRadiator::Frequency);
	//int _threadNum, int _NN, int _Nx_model, int _Ny_model, int _Nz_model, int _Nspa, int _num_pml, float _dt, float _dx, float _dy, float _dz, float _Frequency

	FDTD.MemoryAllocate(0, 1, 0);	//这个是单频的形式
	//int _timemode, int _Nfreq, float _BW
	
	//SetUpHuygens - Freq;
	HuygensPro.SetUpFrequency(FDTD.Nfreq, FDTD.freq, FDTD.BW);
	HuygensPro.SetUpPropagatedAperture(FDTDRadiator::AperturePosition, FDTDRadiator::ApertureDirection, FDTDRadiator::UDirection, FDTDRadiator::VDirection, FDTDRadiator::Lu, FDTDRadiator::Lv, FDTDRadiator::Nu, FDTDRadiator::Nv);

	int Nx_exc = Ns;	int Ny_exc = Ns;
	
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
	SourceCenter.setZ(FDTD.Nz_DFT*FDTD.dz*0.5 - FDTDRadiator::Lp - FDTD.N_spa*FDTD.dz);
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
	HuygensPro.Propagation5FaceBox(FDTDRadiator::Eu, FDTDRadiator::Ev, FDTD.HuygensBoxData, FDTDRadiator::OmpNum, 0);
	FDTDRadiator::WriteApertureDataToFile("./PropagatedEField.dat");

	//cout << "Calculation Completed. ENTER to exit. " << endl;
	logfile << "Calculation Complete." << endl;
	logfile << "Clean Memories" << endl;
	if (returnInt)
	{
		returnInt(3, user);//完成计算

	}
	FDTD.~FDTD();
	HuygensPro.~ApertureRadiation();
	logfile.close();	//写文件
	Radiator.~CRadiator();

	//写回调位置！！！！！！！！！！！！！！！！！

}