#include "Radiator.h"
#include"GEMS_Memory.h"

CRadiator::CRadiator() {

}
CRadiator::CRadiator(
	int _Kind, int _Type, int _Rotation, int _m,
	int _n, int _Nx, int _Ny, float _lambda,	
	float _Radius, float _Lc,	float _Lp,	float _wa,
	float _wb,	float _Hp,	float _Ha,	float _aa,	float _bb):
	Kind(_Kind), Type(_Type),Rotation(_Rotation),m(_m),n(_n),
	Nx(_Nx),Ny(_Ny),lambda(_lambda),Radius(_Radius),Lc(_Lc),
	Lp(_Lp),wa(_wa),wb(_wb),Hp(_Hp),Ha(_Ha),aa(_aa),bb(_bb)
{
	if (Kind == 1) {//Round Radiator
		dx = Radius * 2 / Nx;
		dy = Radius * 2 / Ny;
		dz = dx;
		Nx_model = Nx + 20;	//5 ds for Structure 5 ds for Space 
		Ny_model = Ny + 20;
	}
	else if (Kind == 2) {//Round Radiator
		dx = Radius * 2 / Nx;
		dy = Radius * 2 / Ny;
		dz = dx;
		Nx_model = Nx + 20;
		Ny_model = Ny + 20;
	}
	else if (Kind == 3) {//Rectan Radiator
	
	}
}


//Reset Parameters 低阶斜切辐射器
void CRadiator::ResetRadiator_RoundL( float _lambda, float _Radius, int _Nx, int _Ny, int _N_spa, float _Lc, float _Lp) {
	//Assign Values
	Kind = 1; //低阶斜切辐射器
	lambda = _lambda;
	Radius = _Radius;
	Nx = _Nx;	Ny = _Ny;
	Lc = _Lc;	Lp = _Lp;
	N_spa = _N_spa;
	dx = Radius*2 / Nx;	dy = Radius*2 / Ny;
	dz = dx;
	Nx_model = _Nx + N_spa*2;
	Ny_model = _Ny + N_spa*2;
	Nz_model = int((Lc + Lp) / dz);
	Nz_model = Nz_model + N_spa*2;
	//cout << dx << " " << dy << " " << dz << endl;
	//cout << Nx_model << " " << Ny_model << " " << Nz_model << endl;
	//getchar();
}

void CRadiator::SetLogfile(fstream* logAdd)
{
	this->Logfile = logAdd;
}

//Reset Parameters 高阶螺旋辐射器
void CRadiator::ResetRadiator_RoundH(int _Rotation,float _lambda, float _Radius, int _Nx, int _Ny, int _N_spa, float _Lc, float _Lp) {
	//Assign Values
	Kind = 2; //高阶阶斜切辐射器
	Rotation = _Rotation;
	lambda = _lambda;
	Radius = _Radius;
	Nx = _Nx;	Ny = _Ny;	N_spa = _N_spa;
	Lc = _Lc;	Lp = _Lp;
	N_spa = _N_spa;
	dx = Radius * 2 / Nx;	dy = Radius * 2 / Ny;
	dz = dx;
	Nx_model = _Nx + N_spa*2;
	Ny_model = _Ny + N_spa*2;
	Nz_model = int((Lc + Lp) / dz);
	Nz_model = Nz_model + N_spa*2;
	//cout << dx << " " << dy << " " << dz << endl;
	//cout << Nx_model << " " << Ny_model << " " << Nz_model << endl;
	//getchar();

}

//Set First Mirror for the Low order TE round Raidator
//Parabolic Mirror 
void CRadiator::SetFirstMirror_RoundL(float _F) {
	float F = _F;
	float MirrorCenterZ;	//Mirror Center hight
	float MirrorHeightZ;		//Mirror Upside Height 
	float YMax;
	float XMin;
	int Nx_model_1, Ny_model_1, Nz_model_1;
	float* px_1;	float* py_1;	float* pz_1;
	px_1 = NULL;	py_1 = NULL;	pz_1 = NULL;
	int*** EpsMap_1; EpsMap_1 = NULL;
	float*** Esig_1;	 Esig_1 = NULL;
	//Reset Computation Region;
	MirrorCenterZ = Lp + (F + Radius) / (Radius * 2)*Lc;
	MirrorHeightZ = MirrorCenterZ + Lc;
	MirrorHeight = MirrorHeightZ;
	//Let x = a
	YMax = sqrt(4 * F * (Radius + F));
	//
	XMin = -F;
	//Recalculate Model Domain Size Note to Add _1
	//Z:
	Nz_model_1 = (MirrorHeightZ) / dz;	
	Nz_model_1 = Nz_model_1 + N_spa;
	//Y:
	Ny_model_1 = (YMax * 2) / dy;			
	Ny_model_1 = Ny_model_1 + N_spa * 2;
	//X:
	Nx_model_1 = (Nx_model - N_spa * 2) / 2 + int((-XMin) / dx);	
	Nx_model_1 = Nx_model_1 + N_spa * 2;
	//Nx_model_1 = Nx_model / 2 + int((-XMin) / dx) + N_spa;
	//cout << int((-XMin) / dx) << endl;
	//cout <<N_spa<<" "<< Nx_model_1 << " " << Nx_model << endl;
	(*Logfile) <<"discrete intervals(mm),X,Y,Z: "<< dx << " " << dy << " " << dz << endl;
	(*Logfile) <<"Model Sampling Numbers along X,Y, Z: "<< Nx_model << " " << Ny_model << " " << Nz_model << endl;

	EpsMap_1 = Allocate_3D(EpsMap_1,Nz_model_1, Ny_model_1, Nx_model_1);
	Esig_1 = Allocate_3D(Esig_1, Nz_model_1, Ny_model_1, Nx_model_1);
	px_1 = Allocate_1D(px_1, Nx_model_1);
	py_1 = Allocate_1D(py_1, Ny_model_1);
	pz_1 = Allocate_1D(pz_1, Nz_model_1);
	//prepare
	for (int i = 0; i < Nx_model_1; i++) {
		px_1[i] = (i - N_spa - int(-XMin / (dx)) + 0.5)*dx;
	}
	for (int j = 0; j < Ny_model_1; j++) {
		py_1[j] = (j - Ny_model_1 / 2 + 0.5)*dy;
	}
	for (int k = 0; k < Nz_model_1; k++) {
		pz_1[k] = (k + 0.5-N_spa)*dz;
	}

	//先对新数组赋初值
	for (int k = 0; k < Nz_model_1; k++) {
		for (int j = 0; j < Ny_model_1; j++) {
			for (int i = 0; i < Nx_model_1; i++) {
				EpsMap_1[k][j][i] = 1;
				Esig_1[k][j][i] = 0;
			}
		}
	}
	//然后填充原有的辐射器模型
	int XShift = int(-XMin / dx)+ N_spa - Nx_model/2;
	int YShift = (Ny_model_1 - Ny_model) / 2;
	int ZShift = 0;
	for (int k = 0; k < Nz_model; k++) {
		int kk = k + ZShift;
		for (int j = 0; j < Ny_model; j++) {
			int jj = j + YShift;
			for (int i = 0; i < Nx_model; i++) {
				int ii = i + XShift;
				EpsMap_1[kk][jj][ii] = EpsMap[k][j][i];
				Esig_1[kk][jj][ii] = Esig[k][j][i];
			}
		}
	}
	//绘制镜面
	float MirrorX;
	for (int k = 0; k < Nz_model_1; k++) {
		if (pz_1[k] < MirrorHeightZ) {//Set Z Bound
			for (int j = 0; j < Ny_model_1; j++) {
				if (abs(py_1[j]) < YMax) {	//Set Y Bound
					MirrorX = py_1[j] * py_1[j] / 4 / F - F;
					for (int i = 0; i < Nx_model_1; i++) {
						if ((px_1[i] < MirrorX) && (px_1[i] > MirrorX - 5 * dx)) {//Set PEC Mirror
							EpsMap_1[k][j][i] = 0;
							Esig_1[k][j][i] = Sig_PECf;
						}
					}
				}
			}
		}
	}
	//Reset Domain
	//Position of Excitation
	Px_model = int(-XMin / dx) + N_spa - Nx_model/2 + N_spa;
	Py_model = (Ny_model_1 - Ny_model)/2 + N_spa;
	Pz_model = N_spa;

	Free_3D(EpsMap, Nz_model);	Free_3D(Esig, Nz_model);
	Free_1D(px);	Free_1D(py);	Free_1D(pz);
	EpsMap = NULL;	Esig = NULL;
	px = NULL; py = NULL; pz = NULL;
	Nx_model = Nx_model_1;
	Ny_model = Ny_model_1;
	Nz_model = Nz_model_1;
	EpsMap = Allocate_3D(EpsMap, Nz_model, Ny_model, Nx_model);
	Esig = Allocate_3D(Esig, Nz_model, Ny_model, Nx_model);
	px = Allocate_1D(px, Nx_model);	py = Allocate_1D(py, Ny_model); pz = Allocate_1D(pz, Nz_model);
	
	//Assign Value
	for (int i = 0; i < Nx_model_1; i++) {		px[i] = px_1[i];	}
	for (int j = 0; j < Ny_model_1; j++) {		py[j] = py_1[j];	}
	for (int k = 0; k < Nz_model_1; k++) {		pz[k] = pz_1[k];	}


	//先对新数组赋初值
	for (int k = 0; k < Nz_model; k++) {
		for (int j = 0; j < Ny_model; j++) {
			for (int i = 0; i < Nx_model; i++) {
				EpsMap[k][j][i] = EpsMap_1[k][j][i];
				Esig[k][j][i] = Esig_1[k][j][i];
			}
		}
	}
	//输出模型待检查
	FILE* Checkmodel;
	Checkmodel = fopen("./CheckmodelI_Mirror.dat", "wb");
	fwrite(&Nx_model, sizeof(int), 1, Checkmodel);
	fwrite(&Ny_model, sizeof(int), 1, Checkmodel);
	fwrite(&Nz_model, sizeof(int), 1, Checkmodel);
	for (int k = 0; k < Nz_model; k++) {
		for (int j = 0; j < Ny_model; j++) {
			for (int i = 0; i < Nx_model; i++) {
				fwrite(&EpsMap[k][j][i], sizeof(int), 1, Checkmodel);
			}
		}
	}
	fclose(Checkmodel);
	Checkmodel = fopen("./CheckmodelF_Mirror.dat", "wb");
	fwrite(&Nx_model, sizeof(int), 1, Checkmodel);
	fwrite(&Ny_model, sizeof(int), 1, Checkmodel);
	fwrite(&Nz_model, sizeof(int), 1, Checkmodel);
	for (int k = 0; k < Nz_model; k++) {
		for (int j = 0; j < Ny_model; j++) {
			for (int i = 0; i < Nx_model; i++) {
				fwrite(&Esig[k][j][i], sizeof(float), 1, Checkmodel);
			}
		}
	}
	fclose(Checkmodel);
	//cout << "  checkmodel withMirror output done!" << endl;
	(*Logfile) << "  checkmodel withMirror output done!" << endl;
	//Clean memories
	Free_3D(EpsMap_1, Nz_model_1);	Free_3D(Esig_1, Nz_model_1);
	Free_1D(px_1);	Free_1D(py_1);	Free_1D(pz_1);
	EpsMap_1 = NULL;	Esig_1 = NULL;
	px_1 = NULL;	py_1 = NULL;  pz_1 = NULL;

}


//Set Low Order TE Round Radiator
void CRadiator::SetRadiator_RoundL(float _Lc, float _Lp) {
	Lc = _Lc;//	
	Lp = _Lp;
	dx = Radius * 2 / Nx;
	dy = dx; dz = dx;
	Nz_model = int((Lc + Lp) / dz);
	Nz_model = Nz_model + N_spa*2;
}


void CRadiator::GenerateCellArray_RoundL(void) {
	int i, j, k;
	float tempR;
	FILE* Checkmodel;
	//Position
	px = Allocate_1D(px, Nx_model);
	py = Allocate_1D(py, Ny_model);
	pz = Allocate_1D(pz, Nz_model);
	//cout << "  Position Allocated" << endl;
	(*Logfile) << " Position Allocated " << endl;
	for (i = 0; i < Nx_model; i++) {
		px[i] = (i - Nx_model / 2.0 + 0.5)*dx;
		//cout << px[i] << endl;
	}
	for (j = 0; j < Ny_model; j++) {
		py[j] = (j - Ny_model / 2.0 + 0.5)*dy;
	}
	for (k = 0; k < Nz_model; k++) {
		pz[k] = (k + 0.5-N_spa)*dz;
	}
	//Array 
	EpsMap = Allocate_3D(EpsMap, Nz_model, Ny_model, Nx_model);
	Esig= Allocate_3D(Esig, Nz_model, Ny_model, Nx_model);
	//cout << "  Model Array Allocated" << endl;
	(*Logfile) << "  Model Array Allocated" << endl;
	//Draw Shape
	for (k = 0; k < Nz_model; k++) {
		for (j = 0; j < Ny_model; j++) {
			for (i = 0; i < Nx_model; i++) {
				EpsMap[k][j][i] = 1;
				Esig[k][j][i] = 0;
			}
		}
	}
	//Round Waveguide
	for (k = 0; k < Nz_model; k++) {
		if(pz[k]<Lc+Lp){
			for (j = 0; j < Ny_model; j++) {
				for (i = 0; i < Nx_model; i++) {
					tempR = sqrt(px[i] * px[i] + py[j] * py[j]);
					if (tempR > Radius && tempR < Radius + 5 * dx) {
						EpsMap[k][j][i] = 0;
						Esig[k][j][i] = Sig_PECf;
					}
				}
			}
		}
	}
	//cout<<"Low Order TE Radiator, Radius: "<< Radius<<"m, Cut Height: "<<Lc<<"m, Propagation Section Height: "<<Lp <<"m."<< endl;
	(*Logfile) << "Low Order TE Radiator, Radius: " << Radius << "m, Cut Height: " << Lc << "m, Propagation Section Height: " << Lp << "m." << endl;
	//斜切口-冲X-方向
	for (k = 0; k < Nz_model; k++) {
		for (i = 0; i < Nx_model; i++) {
			if ( (pz[k]>Lp) && (pz[k] > px[i] * Lc*0.5 / Radius + Lp + Lc*0.5) ) {
				for (j = 0; j < Ny_model; j++) {
					EpsMap[k][j][i] = 1;
					Esig[k][j][i] = 0;
				}
			}
		}
	}
	//cout << "  Low Order Round Waveguide Radiator Model established" << endl;
	(*Logfile) << "  Low Order Round Waveguide Radiator Model established" << endl;
	Checkmodel = fopen("./CheckmodelI.dat", "wb");
	fwrite(&Nx_model, sizeof(int), 1, Checkmodel);
	fwrite(&Ny_model, sizeof(int), 1, Checkmodel);
	fwrite(&Nz_model, sizeof(int), 1, Checkmodel);
	for (k = 0; k < Nz_model; k++) {
		for (j = 0; j < Ny_model; j++) {
			for (i = 0; i < Nx_model; i++) {
				fwrite(&EpsMap[k][j][i], sizeof(int), 1, Checkmodel);
			}
		}
	}
	fclose(Checkmodel);
	Checkmodel = fopen("./CheckmodelF.dat", "wb");
	fwrite(&Nx_model, sizeof(int), 1, Checkmodel);
	fwrite(&Ny_model, sizeof(int), 1, Checkmodel);
	fwrite(&Nz_model, sizeof(int), 1, Checkmodel);
	for (k = 0; k < Nz_model; k++) {
		for (j = 0; j < Ny_model; j++) {
			for (i = 0; i < Nx_model; i++) {
				fwrite(&Esig[k][j][i], sizeof(float), 1, Checkmodel);
			}
		}
	}
	fclose(Checkmodel);
	//cout << "  checkmodel output done!" << endl;
	(*Logfile) << "  checkmodel output done!" << endl;
}

void CRadiator::GenerateCellArray_RoundH(void) {
	int i, j, k;
	float tempR;
	FILE* Checkmodel;
	//Position
	px = Allocate_1D(px, Nx_model);
	py = Allocate_1D(py, Ny_model);
	pz = Allocate_1D(pz, Nz_model);
	//cout << "Position Allocated" << endl;
	for (i = 0; i < Nx_model; i++) {
		px[i] = (i - Nx_model / 2.0 + 0.5)*dx;
	}
	for (j = 0; j < Ny_model; j++) {
		py[j] = (j - Ny_model / 2.0 + 0.5)*dy;
	}
	for (k = 0; k < Nz_model; k++) {
		pz[k] = (k + 0.5-N_spa)*dz;
	}
	//Array 
	EpsMap = Allocate_3D(EpsMap, Nz_model, Ny_model, Nx_model);
	Esig = Allocate_3D(Esig, Nz_model, Ny_model, Nx_model);
	//cout << "Model Array Allocated" << endl;

	//Draw Shape 
	for (k = 0; k < Nz_model; k++) {
		for (j = 0; j < Ny_model; j++) {
			for (i = 0; i < Nx_model; i++) {
				if (RPECStructure(px[i], py[j], pz[k])) {
					EpsMap[k][j][i] = 0;
					Esig[k][j][i] = Sig_PECf;
				}
				else {
					EpsMap[k][j][i] = 1;
					Esig[k][j][i] = 0;
				}
			}
		}
	}

	//cout <<"High Order TE Radiator:, Radius: "<< Radius << "m, Cut Height: " << Lc << "m, Propagation Section Height: " << Lp<<"m." << endl;
	(*Logfile) << "High Order TE Radiator:, Radius: " << Radius << "m, Cut Height: " << Lc << "m, Propagation Section Height: " << Lp << "m." << endl;
	//cout << "High Order Round Waveguide Radiator Model established" << endl;
	(*Logfile) << "High Order Round Waveguide Radiator Model established" << endl;
	Checkmodel = fopen("./CheckmodelI.dat", "wb");
	fwrite(&Nx_model, sizeof(int), 1, Checkmodel);
	fwrite(&Ny_model, sizeof(int), 1, Checkmodel);
	fwrite(&Nz_model, sizeof(int), 1, Checkmodel);
	for (k = 0; k < Nz_model; k++) {
		for (j = 0; j < Ny_model; j++) {
			for (i = 0; i < Nx_model; i++) {
				fwrite(&EpsMap[k][j][i], sizeof(int), 1, Checkmodel);
			}
		}
	}
	fclose(Checkmodel);
	Checkmodel = fopen("./CheckmodelF.dat", "wb");
	fwrite(&Nx_model, sizeof(int), 1, Checkmodel);
	fwrite(&Ny_model, sizeof(int), 1, Checkmodel);
	fwrite(&Nz_model, sizeof(int), 1, Checkmodel);
	for (k = 0; k < Nz_model; k++) {
		for (j = 0; j < Ny_model; j++) {
			for (i = 0; i < Nx_model; i++) {
				fwrite(&Esig[k][j][i], sizeof(float), 1, Checkmodel);
			}
		}
	}
	fclose(Checkmodel);
	//cout << "checkmodel output done!" << endl;
	(*Logfile) << "checkmodel output done!" << endl;
}

float CRadiator::RFxy(float x, float y) {
	float z;
	float phi = atan2(x, y)+PIf;
	//cout << phi << endl;
	//getchar();
	if (Rotation == 1) phi = 2 * PIf - phi;
	else if (Rotation == 2) phi = phi;
	z = Lc * phi / (2 * PIf);
	if (phi < 0) {
		(*Logfile) << "Error in CRadiator::RFxy, phi should not < 0" << endl;
	}
	return z;
}

bool CRadiator::RPECStructure(float x, float y, float z) {
	bool PEC = false;
	float rr;
	float thick = 5*dx;
	float top;
	rr = sqrt(x*x + y*y);
	top = RFxy(x, y) + Lp;

	if (rr>Radius && rr<Radius + thick && z<top) PEC = true;
	else PEC = false;

	return PEC;
}