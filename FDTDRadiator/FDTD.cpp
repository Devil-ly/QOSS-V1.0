/*
Source for 3D-FDTD
PEC Core for Radiator
v 1.0
Author: Ming Jin, Jinmingaps@163.com
Key feature:
	PEC Core
	Single Aperture Waveguide Excitation output
	Single DFT pair Record
	CA CB Iteration Control, Compatible with PML
*/

#include "FDTD.h"
#include "immintrin.h"

CFDTD::CFDTD(void){

	//Frequency
	freqs = NULL;
	lambdas = NULL;
	
	//Model & Fields datas:
	//Eps = NULL;
	Epsr = NULL;	Epsi = NULL;
	//Eps_Ex = NULL;	Eps_Ey = NULL;	Eps_Ez = NULL;
	Ex = NULL;	Ey = NULL;	Ez = NULL;
	Hx = NULL;	Hy = NULL;	Hz = NULL;

	//DFT box
	//Ex_z0 = NULL;		Ex_z1 = NULL;
	//Ex_x0 = NULL;		Ex_x1 = NULL;

	//Ey_z0 = NULL;		Ey_z1 = NULL;
	//Ey_x0 = NULL;	Ey_x1 = NULL;

	//Ez_z0 = NULL;		Ez_z1 = NULL;
	//Ez_x0 = NULL;		Ez_x1 = NULL;

	//Hx_z0 = NULL;	Hy_z1 = NULL;
	//Hy_x0 = NULL;	Hy_x1 = NULL;

	//Hz_z0 = NULL;	Hz_z1 = NULL;
	//Hz_x0 = NULL;	Hz_x1 = NULL;

	//ADEPML sigma
	//sigmaE = NULL;		sigmaH = NULL;
	//For PML
	//fyx = NULL;	fyz = NULL;
	//gxz = NULL;	gzx = NULL;

	//gyx = NULL;	gyz = NULL;
	//fxz = NULL;		fzx = NULL;

	returnFloat = NULL;
	user = NULL;
	
}
CFDTD::~CFDTD(void){

	//Frequency Sweep Allocate
	Free_1D(freqs);		freqs = NULL;
	Free_1D(wws);		wws = NULL;
	Free_1D(wwdts);		wwdts = NULL;
	Free_1D(lambdas);	lambdas = NULL;

	Free_3D(Ex, Nz + 2);	Ex = NULL;
	Free_3D(Ey, Nz + 2);	Ey = NULL;
	Free_3D(Ez, Nz + 2);	Ez = NULL;
	Free_3D(Dx, Nz + 2);	Dx = NULL;
	Free_3D(Dy, Nz + 2);	Dy = NULL;
	Free_3D(Dz, Nz + 2);	Dz = NULL;
	Free_3D(Hx, Nz + 2);	Hx = NULL;
	Free_3D(Hy, Nz + 2);	Hy = NULL;
	Free_3D(Hz, Nz + 2);	Hz = NULL;

	Free_3D(CA_Ex, Nz + 2);	CA_Ex = NULL;
	Free_3D(CA_Ey, Nz + 2);	CA_Ey = NULL;
	Free_3D(CA_Ez, Nz + 2);	CA_Ez = NULL;
	Free_3D(CB_Ex, Nz + 2);	CB_Ex = NULL;
	Free_3D(CB_Ey, Nz + 2);	CB_Ey = NULL;
	Free_3D(CB_Ez, Nz + 2);	CB_Ez = NULL;

	Free_3D(EpsMap, Nz + 2); EpsMap = NULL;
	Free_3D(Epsr, Nz + 2);	Epsr = NULL;
	Free_3D(Esig, Nz + 2);	Esig = NULL;

	Free_3D(gxz, num_pml * 2 + 2);	gxz = NULL;
	Free_3D(gyz, num_pml * 2 + 2);	gyz = NULL;
	Free_3D(fxz, num_pml * 2 + 2);	fxz = NULL;
	Free_3D(fyz, num_pml * 2 + 2);	fyz = NULL;

	Free_3D(gxy, Nz + 2);	gxy = NULL;
	Free_3D(gzy, Nz + 2);	gzy = NULL;
	Free_3D(fxy, Nz + 2);	fxy = NULL;
	Free_3D(fzy, Nz + 2);	fzy = NULL;

	Free_3D(gyx, Nz + 2);	gyx = NULL;
	Free_3D(gzx, Nz + 2);	gzx = NULL;
	Free_3D(fyx, Nz + 2);	fyx = NULL;
	Free_3D(fzx, Nz + 2);	fzx = NULL;

	Free_1D(sigmaEx);		sigmaEx = NULL;
	Free_1D(sigmaEy);		sigmaEy = NULL;
	Free_1D(sigmaEz);		sigmaEz = NULL;
	Free_1D(sigmaHx);		sigmaHx = NULL;
	Free_1D(sigmaHy);		sigmaHy = NULL;
	Free_1D(sigmaHz);		sigmaHz = NULL;

	Free_2D(Ex_AP_mag);		Ex_AP_mag = NULL;
	Free_2D(Ex_AP_rad);		Ex_AP_rad = NULL;
	Free_2D(Ex_AP_cos);		Ex_AP_cos = NULL;
	Free_2D(Ex_AP_sin);		Ex_AP_sin = NULL;

	Free_2D(Ey_AP_mag);		Ey_AP_mag = NULL;
	Free_2D(Ey_AP_rad);		Ey_AP_rad = NULL;
	Free_2D(Ey_AP_cos);		Ey_AP_cos = NULL;
	Free_2D(Ey_AP_sin);		Ey_AP_sin = NULL;

	Free_2D(Hx_AP_mag);		Hx_AP_mag = NULL;
	Free_2D(Hx_AP_rad);		Hx_AP_rad = NULL;
	Free_2D(Hx_AP_cos);		Hx_AP_cos = NULL;
	Free_2D(Hx_AP_sin);		Hx_AP_sin = NULL;

	Free_2D(Hy_AP_mag);		Hy_AP_mag = NULL;
	Free_2D(Hy_AP_rad);		Hy_AP_rad = NULL;
	Free_2D(Hy_AP_cos);		Hy_AP_cos = NULL;
	Free_2D(Hy_AP_sin);		Hy_AP_sin = NULL;

	Free_2D(Ex_xoz_real);	Ex_xoz_real = NULL;
	Free_2D(Ex_xoz_imag);	Ex_xoz_imag = NULL;
	Free_2D(Ey_xoz_real);	Ey_xoz_real = NULL;
	Free_2D(Ey_xoz_imag);	Ey_xoz_imag = NULL;
	Free_2D(Ez_xoz_real);	Ez_xoz_real = NULL;
	Free_2D(Ez_xoz_imag);	Ez_xoz_imag = NULL;

	Free_2D(Hx_xoz_real);	Hx_xoz_real = NULL;
	Free_2D(Hx_xoz_imag);	Hx_xoz_imag = NULL;
	Free_2D(Hy_xoz_real);	Hy_xoz_real = NULL;
	Free_2D(Hy_xoz_imag);	Hy_xoz_imag = NULL;
	Free_2D(Hz_xoz_real);	Hz_xoz_real = NULL;
	Free_2D(Hz_xoz_imag);	Hz_xoz_imag = NULL;

	Free_3D(Ex_z1, Nfreq);	Ex_z1 = NULL;
	Free_3D(Ey_z1, Nfreq);	Ey_z1 = NULL;
	Free_3D(Hx_z1, Nfreq);	Hx_z1 = NULL;
	Free_3D(Hy_z1, Nfreq);	Hy_z1 = NULL;

	Free_3D(Ey_x0, Nfreq);	Ey_x0 = NULL;
	Free_3D(Ez_x0, Nfreq);	Ez_x0 = NULL;
	Free_3D(Hy_x0, Nfreq);	Hy_x0 = NULL;
	Free_3D(Hz_x0, Nfreq);	Hz_x0 = NULL;

	Free_3D(Ey_x1, Nfreq);	Ey_x1 = NULL;
	Free_3D(Ez_x1, Nfreq);	Ez_x1 = NULL;
	Free_3D(Hy_x1, Nfreq);	Hy_x1 = NULL;
	Free_3D(Hz_x1, Nfreq);	Hz_x1 = NULL;

	Free_3D(Ex_y0, Nfreq);	Ex_y0 = NULL;
	Free_3D(Ez_y0, Nfreq);	Ez_y0 = NULL;
	Free_3D(Hx_y0, Nfreq);	Hx_y0 = NULL;
	Free_3D(Hz_y0, Nfreq);	Hz_y0 = NULL;

	Free_3D(Ex_y1, Nfreq);	Ex_y1 = NULL;
	Free_3D(Ez_y1, Nfreq);	Ez_y1 = NULL;
	Free_3D(Hx_y1, Nfreq);	Hx_y1 = NULL;
	Free_3D(Hz_y1, Nfreq);	Hz_y1 = NULL;

	Free_1D(Nfs);	Nfs = NULL;
	Free_1D(Nfn);	Nfn = NULL;
	Free_1D(Nor);	Nor = NULL;

	Free_1D(Nzs_cal);	Nzs_cal = NULL;
	Free_1D(Nzn_cal);	Nzn_cal = NULL;
	Free_1D(Nys_cal);	Nys_cal = NULL;
	Free_1D(Nyn_cal);	Nyn_cal = NULL;
	Free_1D(Ns_pml);	Ns_pml = NULL;
	Free_1D(Nn_pml);	Nn_pml = NULL;

	Free_1D(Nxs);		Nxs = NULL;
	Free_1D(Nxn);		Nxn = NULL;
	Free_1D(Nys);		Nys = NULL;
	Free_1D(Nyn);		Nyn = NULL;
	Free_1D(Nzs);		Nzs = NULL;
	Free_1D(Nzn);		Nzn = NULL;
	
}

//这个是回调函数
void CFDTD::SetReturnFloat(void(*returnFloat)(float, void*), void* _user) {
	this->returnFloat = returnFloat;
	this->user = _user;
}
void CFDTD::SetLogfile(fstream* logAdd) {
	this->Logfile = logAdd;
}

void CFDTD::MemoryAllocate(int _timemode,int _Nfreq, float _BW) 
{
	timemode = _timemode;
	Nfreq = _Nfreq;
	BW = _BW;
	//AllocateMemery
	if (timemode == 0) {//Single Frequency
		Nfreq = 1;
		BW = 0;
	}
	//Frequency Sweep Setup
	sfreq = freq - BW / 2;	efreq = freq + BW / 2;

	//Frequency Sweep Allocate
	freqs = Allocate_1D(freqs, Nfreq);
	wws = Allocate_1D(wws, Nfreq);
	wwdts = Allocate_1D(wwdts, Nfreq);
	lambdas = Allocate_1D(lambdas, Nfreq);

	//Frequency Sweep
	if (Nfreq>1) {
		for (int f = 0; f<Nfreq; f++) {
			freqs[f] = sfreq + BW / (Nfreq - 1)*f;
			wws[f] = 2 * PIf*freqs[f];
			wwdts[f] = wws[f] * dt;
			lambdas[f] = C_Speedf / freqs[f];
		}
		tao = 1.0 / BW;
	}
	else {//Nfreq = 1
		int f = 0;
		freqs[f] = freq;
		wws[f] = 2 * PIf*freqs[f];
		wwdts[f] = wws[f] * dt;
		lambdas[f] = C_Speedf / freqs[f];
		tao = 100*dt;
	}

	stopnn = int(tao * 2/dt + Nz*6);
	//stopnn = 100;
	Step2Show = int(stopnn / 5);


	//cout << "Total size: " << Nx << " " << Nz << ", time interval(s): " << dt << ", stopnn: " << stopnn << ", steptoshow: " << Step2Show << endl;
	//cout << " proceed " << endl;
	(*Logfile) << "Total size: " << Nx << " " << Nz << ", time interval(s): " << dt << ", stopnn: " << stopnn << ", steptoshow: " << Step2Show << endl;
	(*Logfile) << " proceed " << endl;
	//Field
	Ex = Allocate_3D(Ex, Nz + 2, Ny + 2, Nx + 2);	Ey = Allocate_3D(Ey, Nz + 2, Ny + 2, Nx + 2);	Ez = Allocate_3D(Ez, Nz + 2, Ny + 2, Nx + 2);
	Dx = Allocate_3D(Dx, Nz + 2, Ny + 2, Nx + 2);	Dy = Allocate_3D(Dy, Nz + 2, Ny + 2, Nx + 2);	Dz = Allocate_3D(Dz, Nz + 2, Ny + 2, Nx + 2);
	Hx = Allocate_3D(Hx, Nz + 2, Ny + 2, Nx + 2);	Hy = Allocate_3D(Hy, Nz + 2, Ny + 2, Nx + 2);	Hz = Allocate_3D(Hz, Nz + 2, Ny + 2, Nx + 2);
	//Update Middle
	CA_Ex = Allocate_3D(CA_Ex, Nz + 2, Ny + 2, Nx + 2);		CA_Ey = Allocate_3D(CA_Ey, Nz + 2, Ny + 2, Nx + 2);		CA_Ez = Allocate_3D(CA_Ez, Nz + 2, Ny + 2, Nx + 2);
	CB_Ex = Allocate_3D(CB_Ex, Nz + 2, Ny + 2, Nx + 2);		CB_Ey = Allocate_3D(CB_Ey, Nz + 2, Ny + 2, Nx + 2);		CB_Ez = Allocate_3D(CB_Ez, Nz + 2, Ny + 2, Nx + 2);
	//Model
	EpsMap = Allocate_3D(EpsMap, Nz + 2, Ny + 2, Nx + 2);
	Epsr = Allocate_3D(Epsr, Nz + 2, Ny + 2, Nx + 2);	Esig = Allocate_3D(Esig, Nz + 2, Ny + 2, Nx + 2);
	//PML Update
	//Z direction
	gxz = Allocate_3D(gxz, num_pml * 2 + 2, Ny + 2, Nx + 2); 	gyz = Allocate_3D(gyz, num_pml * 2 + 2, Ny + 2, Nx + 2);
	fxz = Allocate_3D(fxz, num_pml * 2 + 2, Ny + 2, Nx + 2);	fyz = Allocate_3D(fyz, num_pml * 2 + 2, Ny + 2, Nx + 2);
	//Y direction
	gxy = Allocate_3D(gxy, Nz + 2, num_pml * 2 + 2, Nx + 2); 	gzy = Allocate_3D(gzy, Nz + 2, num_pml * 2 + 2, Nx + 2);
	fxy = Allocate_3D(fxy, Nz + 2, num_pml * 2 + 2, Nx + 2); 	fzy = Allocate_3D(fzy, Nz + 2, num_pml * 2 + 2, Nx + 2);
	//X direction
	gyx = Allocate_3D(gyx, Nz + 2, Ny + 2, num_pml * 2 + 2); 	gzx = Allocate_3D(gzx, Nz + 2, Ny + 2, num_pml * 2 + 2);
	fyx = Allocate_3D(fyx, Nz + 2, Ny + 2, num_pml * 2 + 2); 	fzx = Allocate_3D(fzx, Nz + 2, Ny + 2, num_pml * 2 + 2);

	//PML Sigma: x direction
	sigmaEx = Allocate_1D(sigmaEx, num_pml);			sigmaHx = Allocate_1D(sigmaHx, num_pml);
	//Filling Sigma:
	for (int i = 0; i<num_pml; i++) {
		//thick = num_pml = 8; m = 3.5;        
		sigmaEx[i] = (3.5 + 1) / (150.0*PIf*dx) * pow((i + 0.5) / num_pml, 3.5) * pow(double(Eps0_Invf), 1.0); //已计入Eps0的修正 Small
		sigmaHx[i] = (3.5 + 1) / (150.0*PIf*dx) * pow((i + 1.0) / num_pml, 3.5) * pow(double(Eps0_Invf), 1.0); //已计入Eps0的修正 Bi
	}
	//PML Sigma: y direction
	sigmaEy = Allocate_1D(sigmaEy, num_pml);			sigmaHy = Allocate_1D(sigmaHy, num_pml);
	//Filling Sigma:
	for (int i = 0; i<num_pml; i++) {
		//thick = num_pml = 8; m = 3.5;        
		sigmaEy[i] = (3.5 + 1) / (150.0*PIf*dy) * pow((i + 0.5) / num_pml, 3.5) * pow(double(Eps0_Invf), 1.0); //已计入Eps0的修正 Small
		sigmaHy[i] = (3.5 + 1) / (150.0*PIf*dy) * pow((i + 1.0) / num_pml, 3.5) * pow(double(Eps0_Invf), 1.0); //已计入Eps0的修正 Bi
	}
	//PML Sigma: z direction
	sigmaEz = Allocate_1D(sigmaEz, num_pml);			sigmaHz = Allocate_1D(sigmaHz, num_pml);
	//Filling Sigma:
	for (int i = 0; i<num_pml; i++) {
		//thick = num_pml = 8; m = 3.5;        
		sigmaEz[i] = (3.5 + 1) / (150.0*PIf*dz) * pow((i + 0.5) / num_pml, 3.5) * pow(double(Eps0_Invf), 1.0); //已计入Eps0的修正 Small
		sigmaHz[i] = (3.5 + 1) / (150.0*PIf*dz) * pow((i + 1.0) / num_pml, 3.5) * pow(double(Eps0_Invf), 1.0); //已计入Eps0的修正 Bi
	}

	//AP aperture
	Ex_AP_mag = Allocate_2D(Ex_AP_mag, Ny + 2, Nx + 2);	Ex_AP_rad = Allocate_2D(Ex_AP_rad, Ny + 2, Nx + 2);
	Ex_AP_cos = Allocate_2D(Ex_AP_cos, Ny + 2, Nx + 2);	Ex_AP_sin = Allocate_2D(Ex_AP_sin, Ny + 2, Nx + 2);

	Ey_AP_mag = Allocate_2D(Ey_AP_mag, Ny + 2, Nx + 2);	Ey_AP_rad = Allocate_2D(Ey_AP_rad, Ny + 2, Nx + 2);
	Ey_AP_cos = Allocate_2D(Ey_AP_cos, Ny + 2, Nx + 2);	Ey_AP_sin = Allocate_2D(Ey_AP_sin, Ny + 2, Nx + 2);

	Hx_AP_mag = Allocate_2D(Hx_AP_mag, Ny + 2, Nx + 2);	Hx_AP_rad = Allocate_2D(Hx_AP_rad, Ny + 2, Nx + 2);
	Hx_AP_cos = Allocate_2D(Hx_AP_cos, Ny + 2, Nx + 2);	Hx_AP_sin = Allocate_2D(Hx_AP_sin, Ny + 2, Nx + 2);

	Hy_AP_mag = Allocate_2D(Hy_AP_mag, Ny + 2, Nx + 2);	Hy_AP_rad = Allocate_2D(Hy_AP_rad, Ny + 2, Nx + 2);
	Hy_AP_cos = Allocate_2D(Hy_AP_cos, Ny + 2, Nx + 2);	Hy_AP_sin = Allocate_2D(Hy_AP_sin, Ny + 2, Nx + 2);
	
	//DFT Allocate
	//2D Cut Single Frequency
	Ex_xoz_real = Allocate_2D(Ex_xoz_real, Nz + 2, Nx + 2);
	Ex_xoz_imag = Allocate_2D(Ex_xoz_imag, Nz + 2, Nx + 2);
	Ey_xoz_real = Allocate_2D(Ey_xoz_real, Nz + 2, Nx + 2);
	Ey_xoz_imag = Allocate_2D(Ey_xoz_imag, Nz + 2, Nx + 2);
	Ez_xoz_real = Allocate_2D(Ez_xoz_real, Nz + 2, Nx + 2);
	Ez_xoz_imag = Allocate_2D(Ez_xoz_imag, Nz + 2, Nx + 2);

	Hx_xoz_real = Allocate_2D(Hx_xoz_real, Nz + 2, Nx + 2);
	Hx_xoz_imag = Allocate_2D(Hx_xoz_imag, Nz + 2, Nx + 2);
	Hy_xoz_real = Allocate_2D(Hy_xoz_real, Nz + 2, Nx + 2);
	Hy_xoz_imag = Allocate_2D(Hy_xoz_imag, Nz + 2, Nx + 2);
	Hz_xoz_real = Allocate_2D(Hz_xoz_real, Nz + 2, Nx + 2);
	Hz_xoz_imag = Allocate_2D(Hz_xoz_imag, Nz + 2, Nx + 2);

	//2D DFT Record Box
	//z1 XOY
	Ex_z1 = Allocate_3D(Ex_z1, Nfreq, Ny_DFT + 2, Nx_DFT + 2); Ey_z1 = Allocate_3D(Ex_z1, Nfreq, Ny_DFT + 2, Nx_DFT + 2);
	Hx_z1 = Allocate_3D(Hx_z1, Nfreq, Ny_DFT + 2, Nx_DFT + 2); Hy_z1 = Allocate_3D(Hy_z1, Nfreq, Ny_DFT + 2, Nx_DFT + 2);
	//x0 YOZ
	Ey_x0 = Allocate_3D(Ey_x0, Nfreq, Nz_DFT + 2, Ny_DFT + 2); Ez_x0 = Allocate_3D(Ez_x0, Nfreq, Nz_DFT + 2, Ny_DFT + 2);
	Hy_x0 = Allocate_3D(Hy_x0, Nfreq, Nz_DFT + 2, Ny_DFT + 2); Hz_x0 = Allocate_3D(Hz_x0, Nfreq, Nz_DFT + 2, Ny_DFT + 2);
	//x1 YOZ
	Ey_x1 = Allocate_3D(Ey_x1, Nfreq, Nz_DFT + 2, Ny_DFT + 2); Ez_x1 = Allocate_3D(Ez_x1, Nfreq, Nz_DFT + 2, Ny_DFT + 2);
	Hy_x1 = Allocate_3D(Hy_x1, Nfreq, Nz_DFT + 2, Ny_DFT + 2); Hz_x1 = Allocate_3D(Hz_x1, Nfreq, Nz_DFT + 2, Ny_DFT + 2);
	//y0 XOZ
	Ex_y0 = Allocate_3D(Ex_y0, Nfreq, Nz_DFT + 2, Nx_DFT + 2); Ez_y0 = Allocate_3D(Ez_y0, Nfreq, Nz_DFT + 2, Nx_DFT + 2);
	Hx_y0 = Allocate_3D(Hx_y0, Nfreq, Nz_DFT + 2, Nx_DFT + 2); Hz_y0 = Allocate_3D(Hz_y0, Nfreq, Nz_DFT + 2, Nx_DFT + 2);
	//y1 XOZ
	Ex_y1 = Allocate_3D(Ex_y1, Nfreq, Nz_DFT + 2, Nx_DFT + 2); Ez_y1 = Allocate_3D(Ez_y1, Nfreq, Nz_DFT + 2, Nx_DFT + 2);
	Hx_y1 = Allocate_3D(Hx_y1, Nfreq, Nz_DFT + 2, Nx_DFT + 2); Hz_y1 = Allocate_3D(Hz_y1, Nfreq, Nz_DFT + 2, Nx_DFT + 2);
	

	//Frequency omp division
	Nfs = Allocate_1D(Nfs, threadNum);
	Nfn = Allocate_1D(Nfn, threadNum);
	//Z direction
	for (int i = 0; i<threadNum; i++) {
		Nfn[i] = (Nfreq) / threadNum;
		int rr = (Nfreq) % threadNum;
		if (i<rr && rr != 0)  Nfn[i] += 1;
		if (i == 0) {
			Nfs[i] = 0;
		}
		else {
			Nfs[i] = Nfs[i - 1] + Nfn[i - 1];
		}
		//cout<<Nfs[i]<<" "<<Nfn[i]<<" "<<i<<endl;	
	}
	
	//Nomarlization Factor Port
	Nor = Allocate_1D(Nor, Nfreq);
}
//Feed the Port from Port
void CFDTD::SetExcPort(int _Nz_exc, int _Nx_exc, int _Ny_exc, int _Px_exc, int _Py_exc, complex<float>** _Ex_Port, complex<float>** _Ey_Port, complex<float>** _Hx_Port, complex<float>** _Hy_Port) {
	Nz_exc = _Nz_exc;
	Nx_exc = _Nx_exc;	Ny_exc = _Ny_exc;
	Px_exc = _Px_exc;	Py_exc = _Py_exc;
	Ex_Port = Allocate_2D(Ex_Port, Ny_exc, Nx_exc);	Ey_Port = Allocate_2D(Ey_Port, Ny_exc, Nx_exc);
	Hx_Port = Allocate_2D(Hx_Port, Ny_exc, Nx_exc);	Hy_Port = Allocate_2D(Hy_Port, Ny_exc, Nx_exc);
	for (int j = 0; j < Ny_exc; j++) {
		for (int i = 0; i < Nx_exc; i++) {
			Ex_Port[j][i] = _Ex_Port[j][i];
			Ey_Port[j][i] = _Ey_Port[j][i];
			Hx_Port[j][i] = _Hx_Port[j][i];
			Hy_Port[j][i] = _Hy_Port[j][i];
		}
	}
	//Load into Aperture
	int iShift = Px_exc + 1;
	int jShift = Py_exc + 1;
	int ip, jp;
	//ReSet Excitation Aperture to Zero
	for (int j = 0; j <= Ny + 1; j++) {
		for (int i = 0; i <= Nx + 1; i++) {
			Ex_AP_mag[j][i] = 0;	Ex_AP_rad[j][i] = 0;
			Ey_AP_mag[j][i] = 0;	Ey_AP_rad[j][i] = 0;
			Hx_AP_mag[j][i] = 0;	Hx_AP_rad[j][i] = 0;
			Hy_AP_mag[j][i] = 0;	Hy_AP_rad[j][i] = 0;
		}
	}
	//Add Loaded Port Field into Aperture
	for (int j = 0; j < Ny_exc; j++) {
		jp = j + jShift;
		for (int i = 0; i < Nx_exc; i++) {
			ip = i + iShift;
			//Ex
			Ex_AP_mag[jp][ip] = sqrt(Ex_Port[j][i].real()*Ex_Port[j][i].real() + Ex_Port[j][i].imag()*Ex_Port[j][i].imag());
			Ex_AP_rad[jp][ip] = atan2(Ex_Port[j][i].imag(), Ex_Port[j][i].real());
			
			Ex_AP_cos[jp][ip] = cos(Ex_AP_rad[jp][ip]);	
			Ex_AP_sin[jp][ip] = sin(Ex_AP_rad[jp][ip]);

			//Ey
			Ey_AP_mag[jp][ip] = sqrt(Ey_Port[j][i].real()*Ey_Port[j][i].real() + Ey_Port[j][i].imag()*Ey_Port[j][i].imag());
			Ey_AP_rad[jp][ip] = atan2(Ey_Port[j][i].imag(), Ey_Port[j][i].real());
			
			Ey_AP_cos[jp][ip] = cos(Ey_AP_rad[jp][ip]);	
			Ey_AP_sin[jp][ip] = sin(Ey_AP_rad[jp][ip]);

			//Hx
			Hx_AP_mag[jp][ip] = sqrt(Hx_Port[j][i].real()*Hx_Port[j][i].real() + Hx_Port[j][i].imag()*Hx_Port[j][i].imag());
			Hx_AP_rad[jp][ip] = atan2(Hx_Port[j][i].imag(), Hx_Port[j][i].real());
			
			Hx_AP_cos[jp][ip] = cos(Hx_AP_rad[jp][ip]);	
			Hx_AP_sin[jp][ip] = sin(Hx_AP_rad[jp][ip]);

			//Hy
			Hy_AP_mag[jp][ip] = sqrt(Hy_Port[j][i].real()*Hy_Port[j][i].real() + Hy_Port[j][i].imag()*Hy_Port[j][i].imag());
			Hy_AP_rad[jp][ip] = atan2(Hy_Port[j][i].imag(), Hy_Port[j][i].real());
			
			Hy_AP_cos[jp][ip] = cos(Hy_AP_rad[jp][ip]);	
			Hy_AP_sin[jp][ip] = sin(Hy_AP_rad[jp][ip]);
		}
	}

	//cout for check
	FILE *ExcField;
	ExcField = fopen("./ExcField.dat", "wb");

	fwrite(&Nx, sizeof(int), 1, ExcField);
	fwrite(&Ny, sizeof(int), 1, ExcField);
	for (int j = 1; j <= Ny; j++) {
		for (int i = 1; i <= Nx; i++) {
			fwrite(&Ex_AP_mag[j][i], sizeof(float), 1, ExcField);	//1
			fwrite(&Ex_AP_rad[j][i], sizeof(float), 1, ExcField);	//2
			fwrite(&Ex_AP_cos[j][i], sizeof(float), 1, ExcField);	//3
			fwrite(&Ex_AP_sin[j][i], sizeof(float), 1, ExcField);	//4
			fwrite(&Ey_AP_mag[j][i], sizeof(float), 1, ExcField);	//5
			fwrite(&Ey_AP_rad[j][i], sizeof(float), 1, ExcField);	//6
			fwrite(&Ey_AP_cos[j][i], sizeof(float), 1, ExcField);	//7
			fwrite(&Ey_AP_sin[j][i], sizeof(float), 1, ExcField);	//8
			fwrite(&Hx_AP_mag[j][i], sizeof(float), 1, ExcField);	//9
			fwrite(&Hx_AP_rad[j][i], sizeof(float), 1, ExcField);	//10
			fwrite(&Hx_AP_cos[j][i], sizeof(float), 1, ExcField);	//11
			fwrite(&Hx_AP_sin[j][i], sizeof(float), 1, ExcField);	//12
			fwrite(&Hy_AP_mag[j][i], sizeof(float), 1, ExcField);	//13
			fwrite(&Hy_AP_rad[j][i], sizeof(float), 1, ExcField);	//14
			fwrite(&Hy_AP_cos[j][i], sizeof(float), 1, ExcField);	//15
			fwrite(&Hy_AP_sin[j][i], sizeof(float), 1, ExcField);	//16
		}
	}
	fclose(ExcField);
}
//Setup Model for Update
void CFDTD::SetupModel(int*** _EpsMap, float*** _Esig) { 
	//Allocate_Memory
	EpsMap = Allocate_3D(EpsMap, Nz + 2, Ny + 2, Nx + 2);
	Epsr = Allocate_3D(Epsr, Nz + 2, Ny + 2, Nx + 2);
	Esig = Allocate_3D(Esig, Nz + 2, Ny + 2, Nx + 2);
	//intialization
	for (int k = 0; k <= Nz+1; k++) {
		for (int j = 0; j <= Ny+1; j++) {
			for (int i = 0; i <= Nx+1; i++) {
				EpsMap[k][j][i] = 1;
				Epsr[k][j][i] = Eps0f;
				Esig[k][j][i] = 0;
			}
		}
	}

	int iShift, jShift, kShift;
	iShift = N_spa + 1;
	jShift = N_spa + 1;
	kShift = 1;
	int ip, jp, kp;
	//Load PEC Structure
	for (int k = 0; k < Nz_model; k++) {
		kp = k + kShift;
		for (int j = 0; j < Ny_model; j++) {
			jp = j + jShift;
			for (int i = 0; i < Nx_model; i++) {
				ip = i + iShift;
				EpsMap[kp][jp][ip] = _EpsMap[k][j][i];
				Esig[kp][jp][ip] = _Esig[k][j][i];
			}
		}
	}
	//output for Check

	//Generate CA CB Parameters
	float Eps_Ex, Eps_Ey, Eps_Ez;
	float sig_Ex, sig_Ey, sig_Ez;
	for (int k = 1; k <= Nz; k++) {
		for (int j = 1; j <= Ny; j++) {
			for (int i = 1; i <= Nx; i++) {

				//Ex Ey Ez Epsr
				Eps_Ex = (Epsr[k][j][i] + Epsr[k - 1][j][i] + Epsr[k][j - 1][i] + Epsr[k - 1][j - 1][i])*0.25;
				Eps_Ey = (Epsr[k][j][i] + Epsr[k][j][i - 1] + Epsr[k - 1][j][i] + Epsr[k - 1][j][i - 1])*0.25;
				Eps_Ez = (Epsr[k][j][i] + Epsr[k][j][i - 1] + Epsr[k][j - 1][i] + Epsr[k][j - 1][i - 1])*0.25;

				//Ex Ey Ez sig
				sig_Ex = (Esig[k][j][i] + Esig[k - 1][j][i] + Esig[k][j - 1][i] + Esig[k - 1][j - 1][i])*0.25;
				sig_Ey = (Esig[k][j][i] + Esig[k][j][i - 1] + Esig[k - 1][j][i] + Esig[k - 1][j][i - 1])*0.25;
				sig_Ez = (Esig[k][j][i] + Esig[k][j][i - 1] + Esig[k][j - 1][i] + Esig[k][j - 1][i - 1])*0.25;
				//Ey TE

				CA_Ex[k][j][i] = (Eps_Ex / dt - sig_Ex * 0.5) / (Eps_Ex / dt + sig_Ex * 0.5);
				CA_Ey[k][j][i] = (Eps_Ey / dt - sig_Ey * 0.5) / (Eps_Ey / dt + sig_Ey * 0.5);
				CA_Ez[k][j][i] = (Eps_Ez / dt - sig_Ez * 0.5) / (Eps_Ez / dt + sig_Ez * 0.5);

				CB_Ex[k][j][i] = 1 / (Eps_Ex / dt + sig_Ex * 0.5);
				CB_Ey[k][j][i] = 1 / (Eps_Ey / dt + sig_Ey * 0.5);
				CB_Ez[k][j][i] = 1 / (Eps_Ez / dt + sig_Ez * 0.5);
			}
		}
	}
	//done!
	//cout << "Eps Model Set Done!" << endl;
	(*Logfile)<< "Eps Model Set Done!" << endl;
	//Position vector
	xx = Allocate_1D(xx, Nx + 2);
	yy = Allocate_1D(yy, Ny + 2);
	zz = Allocate_1D(zz, Nz + 2);
	for (int i = 0; i <= Nx + 1; i++) {
		xx[i] = (i - 0.5 - (Nx - Rx_cal_AVX) / 2.0) * dx;
	}
	for (int j = 0; j <= Ny + 1; j++) {
		yy[j] = (j - 0.5) * dy;
	}
	for (int k = 0; k <= Nz + 1; k++) {
		zz[k] = (k - Nz_exc) * dz; //Set Port Position to zero
	}
	
}
void CFDTD::Initial(int _threadNum, int _NN, int _Nx_model, int _Ny_model, int _Nz_model, int _Nspa, int _num_pml, float _dt, float _dx, float _dy, float _dz, float _Frequency){//Modified
	
	start = clock();
	int i,j,k,f;
	//Load Basic Values
	threadNum = _threadNum;	NN = _NN;
	Nx_model = _Nx_model;	Ny_model = _Ny_model;	Nz_model = _Nz_model;
	N_spa = _Nspa;
	num_pml = _num_pml;
	if (N_spa < num_pml) {
		//cout << "N_spa(" << N_spa << ") < num_pml(" << num_pml << "), ERROR!!!!" << endl;
		(*Logfile) << "N_spa(" << N_spa << ") < num_pml(" << num_pml << "), ERROR!!!!" << endl;
		getchar();
		exit(-1);
	}
	dt = _dt;
	dx = _dx;
	dy = _dy;
	dz = _dz;
	freq = _Frequency;
	Nx = Nx_model + N_spa * 2;
	Ny = Ny_model + N_spa * 2;
	Nz = Nz_model + N_spa;

	lambda = C_Speedf / freq;
	ww = 2 * PIf*freq;
	wwdt = ww*dt;
		
	//Computational Domain without PML -3 direction
	Nx_cal = Nx - num_pml*2;	
	Ny_cal = Ny - num_pml*2;	
	Nz_cal = Nz - num_pml*2;
	//Total Domain Resident for SIMD Acceleration AVX
	if (((Nx_cal) % AVX_Count)>0) {
		Rx_cal_AVX = AVX_Count - (Nx_cal % AVX_Count);
	}
	else {
		Rx_cal_AVX = 0;
	}
	//ReSet Computation Total Domain Considering AVX
	Nx_cal = Nx_cal + Rx_cal_AVX;	//Computation domain
	Nx = Nx + Rx_cal_AVX;			//Total domain
	//Set DFT
	//Record Number;
	Nz_DFT = Nz - N_spa;	Nx_DFT = Nx - N_spa*2 - Rx_cal_AVX;	Ny_DFT = Ny - N_spa*2;
	//Record Position;
	//Up XoZ
	Pz_DFT_1 = Nz_model;
	//YoZ
	Px_DFT_0 = N_spa;
	Px_DFT_1 = N_spa + Nx_model;
	//XoZ
	Py_DFT_0 = N_spa;
	Py_DFT_1 = N_spa + Ny_model;
	
	//Open-mp Dividing Z_calculation domain _ no pml
	Nzs_cal = Allocate_1D(Nzs_cal,threadNum);//Start
	Nzn_cal = Allocate_1D(Nzn_cal,threadNum);//End
	for(int i=0;i<threadNum;i++){
		Nzn_cal[i] = (Nz_cal)/threadNum;
		int rr = (Nz_cal)%threadNum;
		if(i<rr && rr != 0)  Nzn_cal[i] +=1;
		if(i==0){
			Nzs_cal[i] = 1;
		}else{
			Nzs_cal[i] = Nzs_cal[i-1]+Nzn_cal[i-1];
		}
		//cout<<Nzs[i]<<" "<<Nzn[i]<<" "<<i<<endl;	
	}	
	//Open-mp Dividing Z_calculation domain _ no pml
	Nys_cal = Allocate_1D(Nys_cal, threadNum);//Start
	Nyn_cal = Allocate_1D(Nyn_cal, threadNum);//End
	for (int i = 0; i<threadNum; i++) {
		Nyn_cal[i] = (Ny_cal) / threadNum;
		int rr = (Ny_cal) % threadNum;
		if (i<rr && rr != 0)  Nyn_cal[i] += 1;
		if (i == 0) {
			Nys_cal[i] = 1;
		}
		else {
			Nys_cal[i] = Nys_cal[i - 1] + Nyn_cal[i - 1];
		}
		//cout<<Nzs[i]<<" "<<Nzn[i]<<" "<<i<<endl;	
	}

	//待修改 - OpenMP
	//Open-mp Dividing PML domain
	Ns_pml = Allocate_1D(Ns_pml,threadNum);//Start
	Nn_pml = Allocate_1D(Nn_pml,threadNum);//End
	for(int i=0;i<threadNum;i++){
		Nn_pml[i] = (num_pml*2)/threadNum;
		int rr = (num_pml*2)%threadNum;
		if(i<rr && rr != 0)  Nn_pml[i] +=1;
		if(i==0){
			Ns_pml[i] = 1;
		}else{
			Ns_pml[i] = Ns_pml[i-1]+Nn_pml[i-1];
		}
		//cout<<Nzs[i]<<" "<<Nzn[i]<<" "<<i<<endl;	
	}	

	//Open-mp Dividing //Total Domain with PML
	Nxs = Allocate_1D(Nxs,threadNum);//Start
	Nxn = Allocate_1D(Nxn,threadNum);//Number
	Nys = Allocate_1D(Nys,threadNum);//Start
	Nyn = Allocate_1D(Nyn,threadNum);//Number
	Nzs = Allocate_1D(Nzs,threadNum);//Start
	Nzn = Allocate_1D(Nzn,threadNum);//Number
	
	//Split Domain for omp Only x direction
	for(int i=0;i<threadNum;i++){
		Nxn[i] = (Nx)/threadNum;
		int rr = (Nx)%threadNum;
		if(i<rr && rr != 0)  Nxn[i] +=1;
		if(i==0){
			Nxs[i] = 1;
		}else{
			Nxs[i] = Nxs[i-1]+Nxn[i-1];
		}
		//cout<<Nxs[i]<<" "<<Nxn[i]<<" "<<i<<endl;	
	}
	//Split Domain for omp Only y direction
	for (int i = 0; i<threadNum; i++) {
		Nyn[i] = (Ny) / threadNum;
		int rr = (Ny) % threadNum;
		if (i<rr && rr != 0)  Nyn[i] += 1;
		if (i == 0) {
			Nys[i] = 1;
		}
		else {
			Nys[i] = Nys[i - 1] + Nyn[i - 1];
		}
		//cout<<Nys[i]<<" "<<Nyn[i]<<" "<<i<<endl;	
	}
	//Split Domain for omp Only z direction
	for(int i=0;i<threadNum;i++){
		Nzn[i] = (Nz)/threadNum;
		int rr = (Nz)%threadNum;
		if(i<rr && rr != 0)  Nzn[i] +=1;
		if(i==0){
			Nzs[i] = 1;
		}else{
			Nzs[i] = Nzs[i-1]+Nzn[i-1];
		}
		//cout<<Nzs[i]<<" "<<Nzn[i]<<" "<<i<<endl;	
	}

	computed = 0;


	/*
	//Far-Field Calculation //0.5deg interval -90~90
	//Direction Theta
	Ntheta = 91;	thetas = Allocate_1D(thetas,Ntheta);
	for(int th=0;th<Ntheta;th++){// 0deg to 90deg, in rad
		thetas[th] = th*PIf/180;
	}
	//Direction Phi //0deg to 359deg, in rad;
	Nphi = 180;	phis = Allocate_1D(phis, Nphi);
	for (int ph = 0; ph < Nphi; ph++) {
		phis[ph] = ph*(2 * PIf) / Nphi;
	}
	phi_s = Allocate_1D(phi_s, threadNum);
	phi_n = Allocate_1D(phi_n, threadNum);
	//Split PHI Domain for omp parallelization 
	for (int i = 0; i<threadNum; i++) {
		phi_n[i] = (Nphi) / threadNum;
		int rr = (Nphi) % threadNum;
		if (i<rr && rr != 0)  phi_n[i] += 1;
		if (i == 0) {
			phi_s[i] = 0;
		}
		else {
			phi_s[i] = phi_s[i - 1] + phi_n[i - 1];
		}
		//cout<<Nzs[i]<<" "<<Nzn[i]<<" "<<i<<endl;	
	}

	//FarField Results
	FarPhi = Allocate_3D(FarPhi,Nfreq,Nphi,Ntheta);
	FarTheta = Allocate_3D(FarTheta,Nfreq,Nphi,Ntheta);
	FoutPhi = Allocate_3D(FoutPhi,Nfreq,Nphi,Ntheta);
	FoutTheta = Allocate_3D(FoutTheta,Nfreq,Nphi,Ntheta);
	FoutPhi_inc = Allocate_3D(FoutPhi_inc,Nfreq,Nphi,Ntheta);
	FoutTheta_inc = Allocate_3D(FoutTheta_inc,Nfreq,Nphi,Ntheta);
	*/

	//Initial timeline
	//float temp;
	//temp = Timesourcep(0,1);
	//cout << "FDTD solver Initialized." << endl;
	(*Logfile) << "FDTD solver Initialized." << endl;

}
void CFDTD::Update(){
	
    //cout<<"Start to Calculate field."<<endl;
	(*Logfile) << "Start to Calculate field." << endl;
	if (model == 0) {
		com_start = clock();
		//cout << "Initialization tooks:" << (com_start - start)*1.0 / CLOCKS_PER_SEC << "seconds" << endl;
		(*Logfile) << "Initialization tooks:" << (com_start - start)*1.0 / CLOCKS_PER_SEC << "seconds" << endl;
	}
	nn = 0;
	//cout << "Step2Show: " << Step2Show << endl;
	(*Logfile) << "Step2Show: " << Step2Show << endl;
	while(nn < stopnn){
		Judge();
		UpdateE();
		BoundaryE_AVX();
		AddSourceE();
		UpdateH();
		BoundaryH_AVX();
		AddSourceH();

		Record();
		Output();
		//cout << ".";
		(*Logfile) << ".";
		if(nn%Step2Show == 0 && nn!=0){
			//cout <<"step"<< nn <<" done! "<< stopnn - nn<<" left!"<<" ";
			(*Logfile) << "step" << nn << " done! " << stopnn - nn << " left!" << " ";
		}
		//回调计算进度
		if (nn % 10 == 0) {
			if (returnFloat) // 没有注册则不回调
			{
				returnFloat(float(nn*1.0 / stopnn * 100.0), user);

			}
		}
		if (nn == stopnn - 1) computed = 1;
		nn++;
		//cout<<nn<<endl;
	}
}
void CFDTD::Record(){//Modified
	if (timemode == 1) {	//Perform DFT
	
	}
	/*
	if(computed == 0){//in computation
		//DFT Record
		omp_set_num_threads(threadNum);
		#pragma omp parallel
		{
			int id =  omp_get_thread_num();
			int i,k,j,jj,ii,kk;
			int i_sse;
			//single Frequency ww*dt
			register float coedte_cos = cos(wwdt*nn);		register float coedte_sin = -sin(wwdt*nn);
			register float coedth_cos = cos(wwdt*(nn+0.5));	register float coedth_sin = -sin(wwdt*(nn+0.5));

			register float dt2 = dt/2;			register float dt4 = dt/4;			register float dt0 = dt;
			
			//Simple version
			register __m256 m0,m1,m2;										
			register __m256 SSE_dt = _mm256_set1_ps(dt0);
			register __m256 SSE_cos;	
			register __m256 SSE_sin;		
			register __m256 SSE_F;
			register __m256 SSE_R; 
			register __m256 SSE_I;
			
			//Single Frequency 2D Cut: //Divided in Z
			//E Fields
			SSE_cos = _mm256_set1_ps(coedte_cos);			SSE_sin = _mm256_set1_ps(coedte_sin);

			jj = Ny_model / 2;
			for(k = Nzs[id]; k <= Nzs[id] + Nzn[id]-1; k++){
				for(i=1; i<=Nx; i+=AVX_Count){
					i_sse = i+AVX_Count;

					SSE_F = _mm256_load_ps(Ex[k][jj]+i);
					SSE_R = _mm256_load_ps(Ex_xoz_real[k]+i);
					SSE_I = _mm256_load_ps(Ex_xoz_imag[k]+i);
					m0 = _mm256_mul_ps(SSE_dt,SSE_F);
					m1 = _mm256_mul_ps(SSE_cos,m0);
					m2 = _mm256_mul_ps(SSE_sin,m0);
					m1 = _mm256_add_ps(m1,SSE_R);
					m2 = _mm256_add_ps(m2,SSE_I);  

					_mm256_storeu_ps(Ex_xoz_real[k]+i,m1);
					_mm256_storeu_ps(Ex_xoz_imag[k]+i,m2);
					
					SSE_F = _mm256_load_ps(Ey[k][jj] +i);
					SSE_R = _mm256_load_ps(Ey_xoz_real[k]+i);
					SSE_I = _mm256_load_ps(Ey_xoz_imag[k]+i);
					m0 = _mm256_mul_ps(SSE_dt,SSE_F);
					m1 = _mm256_mul_ps(SSE_cos,m0);
					m2 = _mm256_mul_ps(SSE_sin,m0);
					m1 = _mm256_add_ps(m1,SSE_R);
					m2 = _mm256_add_ps(m2,SSE_I);  
					
					_mm256_storeu_ps(Ey_xoz_real[k]+i,m1);
					_mm256_storeu_ps(Ey_xoz_imag[k]+i,m2);
					
					
					SSE_F = _mm256_load_ps(Ez[k][jj]+i);
					SSE_R = _mm256_load_ps(Ez_xoz_real[k]+i);
					SSE_I = _mm256_load_ps(Ez_xoz_imag[k]+i);
					m0 = _mm256_mul_ps(SSE_dt,SSE_F);
					m1 = _mm256_mul_ps(SSE_cos,m0);
					m2 = _mm256_mul_ps(SSE_sin,m0);
					m1 = _mm256_add_ps(m1,SSE_R);
					m2 = _mm256_add_ps(m2,SSE_I);  
					_mm256_storeu_ps(Ez_xoz_real[k]+i,m1);
					_mm256_storeu_ps(Ez_xoz_imag[k]+i,m2);					

				}
			}

			//H Field
			SSE_cos = _mm256_set1_ps(coedth_cos);
			SSE_sin = _mm256_set1_ps(coedth_sin);
			for (k = Nzs[id]; k <= Nzs[id] + Nzn[id] - 1; k++) {
				//kk = k + num_pml + N_spa;
				for (i = 1; i <= Nx; i += AVX_Count) {
					//ii = i + num_pml + N_spa;
					i_sse = i + AVX_Count;
					//load H Fields
					SSE_F = _mm256_load_ps(Hx[k][jj]+i);
					SSE_R = _mm256_load_ps(Hx_xoz_real[k]+i);
					SSE_I = _mm256_load_ps(Hx_xoz_imag[k]+i);
					m0 = _mm256_mul_ps(SSE_dt,SSE_F);
					m1 = _mm256_mul_ps(SSE_cos,m0);
					m2 = _mm256_mul_ps(SSE_sin,m0);
					m1 = _mm256_add_ps(m1,SSE_R);
					m2 = _mm256_add_ps(m2,SSE_I);  
					_mm256_storeu_ps(Hx_xoz_real[k]+i,m1);
					_mm256_storeu_ps(Hx_xoz_imag[k]+i,m2);		

					
					SSE_F = _mm256_load_ps(Hy[k][jj]+i);
					SSE_R = _mm256_load_ps(Hy_xoz_real[k]+i);
					SSE_I = _mm256_load_ps(Hy_xoz_imag[k]+i);
					m0 = _mm256_mul_ps(SSE_dt,SSE_F);
					m1 = _mm256_mul_ps(SSE_cos,m0);
					m2 = _mm256_mul_ps(SSE_sin,m0);
					m1 = _mm256_add_ps(m1,SSE_R);
					m2 = _mm256_add_ps(m2,SSE_I);  
					_mm256_storeu_ps(Hy_xoz_real[k]+i,m1);
					_mm256_storeu_ps(Hy_xoz_imag[k]+i,m2);				
					
					SSE_F = _mm256_load_ps(Hz[k][jj]+i);
					SSE_R = _mm256_load_ps(Hz_xoz_real[k]+i);
					SSE_I = _mm256_load_ps(Hz_xoz_imag[k]+i);
					m0 = _mm256_mul_ps(SSE_dt,SSE_F);
					m1 = _mm256_mul_ps(SSE_cos,m0);
					m2 = _mm256_mul_ps(SSE_sin,m0);
					m1 = _mm256_add_ps(m1,SSE_R);
					m2 = _mm256_add_ps(m2,SSE_I);  
					_mm256_storeu_ps(Hz_xoz_real[k]+i,m1);
					_mm256_storeu_ps(Hz_xoz_imag[k]+i,m2);

				}
			}			

			//DFT_Aperture Record
			int f;
			int k0,k1,j0,j1,i0,i1;
			if(model == 1){

				for(f=Nfs[id];f<=Nfs[id] + Nfn[id]-1;f++){
					//Sweep of Frequency
					complex<float> coedte = complex<float>(cos(wwdts[f]*nn),-sin(wwdts[f]*nn));
					complex<float> coedth = complex<float>(cos(wwdts[f]*(nn+0.5)),-sin(wwdts[f]*(nn+0.5)));
					//in 2D case 4 lines form a box
					//Z0 Z1 divided in X
					kk = Nz_DFT; //z1
					for(j=1; j<=Ny;	j++){
						for(i=1; i<=Nx; i++){
							//E-field
							Ex_z1[f][j][i] = Ex_z1[f][j][i] + coedte*(Ex[kk+1][j][i] + Ex[kk+1][j+1][i])*float(0.5);
							Ey_z1[f][j][i] = Ey_z1[f][j][i] + coedte*(Ey[kk+1][j][i] + Ey[kk+1][j][i+1])*float(0.5);
							//H-field
							Hx_z1[f][j][i] = Hx_z1[f][j][i] + coedth*((Hx[kk][j][i] + Hx[kk+1][j][i] + Hx[kk][j][i+1] + Hx[kk+1][j][i+1])*float(0.25));
							Hy_z1[f][j][i] = Hy_z1[f][j][i] + coedth*((Hy[kk][j][i] + Hy[kk+1][j][i] + Hy[kk][j+1][i] + Hy[kk+1][j+1][i])*float(0.25));
						}
					}
				}		
			}
			
			if(model == 0){//inc
				for(f=Nfs[id];f<=Nfs[id] + Nfn[id]-1;f++){
					//Sweep of Frequency
					complex<float> coedte = complex<float>(cos(wwdts[f]*nn),-sin(wwdts[f]*nn));
					complex<float> coedth = complex<float>(cos(wwdts[f]*(nn+0.5)),-sin(wwdts[f]*(nn+0.5)));

					kk = Nz_DFT; //z1
					for (j = 1; j <= Ny; j++) {
						for (i = 1; i <= Nx; i++) {
							//E-field
							Ex_z1_inc[f][j][i] = Ex_z1_inc[f][j][i] + coedte*(Ex[kk+1][j][i] + Ex[kk+1][j+1][i])*float(0.5);
							Ey_z1_inc[f][j][i] = Ey_z1_inc[f][j][i] + coedte*(Ey[kk+1][j][i] + Ey[kk+1][j][i+1])*float(0.5);
							//H-field
							Hx_z1_inc[f][j][i] = Hx_z1_inc[f][j][i] + coedth*((Hx[kk][j][i] + Hx[kk+1][j][i] + Hx[kk][j][i+1] + Hx[kk+1][j][i+1])*float(0.25));
							Hy_z1_inc[f][j][i] = Hy_z1_inc[f][j][i] + coedth*((Hy[kk][j][i] + Hy[kk+1][j][i] + Hy[kk][j+1][i] + Hy[kk+1][j+1][i])*float(0.25));
						}//for i					
					}
					kk = Nz_DFT_0; //z0
					for (j = 1; j <= Ny;j++){
						for(i=1; i<=Nx; i++){
							//E-field
							Ex_z0_inc[f][j][i] = Ex_z0_inc[f][j][i] + coedte*(Ex[kk+1][j][i] + Ex[kk+1][j+1][i])*float(0.5);
							Ey_z0_inc[f][j][i] = Ey_z0_inc[f][j][i] + coedte*(Ey[kk+1][j][i] + Ey[kk+1][j][i+1])*float(0.5);
							//H-field
							Hx_z0_inc[f][j][i] = Hx_z0_inc[f][j][i] + coedth*((Hx[kk][j][i] + Hx[kk+1][j][i] + Hx[kk][j][i+1] + Hx[kk+1][j][i+1])*float(0.25));
							Hy_z0_inc[f][j][i] = Hy_z0_inc[f][j][i] + coedth*((Hy[kk][j][i] + Hy[kk+1][j][i] + Hy[kk][j+1][i] + Hy[kk+1][j+1][i])*float(0.25));
						}//for i
					}
				}//for freq
			}//model == 0
		}//omp
	}//in computation
	
	//Far-Field Calculation
	if(computed==1){//Computation Finished
		if(model == 1){//DFT 数据 对消入射泄露
			cout<<"Count!"<<endl;
			for(int f=0; f<Nfreq; f++){
				for(int j = 1; j <= Ny; j++){
					for (int i = 1; i <= Nx; i++) {
						//TM
						Hy_z1[f][j][i] = Hy_z1[f][j][i] - Hy_z1_inc[f][j][i];//
						Ex_z1[f][j][i] = Ex_z1[f][j][i] - Ex_z1_inc[f][j][i];//
						//TE
						Hx_z1[f][j][i] = Hx_z1[f][j][i] - Hx_z1_inc[f][j][i];//
						Ey_z1[f][j][i] = Ey_z1[f][j][i] - Ey_z1_inc[f][j][i];//
					}
				}//for i
			}//for f
		}// model == 1

		Farfield();

		//To File Buffer
		if(model == 1){//Scattering
			int f;
			int ph,th;
			for(f=0;f<Nfreq;f++){
				for(ph = 0; ph < Nphi; ph++) {
					for (th = 0; th < Ntheta; th++) {
						FoutPhi[f][ph][th] = FarPhi[f][ph][th];
						FoutTheta[f][ph][th] = FarTheta[f][ph][th];
					}//th
				}//ph
			}//f
		}//With model
		else if(model == 0){//Ref Inc
			int f;
			int th,ph;
			for(f=0;f<Nfreq;f++){
				for (ph = 0; ph < Nphi; ph++) {
					for (th = 0; th < Ntheta; th++) {
						FoutPhi_inc[f][ph][th] = FarPhi[f][ph][th];
						FoutTheta_inc[f][ph][th] = FarTheta[f][ph][th];
					}//th
				}//ph
			}//f
		}//incident only
	}//After computation
	*/
}
/*
void CFDTD::Farfield() {
	//complex<float> result(2);
	int t, p;	//index
	float th, ph;	//angle value in degree
	float sint, cost;		float sinp, cosp;
	int f, i, j, k;	//index
	float k0;		//wavenumber
	float kx, ky, kz;	//Vector Wave number
	float deg2rad;	deg2rad = PI / 180.0;
	complex<float> kfactor;	//in Far-field computation
	register complex<float> zero(0.0, 0.0);
	complex<float> fx = zero;		complex<float> fy = zero;	complex<float> fz = zero;
	complex<float> fmx = zero;		complex<float> fmy = zero; 	complex<float> fmz = zero;
	int* DFTyn;	DFTyn = Allocate_1D(DFTyn, threadNum);
	int* DFTys; DFTys = Allocate_1D(DFTys, threadNum);
	complex<float>** Jx_z1;		Jx_z1 = Allocate_2D(Jx_z1, Ny + 2, Nx + 2);
	complex<float>** Jmx_z1;	Jmx_z1 = Allocate_2D(Jmx_z1, Ny + 2, Nx + 2);
	complex<float>** Jy_z1;		Jy_z1 = Allocate_2D(Jy_z1, Ny + 2, Nx + 2);
	complex<float>** Jmy_z1;	Jmy_z1 = Allocate_2D(Jmy_z1, Ny + 2, Nx + 2);
	complex<float>** yky;		yky = Allocate_2D(yky, threadNum, Ny + 2);
	complex<float>** xkx;		xkx = Allocate_2D(xkx, threadNum, Nx + 2);
	complex<float>* fxo;		fxo = Allocate_1D(fxo, threadNum);
	complex<float>* fyo;		fyo = Allocate_1D(fyo, threadNum);
	complex<float>* fzo;		fzo = Allocate_1D(fzo, threadNum);
	complex<float>* fmxo;		fmxo = Allocate_1D(fmxo, threadNum);
	complex<float>* fmyo;		fmyo = Allocate_1D(fmyo, threadNum);
	complex<float>* fmzo;		fmzo = Allocate_1D(fmzo, threadNum);

	//Surface Current Distributions per frequency
	//xoy z1
	for (int i = 0; i<threadNum; i++) {
		DFTyn[i] = (Ny) / threadNum;
		int rr = (Ny) % threadNum;
		if (i<rr && rr != 0)  DFTyn[i] += 1;
		if (i == 0) { DFTys[i] = 1; }
		else { DFTys[i] = DFTys[i - 1] + DFTyn[i - 1]; }
	}


	for (f = 0; f<Nfreq; f++) {//Frequency Sweep
		//Load Tangential Field into Current @ frequency F
		
		if (model == 0) {//Incidence Only
			//z1 XOY Turn around the H-field Let Incident Go upward
			for (j = 1; j <= Ny; j++) {
				for (i = 1; i <= Nx; i++) {
					Jx_z1[j][i] = Hy_z0_inc[f][j][i];				Jy_z1[j][i] = -Hx_z0_inc[f][j][i];
					Jmx_z1[j][i] = Ey_z0_inc[f][j][i];				Jmy_z1[j][i] = -Ex_z0_inc[f][j][i];
				}
			}
		}
		else {//Including RoughSurface
			  //z1 XOY
			for (j = 1; j <= Ny; j++) {
				for (i = 1; i <= Nx; i++) {
					Jx_z1[j][i] = -Hy_z1[f][j][i];				Jy_z1[j][i] = Hx_z1[f][j][i];
					Jmx_z1[j][i] = Ey_z1[f][j][i];				Jmy_z1[j][i] = -Ex_z1[f][j][i];
				}
			}
		}

		//wavenumber
		k0 = 2 * PI / lambdas[f];
		kfactor = complex<float>(0, -k0);
		kfactor = kfactor / float(4 * PI);
		kfactor = kfactor * complex<float>(cos(-k0), sin(-k0));
		//r = 1; unit

		//UpSpace Computation
		for (t = 0; t<Ntheta; t++) {
			cout << ".";
			th = thetas[t];	sint = sin(th);	cost = cos(th);//already in rad
			for (p = 0; p <Nphi; p++) {
				ph = phis[p];	sinp = sin(ph);		cosp = cos(ph);//already in rad

				//far-field vector
				kx = sint*cosp*k0;	ky = sint*sinp*k0;	kz = cost*k0;

				//begin integration								
				omp_set_num_threads(threadNum);
				#pragma omp parallel
				{
					int id = omp_get_thread_num();
					int io, jo, ko;
					//傅立叶变换核

					float xkxf, ykyf, zkzf;
					//积分中临时变量
					complex<float> fxi, fyi, fzi;		complex<float> fmxi, fmyi, fmzi;
					//积分面所在位置
					complex<float> xkx0;		complex<float> yky0;		complex<float> zkz0;
					//傅立叶积分位置-均匀采样点

					for (j = 0; j <= Ny + 1; j++) {
						ykyf = ky*(j - Ny / 2 - 0.5)*ds;	yky[id][j] = complex<float>(cos(ykyf), sin(ykyf));
					}
					for (i = 0; i <= Nx + 1; i++) {
						xkxf = kx*(i - Nx / 2 - 0.5)*ds;	xkx[id][i] = complex<float>(cos(xkxf), sin(xkxf));
					}
					//begin intergation
					//set zero;
					fxo[id] = zero;			fyo[id] = zero;			fzo[id] = zero;
					fmxo[id] = zero;		fmyo[id] = zero;		fmzo[id] = zero;

					//xoy 1;
					//kz-z position
					zkz0 = complex<float>(cos(kz*DCount*ds), sin(kz*DCount*ds));
					//外积分-OMP
					for (jo = DFTys[id]; jo<DFTys[id] + DFTyn[id]; jo++) {
						fxi = zero;	fyi = zero;	fmxi = zero;		fmyi = zero;
						for (io = 1; io <= Nx; io++) {
							fxi = fxi + xkx[id][io] * Jx_z1[jo][io];	fyi = fyi + xkx[id][io] * Jy_z1[jo][io];
							fmxi = fmxi + xkx[id][io] * Jmx_z1[jo][io];	fmyi = fmyi + xkx[id][io] * Jmy_z1[jo][io];

						}//io
						fxo[id] = fxo[id] + yky[id][jo] * zkz0 * fxi;	fyo[id] = fyo[id] + yky[id][jo] * zkz0 * fyi;
						fmxo[id] = fmxo[id] + yky[id][jo] * zkz0 * fmxi;fmyo[id] = fmyo[id] + yky[id][jo] * zkz0 * fmyi;
					}//jo
				}//omp division

				 //累加前先清零
				fx = zero;		fy = zero;		fz = zero;
				fmx = zero;		fmy = zero;		fmz = zero;

				for (i = 0; i<threadNum; i++) {
					fx = fx + fxo[i];		fy = fy + fyo[i];		fz = fz + fzo[i];
					fmx = fmx + fmxo[i];	fmy = fmy + fmyo[i];	fmz = fmz + fmzo[i];
				}//sum-up omp divided results

				fx = fx*float(ds*ds);		fy = fy*float(ds*ds);		fz = fz*float(ds*ds);
				fmx = fmx*float(ds*ds);		fmy = fmy*float(ds*ds);		fmz = fmz*float(ds*ds);

				if (model == 1) {
					if (Pol == 1) {
						FarTheta[f][p][t] = -kfactor*((fmx*sinp - fmy*cosp) - float(Eta0)*(fx*cost*cosp + fy*cost*sinp - fz*sint));
						FarPhi[f][p][t] = kfactor*((fmx*cost*cosp + fmy*cost*sinp - fmz*sint) - float(Eta0)*(-fx*sinp + fy*cosp));
					}
					else if (Pol == 2) {
						FarTheta[f][p][t] = kfactor*(float(Eta0)*(fx*cost*cosp + fy*cost*sinp - fz*sint) + (-fmx*sinp + fmy*cosp));
						FarPhi[f][p][t] = -kfactor*(float(Eta0)*(fx*sinp - fy*cosp) + (fmx*cost*cosp + fmy*cost*sinp - fmz*sint));
					}
				}//if
				else if (model == 0) {
					if (Pol == 1) {
						FarTheta[f][p][t] = -kfactor*((fmx*sinp - fmy*cosp) - float(Eta0)*(fx*cost*cosp + fy*cost*sinp - fz*sint));
						FarPhi[f][p][t] = kfactor*((fmx*cost*cosp + fmy*cost*sinp - fmz*sint) - float(Eta0)*(-fx*sinp + fy*cosp));
					}
					else if (Pol == 2) {
						FarTheta[f][p][t] = kfactor*(float(Eta0)*(fx*cost*cosp + fy*cost*sinp - fz*sint) + (-fmx*sinp + fmy*cosp));
						FarPhi[f][p][t] = -kfactor*(float(Eta0)*(fx*sinp - fy*cosp) + (fmx*cost*cosp + fmy*cost*sinp - fmz*sint));
					}

				}//else if
			}//phi 
		}//theta
		cout << endl;
	}//frequency Sweep

	Free_1D(DFTyn);	Free_1D(DFTys);
	Free_2D(Jx_z1);	Free_2D(Jmx_z1);
	Free_2D(Jy_z1);	Free_2D(Jmy_z1);
	Free_2D(yky);	Free_2D(xkx);
	Free_1D(fxo);	Free_1D(fyo);	Free_1D(fzo);
	Free_1D(fmxo);	Free_1D(fmyo);	Free_1D(fmzo);
	cout << "FarField Calculation Completed!" << endl;
}
*/
void CFDTD::BoundaryE_AVX() {
	//Open MP
	omp_set_num_threads(threadNum);
	#pragma omp parallel
	{
		int id = omp_get_thread_num();

		//iteration
		int i, j, k;
		int ii, jj, kk;
		int i_AVX;		int i_sse;
		//for pml
		float sigmaX = 0.0;		float sigmaY = 0.0;		float sigmaZ = 0.0;
		float SEx, SEy, SEz;	float SDx, SDy, SDz;
		float fxy0 = 0;			float fxz0 = 0;
		float fyz0 = 0;			float fyx0 = 0;
		float fzx0 = 0;			float fzy0 = 0;
		float idt = 1 / dt;
		float Epso = Eps0f;		
		int ks, ke;
		int index;	int index_pml;

		//AVX_dataset
		register __m256 SSE_dt = _mm256_set1_ps(dt);
		register __m256 SSE_idt = _mm256_set1_ps(idt);
		//register __m256 SSE_ds = _mm256_set1_ps(dso); //4
		register __m256 SSE_d1;
		register __m256 SSE_d2;
		register __m256 SSE_E;		register __m256 SSE_D;		register __m256 SSE_SD;
		register __m256 SSE_H1H; 		register __m256 SSE_H1L;
		register __m256 SSE_H2H; 		register __m256 SSE_H2L;
		register __m256 SSE_sigma1;		register __m256 SSE_sigma2;
		register __m256 SSE_sigmaX;
		register __m256 SSE_k1;		register __m256 SSE_k2;
		register __m256 SSE_CA;
		register __m256 SSE_CB;
		register __m256 m0;
		register __m256 m1;
		register __m256 m2;

		//Re Distribute:
		
		//Set 1
		//X- X+ PML //Z Full //Y Full
		ks = Nzs[id];
		ke = Nzs[id] + Nzn[id] - 1;
		for (k = ks; k <= ke; k++) { //Z Full
			sigmaZ = 0.0;	kk = 0;
			if (k <= num_pml) { //k-
				sigmaZ = sigmaHz[num_pml - k];
				kk = k;
			}
			if (k >= Nz - num_pml + 1) { //Y+
				sigmaZ = sigmaEz[k - Nz + num_pml - 1];
				kk = k - (Nz - num_pml) + num_pml;
			}
			for (j = 1; j <= Ny; j++) { // Y Full
				sigmaY = 0.0;	jj = 0;
				if (j <= num_pml) { //Y-
					sigmaY = sigmaHy[num_pml - j];
					jj = j;
				}
				if (j >= Ny - num_pml + 1) { //Y+
					sigmaY = sigmaEy[j - Ny + num_pml - 1];
					jj = j - (Ny - num_pml) + num_pml;
				}
				//i_AVX_pml 
				for (i_AVX = 0; i_AVX <= 1; i_AVX++) {//Address
					//i_AVX=0:index = 1; i_AVX = 1: index = Nx-num_pml+1
					index = 1 + (Nx - num_pml)*i_AVX;
					//i_AVX=0:index_pml = 1; i_AVX = 1: index_pml = 1+num_pml
					index_pml = 1 + num_pml*i_AVX;
					if (i_AVX == 0) {
						//-i 正常顺序是前面index大，后面index小
						//-i 顺序是前面index小，后面index大
						SSE_sigmaX = _mm256_set_ps(sigmaHx[0],sigmaHx[1],sigmaHx[2],sigmaHx[3],sigmaHx[4],sigmaHx[5],sigmaHx[6],sigmaHx[7]);
						//sse_Ez_max = _mm256_set_ps(Ez[k][j][i_sse], Ez[k][j][i_sse - 1], Ez[k][j][i_sse - 2], Ez[k][j][i_sse - 3], Ez[k][j][i_sse - 4], Ez[k][j][i_sse - 5], Ez[k][j][i_sse - 6], Ez[k][j][i_sse - 7]);
					}
					else {
						//i 正常顺序是前面index大，后面index小
						SSE_sigmaX = _mm256_set_ps(sigmaEx[7],sigmaEx[6],sigmaEx[5],sigmaEx[4],sigmaEx[3],sigmaEx[2],sigmaEx[1],sigmaEx[0]);
					}

					//Set AVX input
					//Ex
					SSE_SD = _mm256_loadu_ps(Dx[k][j] + index);
					SSE_D = SSE_SD;
					SSE_H1H = _mm256_loadu_ps(Hz[k][j] + index);
					SSE_H1L = _mm256_loadu_ps(Hz[k][j-1] + index);
					SSE_d1 = _mm256_set1_ps(dz);
					SSE_H2H = _mm256_loadu_ps(Hy[k][j] + index);
					SSE_H2L = _mm256_loadu_ps(Hy[k-1][j] + index);
					SSE_d2 = _mm256_set1_ps(dy);
					SSE_sigma1 = _mm256_set1_ps(sigmaZ);
					SSE_sigma2 = _mm256_set1_ps(sigmaY);
					SSE_k1 = _mm256_loadu_ps(fxz[kk][j] + index);
					SSE_k2 = _mm256_loadu_ps(fxy[k][jj] + index);
					ADEPML_AVX(SSE_H1H,SSE_H1L,SSE_H2H,SSE_H2L,SSE_dt,SSE_d1,SSE_d2,SSE_sigma1,SSE_sigma2,SSE_k1,SSE_k2,SSE_D);
					//SDx = Dx[k][j][i];
					//ADEPML(Hz[k][j][i], Hz[k][j - 1][i], Hy[k][j][i], Hy[k - 1][j][i], dso, dso, sigmaZ, sigmaY, fxz[kk][j][i], fxy[k][jj][i], Dx[k][j][i]);
					SSE_E = _mm256_loadu_ps(Ex[k][j] + index);
					SSE_CA = _mm256_loadu_ps(CA_Ex[k][j] + index);
					SSE_CB = _mm256_loadu_ps(CB_Ex[k][j] + index);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					m0 = _mm256_sub_ps(SSE_D, SSE_SD);
					m2 = _mm256_mul_ps(SSE_idt, m0);
					m2 = _mm256_mul_ps(SSE_CB, m2);
					SSE_E = _mm256_add_ps(m1, m2);
					//Ex[k][j][i] = CA_Ex[k][j][i] * Ex[k][j][i] + CB_Ex[k][j][i] * idt*(Dx[k][j][i] - SDx);
					//Store
					_mm256_storeu_ps(fxz[kk][j] + index, SSE_k1);
					_mm256_storeu_ps(fxy[k][jj] + index, SSE_k2);
					_mm256_storeu_ps(Dx[k][j] + index, SSE_D);
					_mm256_storeu_ps(Ex[k][j] + index, SSE_E);

					//SSE_E = _mm256_loadu_ps(Ex[k][j] + i);
					//Ex
					//SDx = Dx[k][j][i];
					//ADEPML(Hz[k][j][i], Hz[k][j - 1][i], Hy[k][j][i], Hy[k - 1][j][i], dso, dso, sigmaZ, sigmaY, fxz[kk][j][i], fxy[k][jj][i], Dx[k][j][i]);
					//Ex[k][j][i] = CA_Ex[k][j][i] * Ex[k][j][i] + CB_Ex[k][j][i] * idt*(Dx[k][j][i] - SDx);
					
					//Ey
					SSE_SD = _mm256_loadu_ps(Dy[k][j] + index);
					SSE_D = SSE_SD;
					SSE_H1H = _mm256_loadu_ps(Hx[k][j] + index);
					SSE_H1L = _mm256_loadu_ps(Hx[k-1][j] + index);
					SSE_d1 = _mm256_set1_ps(dx);
					SSE_H2H = _mm256_loadu_ps(Hz[k][j] + index);
					SSE_H2L = _mm256_loadu_ps(Hz[k][j] - 1 + index);
					SSE_d2 = _mm256_set1_ps(dz);
					SSE_sigma1 = SSE_sigmaX;
					SSE_sigma2 = _mm256_set1_ps(sigmaZ);
					SSE_k1 = _mm256_loadu_ps(fyx[k][j] + index_pml);
					SSE_k2 = _mm256_loadu_ps(fyz[kk][j] + index);
					ADEPML_AVX(SSE_H1H, SSE_H1L, SSE_H2H, SSE_H2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_D);
					//SDx = Dx[k][j][i];
					//ADEPML(Hx[k][j][i], Hx[k-1][j][i], Hz[k][j][i], Hz[k][j][i-1], dso, dso, sigmaX, sigmaZ, fyx[k][j][ii], fyz[kk][j][i], Dy[k][j][i]);
					SSE_E = _mm256_loadu_ps(Ey[k][j] + index);
					SSE_CA = _mm256_loadu_ps(CA_Ey[k][j] + index);
					SSE_CB = _mm256_loadu_ps(CB_Ey[k][j] + index);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					m0 = _mm256_sub_ps(SSE_D, SSE_SD);
					m2 = _mm256_mul_ps(SSE_idt, m0);
					m2 = _mm256_mul_ps(SSE_CB, m2);
					SSE_E = _mm256_add_ps(m1, m2);
					//Ey[k][j][i] = CA_Ey[k][j][i] * Ey[k][j][i] + CB_Ey[k][j][i] * idt*(Dy[k][j][i] - SDy);
					//Store
					_mm256_storeu_ps(fyx[k][j] + index_pml, SSE_k1);
					_mm256_storeu_ps(fyz[kk][j] + index, SSE_k2);
					_mm256_storeu_ps(Dy[k][j] + index, SSE_D);
					_mm256_storeu_ps(Ey[k][j] + index, SSE_E);

					//Ey 
					//SDy = Dy[k][j][i];
					//ADEPML(Hx[k][j][i], Hx[k - 1][j][i], Hz[k][j][i], Hz[k][j][i - 1], dso, dso, sigmaX, sigmaZ, fyx[k][j][ii], fyz[kk][j][i], Dy[k][j][i]);
					//Ey[k][j][i] = CA_Ey[k][j][i] * Ey[k][j][i] + CB_Ey[k][j][i] * idt*(Dy[k][j][i] - SDy);

					//Ez
					SSE_SD = _mm256_loadu_ps(Dz[k][j] + index);
					SSE_D = SSE_SD;
					SSE_H1H = _mm256_loadu_ps(Hy[k][j] + index);
					SSE_H1L = _mm256_loadu_ps(Hy[k][j] - 1 + index);
					SSE_d1 = _mm256_set1_ps(dy);
					SSE_H2H = _mm256_loadu_ps(Hx[k][j] + index);
					SSE_H2L = _mm256_loadu_ps(Hx[k][j - 1] + index);
					SSE_d2 = _mm256_set1_ps(dx);
					SSE_sigma1 = _mm256_set1_ps(sigmaY);
					SSE_sigma2 = SSE_sigmaX;
					SSE_k1 = _mm256_loadu_ps(fzy[k][jj] + index);
					SSE_k2 = _mm256_loadu_ps(fzx[k][j] + index_pml);
					ADEPML_AVX(SSE_H1H, SSE_H1L, SSE_H2H, SSE_H2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_D);
					//Dz = Dz[k][j][i];
					//ADEPML(Hy[k][j][i], Hy[k][j][i - 1], Hx[k][j][i], Hx[k][j - 1][i], dso, dso, sigmaY, sigmaX, fzy[k][jj][i], fzx[k][j][ii], Dz[k][j][i]);
					SSE_E = _mm256_loadu_ps(Ez[k][j] + index);
					SSE_CA = _mm256_loadu_ps(CA_Ez[k][j] + index);
					SSE_CB = _mm256_loadu_ps(CB_Ez[k][j] + index);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					m0 = _mm256_sub_ps(SSE_D, SSE_SD);
					m2 = _mm256_mul_ps(SSE_idt, m0);
					m2 = _mm256_mul_ps(SSE_CB, m2);
					SSE_E = _mm256_add_ps(m1, m2);
					//Ez[k][j][i] = CA_Ez[k][j][i] * Ez[k][j][i] + CB_Ez[k][j][i] * idt*(Dz[k][j][i] - SDz);
					//Store
					_mm256_storeu_ps(fzy[k][jj] + index, SSE_k1);
					_mm256_storeu_ps(fzx[k][j] + index_pml, SSE_k2);
					_mm256_storeu_ps(Dz[k][j] + index, SSE_D);
					_mm256_storeu_ps(Ez[k][j] + index, SSE_E);
					//Ez
					//Dz = Dz[k][j][i];
					//ADEPML(Hy[k][j][i], Hy[k][j][i - 1], Hx[k][j][i], Hx[k][j - 1][i], dso, dso, sigmaY, sigmaX, fzy[k][jj][i], fzx[k][j][ii], Dz[k][j][i]);
					//Ez[k][j][i] = CA_Ez[k][j][i] * Ez[k][j][i] + CB_Ez[k][j][i] * idt*(Dz[k][j][i] - SDz);
				}//i
			}//j
		}//k
		//cout << "set1" <<endl;
		//Set 2
		//X No PML //Z- Z+ PML // Y Full //待修改
		ks = Ns_pml[id];
		ke = Ns_pml[id] + Nn_pml[id] - 1;
		for (kk = ks; kk <= ke; kk++) {//Only PML
			if (kk <= num_pml) {//low
				k = kk;
				sigmaZ = sigmaHz[num_pml - k];//bigger
			}
			else {//up
				k = kk + (Nz - num_pml) - num_pml;
				sigmaZ = sigmaEz[k - Nz + num_pml - 1];
			}
			for (j = 1; j <= Ny; j++) { // Y Full
				sigmaY = 0.0;	jj = 0;
				if (j <= num_pml) { //Y-
					sigmaY = sigmaHy[num_pml - j];
					jj = j;
				}
				if (j >= Ny - num_pml + 1) { //Y+
					sigmaY = sigmaEy[j - Ny + num_pml - 1];
					jj = j - (Ny - num_pml) + num_pml;
				}
				for (i = 1 + num_pml; i < Nx_cal + num_pml; i += AVX_Count) {
					index = i;
					sigmaX = 0;	
					SSE_sigmaX = _mm256_set1_ps(sigmaX);
					//AVX Process
					//Set AVX input
					//Ex
					SSE_SD = _mm256_loadu_ps(Dx[k][j] + index);
					SSE_D = SSE_SD;
					SSE_H1H = _mm256_loadu_ps(Hz[k][j] + index);
					SSE_H1L = _mm256_loadu_ps(Hz[k][j - 1] + index);
					SSE_d1 = _mm256_set1_ps(dz);
					SSE_H2H = _mm256_loadu_ps(Hy[k][j] + index);
					SSE_H2L = _mm256_loadu_ps(Hy[k - 1][j] + index);
					SSE_d2 = _mm256_set1_ps(dy);
					SSE_sigma1 = _mm256_set1_ps(sigmaZ);
					SSE_sigma2 = _mm256_set1_ps(sigmaY);
					SSE_k1 = _mm256_loadu_ps(fxz[kk][j] + index);
					SSE_k2 = _mm256_loadu_ps(fxy[k][jj] + index);
					ADEPML_AVX(SSE_H1H, SSE_H1L, SSE_H2H, SSE_H2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_D);
					//SDx = Dx[k][j][i];
					//ADEPML(Hz[k][j][i], Hz[k][j - 1][i], Hy[k][j][i], Hy[k - 1][j][i], dso, dso, sigmaZ, sigmaY, fxz[kk][j][i], fxy[k][jj][i], Dx[k][j][i]);
					SSE_E = _mm256_loadu_ps(Ex[k][j] + index);
					SSE_CA = _mm256_loadu_ps(CA_Ex[k][j] + index);
					SSE_CB = _mm256_loadu_ps(CB_Ex[k][j] + index);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					m0 = _mm256_sub_ps(SSE_D, SSE_SD);
					m2 = _mm256_mul_ps(SSE_idt, m0);
					m2 = _mm256_mul_ps(SSE_CB, m2);
					SSE_E = _mm256_add_ps(m1, m2);
					//Ex[k][j][i] = CA_Ex[k][j][i] * Ex[k][j][i] + CB_Ex[k][j][i] * idt*(Dx[k][j][i] - SDx);
					//Store
					_mm256_storeu_ps(fxz[kk][j] + index, SSE_k1);
					_mm256_storeu_ps(fxy[k][jj] + index, SSE_k2);
					_mm256_storeu_ps(Dx[k][j] + index, SSE_D);
					_mm256_storeu_ps(Ex[k][j] + index, SSE_E);

					//SSE_E = _mm256_loadu_ps(Ex[k][j] + i);
					//Ex
					//SDx = Dx[k][j][i];
					//ADEPML(Hz[k][j][i], Hz[k][j - 1][i], Hy[k][j][i], Hy[k - 1][j][i], dso, dso, sigmaZ, sigmaY, fxz[kk][j][i], fxy[k][jj][i], Dx[k][j][i]);
					//Ex[k][j][i] = CA_Ex[k][j][i] * Ex[k][j][i] + CB_Ex[k][j][i] * idt*(Dx[k][j][i] - SDx);

					//Ey
					SSE_SD = _mm256_loadu_ps(Dy[k][j] + index);
					SSE_D = SSE_SD;
					SSE_H1H = _mm256_loadu_ps(Hx[k][j] + index);
					SSE_H1L = _mm256_loadu_ps(Hx[k - 1][j] + index);
					SSE_d1 = _mm256_set1_ps(dx);
					SSE_H2H = _mm256_loadu_ps(Hz[k][j] + index);
					SSE_H2L = _mm256_loadu_ps(Hz[k][j] - 1 + index);
					SSE_d2 = _mm256_set1_ps(dz);
					SSE_sigma1 = SSE_sigmaX;
					SSE_sigma2 = _mm256_set1_ps(sigmaZ);
					SSE_k1 = _mm256_set1_ps(0);
					SSE_k2 = _mm256_loadu_ps(fyz[kk][j] + index);
					ADEPML_AVX(SSE_H1H, SSE_H1L, SSE_H2H, SSE_H2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_D);
					//SDx = Dx[k][j][i];
					//ADEPML(Hx[k][j][i], Hx[k-1][j][i], Hz[k][j][i], Hz[k][j][i-1], dso, dso, sigmaX, sigmaZ, fyx[k][j][ii], fyz[kk][j][i], Dy[k][j][i]);
					SSE_E = _mm256_loadu_ps(Ey[k][j] + index);
					SSE_CA = _mm256_loadu_ps(CA_Ey[k][j] + index);
					SSE_CB = _mm256_loadu_ps(CB_Ey[k][j] + index);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					m0 = _mm256_sub_ps(SSE_D, SSE_SD);
					m2 = _mm256_mul_ps(SSE_idt, m0);
					m2 = _mm256_mul_ps(SSE_CB, m2);
					SSE_E = _mm256_add_ps(m1, m2);
					//Ey[k][j][i] = CA_Ey[k][j][i] * Ey[k][j][i] + CB_Ey[k][j][i] * idt*(Dy[k][j][i] - SDy);
					//Store
					//_mm256_storeu_ps(fyx[k][j] + index_pml, SSE_k1);
					_mm256_storeu_ps(fyz[kk][j] + index, SSE_k2);
					_mm256_storeu_ps(Dy[k][j] + index, SSE_D);
					_mm256_storeu_ps(Ey[k][j] + index, SSE_E);

					//Ey 
					//SDy = Dy[k][j][i];
					//ADEPML(Hx[k][j][i], Hx[k - 1][j][i], Hz[k][j][i], Hz[k][j][i - 1], dso, dso, sigmaX, sigmaZ, fyx[k][j][ii], fyz[kk][j][i], Dy[k][j][i]);
					//Ey[k][j][i] = CA_Ey[k][j][i] * Ey[k][j][i] + CB_Ey[k][j][i] * idt*(Dy[k][j][i] - SDy);

					//Ez
					SSE_SD = _mm256_loadu_ps(Dz[k][j] + index);
					SSE_D = SSE_SD;
					SSE_H1H = _mm256_loadu_ps(Hy[k][j] + index);
					SSE_H1L = _mm256_loadu_ps(Hy[k][j] - 1 + index);
					SSE_d1 = _mm256_set1_ps(dy);
					SSE_H2H = _mm256_loadu_ps(Hx[k][j] + index);
					SSE_H2L = _mm256_loadu_ps(Hx[k][j - 1] + index);
					SSE_d2 = _mm256_set1_ps(dx);
					SSE_sigma1 = _mm256_set1_ps(sigmaY);
					SSE_sigma2 = SSE_sigmaX;
					SSE_k1 = _mm256_loadu_ps(fzy[k][jj] + index);
					SSE_k2 = _mm256_set1_ps(0);
					ADEPML_AVX(SSE_H1H, SSE_H1L, SSE_H2H, SSE_H2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_D);
					//Dz = Dz[k][j][i];
					//ADEPML(Hy[k][j][i], Hy[k][j][i - 1], Hx[k][j][i], Hx[k][j - 1][i], dso, dso, sigmaY, sigmaX, fzy[k][jj][i], fzx[k][j][ii], Dz[k][j][i]);
					SSE_E = _mm256_loadu_ps(Ez[k][j] + index);
					SSE_CA = _mm256_loadu_ps(CA_Ez[k][j] + index);
					SSE_CB = _mm256_loadu_ps(CB_Ez[k][j] + index);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					m0 = _mm256_sub_ps(SSE_D, SSE_SD);
					m2 = _mm256_mul_ps(SSE_idt, m0);
					m2 = _mm256_mul_ps(SSE_CB, m2);
					SSE_E = _mm256_add_ps(m1, m2);
					//Ez[k][j][i] = CA_Ez[k][j][i] * Ez[k][j][i] + CB_Ez[k][j][i] * idt*(Dz[k][j][i] - SDz);
					//Store
					_mm256_storeu_ps(fzy[k][jj] + index, SSE_k1);
					//_mm256_storeu_ps(fzx[k][j] + index_pml, SSE_k2);
					_mm256_storeu_ps(Dz[k][j] + index, SSE_D);
					_mm256_storeu_ps(Ez[k][j] + index, SSE_E);
					//Ez
					//Dz = Dz[k][j][i];
					//ADEPML(Hy[k][j][i], Hy[k][j][i - 1], Hx[k][j][i], Hx[k][j - 1][i], dso, dso, sigmaY, sigmaX, fzy[k][jj][i], fzx[k][j][ii], Dz[k][j][i]);
					//Ez[k][j][i] = CA_Ez[k][j][i] * Ez[k][j][i] + CB_Ez[k][j][i] * idt*(Dz[k][j][i] - SDz);

				}//i - no pml
			}//j - full
		}//kk only PML
		//cout << "set2" << endl;
		
		//Set 3
		//X No PML //Z No PML // Y- Y+ PML
		ks = Nzs_cal[id];
		ke = Nzs_cal[id] + Nzn_cal[id] - 1;
		for (k = ks + num_pml; k <= ke + num_pml; k++) {
			sigmaZ = 0.0;	kk = 0;
			for (jj = 1; jj <= num_pml * 2; jj++) {
				if (jj <= num_pml) {//low
					j = jj;
					sigmaY = sigmaHy[num_pml - j];//bigger
				}
				else {//up
					j = jj + (Ny - num_pml) - num_pml;
					sigmaY = sigmaEy[j - Ny + num_pml - 1];
				}
				for (i = 1 + num_pml; i < Nx_cal + num_pml; i += AVX_Count) {
					index = i;
					sigmaX = 0;
					SSE_sigmaX = _mm256_set1_ps(sigmaX);
					//AVX Process
					//Set AVX input
					//Ex
					SSE_SD = _mm256_loadu_ps(Dx[k][j] + index);
					SSE_D = SSE_SD;
					SSE_H1H = _mm256_loadu_ps(Hz[k][j] + index);
					SSE_H1L = _mm256_loadu_ps(Hz[k][j - 1] + index);
					SSE_d1 = _mm256_set1_ps(dz);
					SSE_H2H = _mm256_loadu_ps(Hy[k][j] + index);
					SSE_H2L = _mm256_loadu_ps(Hy[k - 1][j] + index);
					SSE_d2 = _mm256_set1_ps(dy);
					SSE_sigma1 = _mm256_set1_ps(sigmaZ);
					SSE_sigma2 = _mm256_set1_ps(sigmaY);
					SSE_k1 = _mm256_loadu_ps(fxz[kk][j] + index);
					SSE_k2 = _mm256_loadu_ps(fxy[k][jj] + index);
					ADEPML_AVX(SSE_H1H, SSE_H1L, SSE_H2H, SSE_H2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_D);
					//SDx = Dx[k][j][i];
					//ADEPML(Hz[k][j][i], Hz[k][j - 1][i], Hy[k][j][i], Hy[k - 1][j][i], dso, dso, sigmaZ, sigmaY, fxz[kk][j][i], fxy[k][jj][i], Dx[k][j][i]);
					SSE_E = _mm256_loadu_ps(Ex[k][j] + index);
					SSE_CA = _mm256_loadu_ps(CA_Ex[k][j] + index);
					SSE_CB = _mm256_loadu_ps(CB_Ex[k][j] + index);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					m0 = _mm256_sub_ps(SSE_D, SSE_SD);
					m2 = _mm256_mul_ps(SSE_idt, m0);
					m2 = _mm256_mul_ps(SSE_CB, m2);
					SSE_E = _mm256_add_ps(m1, m2);
					//Ex[k][j][i] = CA_Ex[k][j][i] * Ex[k][j][i] + CB_Ex[k][j][i] * idt*(Dx[k][j][i] - SDx);
					//Store
					_mm256_storeu_ps(fxz[kk][j] + index, SSE_k1);
					_mm256_storeu_ps(fxy[k][jj] + index, SSE_k2);
					_mm256_storeu_ps(Dx[k][j] + index, SSE_D);
					_mm256_storeu_ps(Ex[k][j] + index, SSE_E);

					//SSE_E = _mm256_loadu_ps(Ex[k][j] + i);
					//Ex
					//SDx = Dx[k][j][i];
					//ADEPML(Hz[k][j][i], Hz[k][j - 1][i], Hy[k][j][i], Hy[k - 1][j][i], dso, dso, sigmaZ, sigmaY, fxz[kk][j][i], fxy[k][jj][i], Dx[k][j][i]);
					//Ex[k][j][i] = CA_Ex[k][j][i] * Ex[k][j][i] + CB_Ex[k][j][i] * idt*(Dx[k][j][i] - SDx);

					//Ey
					SSE_SD = _mm256_loadu_ps(Dy[k][j] + index);
					SSE_D = SSE_SD;
					SSE_H1H = _mm256_loadu_ps(Hx[k][j] + index);
					SSE_H1L = _mm256_loadu_ps(Hx[k - 1][j] + index);
					SSE_d1 = _mm256_set1_ps(dx);
					SSE_H2H = _mm256_loadu_ps(Hz[k][j] + index);
					SSE_H2L = _mm256_loadu_ps(Hz[k][j] - 1 + index);
					SSE_d2 = _mm256_set1_ps(dz);
					SSE_sigma1 = SSE_sigmaX;
					SSE_sigma2 = _mm256_set1_ps(sigmaZ);
					SSE_k1 = _mm256_set1_ps(0);
					SSE_k2 = _mm256_loadu_ps(fyz[kk][j] + index);
					ADEPML_AVX(SSE_H1H, SSE_H1L, SSE_H2H, SSE_H2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_D);
					//SDx = Dx[k][j][i];
					//ADEPML(Hx[k][j][i], Hx[k-1][j][i], Hz[k][j][i], Hz[k][j][i-1], dso, dso, sigmaX, sigmaZ, fyx[k][j][ii], fyz[kk][j][i], Dy[k][j][i]);
					SSE_E = _mm256_loadu_ps(Ey[k][j] + index);
					SSE_CA = _mm256_loadu_ps(CA_Ey[k][j] + index);
					SSE_CB = _mm256_loadu_ps(CB_Ey[k][j] + index);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					m0 = _mm256_sub_ps(SSE_D, SSE_SD);
					m2 = _mm256_mul_ps(SSE_idt, m0);
					m2 = _mm256_mul_ps(SSE_CB, m2);
					SSE_E = _mm256_add_ps(m1, m2);
					//Ey[k][j][i] = CA_Ey[k][j][i] * Ey[k][j][i] + CB_Ey[k][j][i] * idt*(Dy[k][j][i] - SDy);
					//Store
					//_mm256_storeu_ps(fyx[k][j] + index_pml, SSE_k1);
					_mm256_storeu_ps(fyz[kk][j] + index, SSE_k2);
					_mm256_storeu_ps(Dy[k][j] + index, SSE_D);
					_mm256_storeu_ps(Ey[k][j] + index, SSE_E);

					//Ey 
					//SDy = Dy[k][j][i];
					//ADEPML(Hx[k][j][i], Hx[k - 1][j][i], Hz[k][j][i], Hz[k][j][i - 1], dso, dso, sigmaX, sigmaZ, fyx[k][j][ii], fyz[kk][j][i], Dy[k][j][i]);
					//Ey[k][j][i] = CA_Ey[k][j][i] * Ey[k][j][i] + CB_Ey[k][j][i] * idt*(Dy[k][j][i] - SDy);

					//Ez
					SSE_SD = _mm256_loadu_ps(Dz[k][j] + index);
					SSE_D = SSE_SD;
					SSE_H1H = _mm256_loadu_ps(Hy[k][j] + index);
					SSE_H1L = _mm256_loadu_ps(Hy[k][j] - 1 + index);
					SSE_d1 = _mm256_set1_ps(dy);
					SSE_H2H = _mm256_loadu_ps(Hx[k][j] + index);
					SSE_H2L = _mm256_loadu_ps(Hx[k][j - 1] + index);
					SSE_d2 = _mm256_set1_ps(dx);
					SSE_sigma1 = _mm256_set1_ps(sigmaY);
					SSE_sigma2 = SSE_sigmaX;
					SSE_k1 = _mm256_loadu_ps(fzy[k][jj] + index);
					SSE_k2 = _mm256_set1_ps(0);
					ADEPML_AVX(SSE_H1H, SSE_H1L, SSE_H2H, SSE_H2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_D);
					//Dz = Dz[k][j][i];
					//ADEPML(Hy[k][j][i], Hy[k][j][i - 1], Hx[k][j][i], Hx[k][j - 1][i], dso, dso, sigmaY, sigmaX, fzy[k][jj][i], fzx[k][j][ii], Dz[k][j][i]);
					SSE_E = _mm256_loadu_ps(Ez[k][j] + index);
					SSE_CA = _mm256_loadu_ps(CA_Ez[k][j] + index);
					SSE_CB = _mm256_loadu_ps(CB_Ez[k][j] + index);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					m0 = _mm256_sub_ps(SSE_D, SSE_SD);
					m2 = _mm256_mul_ps(SSE_idt, m0);
					m2 = _mm256_mul_ps(SSE_CB, m2);
					SSE_E = _mm256_add_ps(m1, m2);
					//Ez[k][j][i] = CA_Ez[k][j][i] * Ez[k][j][i] + CB_Ez[k][j][i] * idt*(Dz[k][j][i] - SDz);
					//Store
					_mm256_storeu_ps(fzy[k][jj] + index, SSE_k1);
					//_mm256_storeu_ps(fzx[k][j] + index_pml, SSE_k2);
					_mm256_storeu_ps(Dz[k][j] + index, SSE_D);
					_mm256_storeu_ps(Ez[k][j] + index, SSE_E);
					//Ez
					//Dz = Dz[k][j][i];
					//ADEPML(Hy[k][j][i], Hy[k][j][i - 1], Hx[k][j][i], Hx[k][j - 1][i], dso, dso, sigmaY, sigmaX, fzy[k][jj][i], fzx[k][j][ii], Dz[k][j][i]);
					//Ez[k][j][i] = CA_Ez[k][j][i] * Ez[k][j][i] + CB_Ez[k][j][i] * idt*(Dz[k][j][i] - SDz);

				}//i - no pml
			}//jj - only pml
		}//k - no pml
		//cout << "set3" << endl;
	}//omp
}
void CFDTD::BoundaryH_AVX() {
	omp_set_num_threads(threadNum);
	#pragma omp parallel
	{
		int id = omp_get_thread_num();

		//iteration
		int i, j, k;			int ii, jj, kk;			register int i_AVX;
		//for pml
		float sigmaX = 0.0;	float sigmaY = 0.0;	float sigmaZ = 0.0;
		float SHx, SHy, SHz;
		float gxy0 = 0;	float gxz0 = 0;
		float gyz0 = 0;	float gyx0 = 0;
		float gzx0 = 0;	float gzy0 = 0;
		float Muo = Mu0f;
		int ks, ke;
		int index, index_pml;

		//AVX_dataset
		register __m256 SSE_d1;// = _mm256_set1_ps(ds);
		register __m256 SSE_d2;
		register __m256 SSE_dt = _mm256_set1_ps(dt);
		register __m256 SSE_M = _mm256_set1_ps(Mu0f);
		register __m256 SSE_IM = _mm256_set1_ps(Mu0_Invf);
		register __m256 SSE_H;
		register __m256 SSE_B;
		register __m256 SSE_E1H;
		register __m256 SSE_E1L;
		register __m256 SSE_E2H;
		register __m256 SSE_E2L;
		register __m256 SSE_sigma1;
		register __m256 SSE_sigma2;
		register __m256 SSE_sigmaX;
		register __m256 SSE_k1;
		register __m256 SSE_k2;
		register __m256 m0;
		register __m256 m1;
		register __m256 m2;

		//Set1
		//X+ X- Pml //Z Full //Y Full
		ks = Nzs[id];
		ke = Nzs[id] + Nzn[id] - 1;
		for (k = ks; k <= ke; k++) {
			sigmaZ = 0.0;	kk = 0;
			if (k <= num_pml) { //Z-
				sigmaZ = sigmaEz[num_pml - k];
				kk = k;
			}
			if (k >= Nz - num_pml + 1) { //Z+
				sigmaZ = sigmaHz[k - Nz + num_pml - 1];
				kk = k - (Nz - num_pml) + num_pml;
			}
			for (j = 1; j <= Ny; j++) {
				sigmaY = 0.0;	jj = 0;
				if (j <= num_pml) { //Y-
					sigmaY = sigmaEy[num_pml - j];
					jj = j;
				}
				if (j >= Ny - num_pml + 1) { //Y+
					sigmaY = sigmaHy[j - Ny + num_pml - 1];
					jj = j - (Ny - num_pml) + num_pml;
				}
				//i_AVX_pml 
				for (i_AVX = 0; i_AVX <= 1; i_AVX++) {//Address
					//i_AVX=0:index = 1; i_AVX = 1: index = Nx-num_pml+1
					index = 1 + (Nx - num_pml) * i_AVX;
					//i_AVX=0:index_pml = 1; i_AVX = 1: index_pml = 1+num_pml
					index_pml = 1 + num_pml * i_AVX;
					if (i_AVX == 0) {
						//-i 正常顺序是前面index大，后面index小
						//-i 顺序是前面index小，后面index大
						SSE_sigmaX = _mm256_set_ps(sigmaEx[0], sigmaEx[1], sigmaEx[2], sigmaEx[3], sigmaEx[4], sigmaEx[5], sigmaEx[6], sigmaEx[7]);
						//sse_Ez_max = _mm256_set_ps(Ez[k][j][i_sse], Ez[k][j][i_sse - 1], Ez[k][j][i_sse - 2], Ez[k][j][i_sse - 3], Ez[k][j][i_sse - 4], Ez[k][j][i_sse - 5], Ez[k][j][i_sse - 6], Ez[k][j][i_sse - 7]);
					}
					else {
						//i 正常顺序是前面index大，后面index小
						SSE_sigmaX = _mm256_set_ps(sigmaHx[7], sigmaHx[6], sigmaHx[5], sigmaHx[4], sigmaHx[3], sigmaHx[2], sigmaHx[1], sigmaHx[0]);
					}
					//Set AVX input
					//Hx
					SSE_H = _mm256_loadu_ps(Hx[k][j] + index);
					SSE_B = _mm256_mul_ps(SSE_H,SSE_M);
					//SHx = Hx[k][j][i] * Mu0;
					SSE_E1H = _mm256_loadu_ps(Ey[k+1][j] + index);
					SSE_E1L = _mm256_loadu_ps(Ey[k][j] + index);
					SSE_d1 = _mm256_set1_ps(dy);
					SSE_E2H = _mm256_loadu_ps(Ez[k][j+1] + index);
					SSE_E2L = _mm256_loadu_ps(Ez[k][j] + index);
					SSE_d2 = _mm256_set1_ps(dz);
					SSE_sigma1 = _mm256_set1_ps(sigmaY);
					SSE_sigma2 = _mm256_set1_ps(sigmaZ);
					SSE_k1 = _mm256_loadu_ps(gxy[k][jj] + index);
					SSE_k2 = _mm256_loadu_ps(gxz[kk][j] + index);
					ADEPML_AVX(SSE_E1H, SSE_E1L, SSE_E2H, SSE_E2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_B);
					//ADEPML(Ey[k + 1][j][i], Ey[k][j][i], Ez[k][j + 1][i], Ez[k][j][i], dso, dso, sigmaY, sigmaZ, gxy[k][jj][i], gxz[kk][j][i], SHx);
					SSE_H = _mm256_mul_ps(SSE_B, SSE_IM);
					//Hx[k][j][i] = SHx * Mu0_Inv;
					//Store
					_mm256_storeu_ps(gxy[k][jj] + index, SSE_k1);
					_mm256_storeu_ps(gxz[kk][j] + index, SSE_k2);
					_mm256_storeu_ps(Hx[k][j] + index, SSE_H);
					//Hx
					//SHx = Hx[k][j][i] * Mu0;
					//ADEPML(Ey[k + 1][j][i], Ey[k][j][i], Ez[k][j + 1][i], Ez[k][j][i], dso, dso, sigmaY, sigmaZ, gxy[k][jj][i], gxz[kk][j][i], SHx);
					//Hx[k][j][i] = SHx * Mu0_Inv;

					//Hy
					SSE_H = _mm256_loadu_ps(Hy[k][j] + index);
					SSE_B = _mm256_mul_ps(SSE_H,SSE_M);
					//SHy = Hy[k][j][i] * Mu0;
					SSE_E1H = _mm256_loadu_ps(Ez[k][j] + 1 + index);
					SSE_E1L = _mm256_loadu_ps(Ez[k][j] + index);
					SSE_d1 = _mm256_set1_ps(dz);
					SSE_E2H = _mm256_loadu_ps(Ex[k + 1][j] + index);
					SSE_E2L = _mm256_loadu_ps(Ex[k][j] + index);
					SSE_d2 = _mm256_set1_ps(dx);
					SSE_sigma1 = _mm256_set1_ps(sigmaZ);
					SSE_sigma2 = SSE_sigmaX;
					SSE_k1 = _mm256_loadu_ps(gyz[kk][j] + index);
					SSE_k2 = _mm256_loadu_ps(gyx[k][j] + index_pml);
					ADEPML_AVX(SSE_E1H, SSE_E1L, SSE_E2H, SSE_E2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_B);
					//ADEPML(Ez[k][j][i + 1], Ez[k][j][i], Ex[k + 1][j][i], Ex[k][j][i], dso, dso, sigmaZ, sigmaX, gyz[kk][j][i], gyx[k][j][ii], SHy);
					SSE_H = _mm256_mul_ps(SSE_B, SSE_IM);
					//Hy[k][j][i] = SHy * Mu0_Inv;
					//Store
					_mm256_storeu_ps(gyz[kk][j] + index, SSE_k1);
					_mm256_storeu_ps(gyx[k][j] + index_pml, SSE_k2);
					_mm256_storeu_ps(Hy[k][j] + index, SSE_H);
					//Hy 
					//SHy = Hy[k][j][i] * Mu0;
					//ADEPML(Ez[k][j][i + 1], Ez[k][j][i], Ex[k + 1][j][i], Ex[k][j][i], dso, dso, sigmaZ, sigmaX, gyz[kk][j][i], gyx[k][j][ii], SHy);
					//Hy[k][j][i] = SHy * Mu0_Inv;

					//Hz
					SSE_H = _mm256_loadu_ps(Hz[k][j] + index);
					SSE_B = _mm256_mul_ps(SSE_H,SSE_M);
					//SHz = Hz[k][j][i] * Mu0;
					SSE_E1H = _mm256_loadu_ps(Ex[k][j + 1] + index);
					SSE_E1L = _mm256_loadu_ps(Ex[k][j] + index);
					SSE_d1 = _mm256_set1_ps(dx);
					SSE_E2H = _mm256_loadu_ps(Ey[k][j] + 1 + index);
					SSE_E2L = _mm256_loadu_ps(Ey[k][j] + index);
					SSE_d2 = _mm256_set1_ps(dy);
					SSE_sigma1 = SSE_sigmaX;
					SSE_sigma2 = _mm256_set1_ps(sigmaY);
					SSE_k1 = _mm256_loadu_ps(gzx[k][j] + index_pml);
					SSE_k2 = _mm256_loadu_ps(gzy[k][jj] + index);
					ADEPML_AVX(SSE_E1H, SSE_E1L, SSE_E2H, SSE_E2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_B);
					//ADEPML(Ex[k][j + 1][i], Ex[k][j][i], Ey[k][j][i + 1], Ey[k][j][i], dso, dso, sigmaX, sigmaY, gzx[k][j][ii], gzy[k][jj][i], SHz);
					SSE_H = _mm256_mul_ps(SSE_B,SSE_IM);
					//Hz[k][j][i] = SHz * Mu0_Inv;
					//Store
					_mm256_storeu_ps(gzx[k][j] + index_pml, SSE_k1);
					_mm256_storeu_ps(gzy[k][jj] + index, SSE_k2);
					_mm256_storeu_ps(Hz[k][j] + index, SSE_H);
					//Hz
					//SHz = Hz[k][j][i] * Mu0;
					//ADEPML(Ex[k][j + 1][i], Ex[k][j][i], Ey[k][j][i + 1], Ey[k][j][i], dso, dso, sigmaX, sigmaY, gzx[k][j][ii], gzy[k][jj][i], SHz);
					//Hz[k][j][i] = SHz * Mu0_Inv;
				}//i AVX_pml
			}//j full
		}//k full

		//Set 2
		//X No PML //Z- Z+ PML // Y Full //待修改
		ks = Ns_pml[id];
		ke = Ns_pml[id] + Nn_pml[id] - 1;
		for (kk = ks; kk <= ke; kk++) {//Only PML
			if (kk <= num_pml) {//low
				k = kk;
				sigmaZ = sigmaEz[num_pml - k];//bigger
			}
			else {//up
				k = kk + (Nz - num_pml) - num_pml;
				sigmaZ = sigmaHz[k - Nz + num_pml - 1];
			}
			for (j = 1; j <= Ny; j++) { // Y Full
				sigmaY = 0.0;	jj = 0;
				if (j <= num_pml) { //Y-
					sigmaY = sigmaEy[num_pml - j];
					jj = j;
				}
				if (j >= Ny - num_pml + 1) { //Y+
					sigmaY = sigmaHy[j - Ny + num_pml - 1];
					jj = j - (Ny - num_pml) + num_pml;
				}
				for (i = 1 + num_pml; i < Nx_cal + num_pml; i += AVX_Count) {//No PML
					sigmaX = 0; SSE_sigmaX = _mm256_set1_ps(sigmaX);
					//i_AVX=0:index = 0; i_AVX = 1: index = Nx-num_pml //Should be adjusted
					index = i;			
					//Set AVX input
					//Hx
					SSE_H = _mm256_loadu_ps(Hx[k][j] + index);
					SSE_B = _mm256_mul_ps(SSE_H, SSE_M);
					//SHx = Hx[k][j][i] * Mu0;
					SSE_E1H = _mm256_loadu_ps(Ey[k + 1][j] + index);
					SSE_E1L = _mm256_loadu_ps(Ey[k][j] + index);
					SSE_d1 = _mm256_set1_ps(dy);
					SSE_E2H = _mm256_loadu_ps(Ez[k][j + 1] + index);
					SSE_E2L = _mm256_loadu_ps(Ez[k][j] + index);
					SSE_d2 = _mm256_set1_ps(dz);
					SSE_sigma1 = _mm256_set1_ps(sigmaY);
					SSE_sigma2 = _mm256_set1_ps(sigmaZ);
					SSE_k1 = _mm256_loadu_ps(gxy[k][jj] + index);
					SSE_k2 = _mm256_loadu_ps(gxz[kk][j] + index);
					ADEPML_AVX(SSE_E1H, SSE_E1L, SSE_E2H, SSE_E2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_B);
					//ADEPML(Ey[k + 1][j][i], Ey[k][j][i], Ez[k][j + 1][i], Ez[k][j][i], dso, dso, sigmaY, sigmaZ, gxy[k][jj][i], gxz[kk][j][i], SHx);
					SSE_H = _mm256_mul_ps(SSE_B, SSE_IM);
					//Hx[k][j][i] = SHx * Mu0_Inv;
					//Store
					_mm256_storeu_ps(gxy[k][jj] + index, SSE_k1);
					_mm256_storeu_ps(gxz[kk][j] + index, SSE_k2);
					_mm256_storeu_ps(Hx[k][j] + index, SSE_H);
					//Hx
					//SHx = Hx[k][j][i] * Mu0;
					//ADEPML(Ey[k + 1][j][i], Ey[k][j][i], Ez[k][j + 1][i], Ez[k][j][i], dso, dso, sigmaY, sigmaZ, gxy[k][jj][i], gxz[kk][j][i], SHx);
					//Hx[k][j][i] = SHx * Mu0_Inv;

					//Hy
					SSE_H = _mm256_loadu_ps(Hy[k][j] + index);
					SSE_B = _mm256_mul_ps(SSE_H, SSE_M);
					//SHy = Hy[k][j][i] * Mu0;
					SSE_E1H = _mm256_loadu_ps(Ez[k][j] + 1 + index);
					SSE_E1L = _mm256_loadu_ps(Ez[k][j] + index);
					SSE_d1 = _mm256_set1_ps(dz);
					SSE_E2H = _mm256_loadu_ps(Ex[k + 1][j] + index);
					SSE_E2L = _mm256_loadu_ps(Ex[k][j] + index);
					SSE_d2 = _mm256_set1_ps(dx);
					SSE_sigma1 = _mm256_set1_ps(sigmaZ);
					SSE_sigma2 = SSE_sigmaX;
					SSE_k1 = _mm256_loadu_ps(gyz[kk][j] + index);
					SSE_k2 = _mm256_set1_ps(0);
					ADEPML_AVX(SSE_E1H, SSE_E1L, SSE_E2H, SSE_E2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_B);
					//ADEPML(Ez[k][j][i + 1], Ez[k][j][i], Ex[k + 1][j][i], Ex[k][j][i], dso, dso, sigmaZ, sigmaX, gyz[kk][j][i], gyx[k][j][ii], SHy);
					SSE_H = _mm256_mul_ps(SSE_B, SSE_IM);
					//Hy[k][j][i] = SHy * Mu0_Inv;
					//Store
					_mm256_storeu_ps(gyz[kk][j] + index, SSE_k1);
					//_mm256_storeu_ps(gyx[k][j] + index_pml, SSE_k2);
					_mm256_storeu_ps(Hy[k][j] + index, SSE_H);
					//Hy 
					//SHy = Hy[k][j][i] * Mu0;
					//ADEPML(Ez[k][j][i + 1], Ez[k][j][i], Ex[k + 1][j][i], Ex[k][j][i], dso, dso, sigmaZ, sigmaX, gyz[kk][j][i], gyx[k][j][ii], SHy);
					//Hy[k][j][i] = SHy * Mu0_Inv;

					//Hz
					SSE_H = _mm256_loadu_ps(Hz[k][j] + index);
					SSE_B = _mm256_mul_ps(SSE_H, SSE_M);
					//SHz = Hz[k][j][i] * Mu0;
					SSE_E1H = _mm256_loadu_ps(Ex[k][j + 1] + index);
					SSE_E1L = _mm256_loadu_ps(Ex[k][j] + index);
					SSE_d1 = _mm256_set1_ps(dx);
					SSE_E2H = _mm256_loadu_ps(Ey[k][j] + 1 + index);
					SSE_E2L = _mm256_loadu_ps(Ey[k][j] + index);
					SSE_d2 = _mm256_set1_ps(dy);
					SSE_sigma1 = SSE_sigmaX;
					SSE_sigma2 = _mm256_set1_ps(sigmaY);
					SSE_k1 = _mm256_set1_ps(0);
					SSE_k2 = _mm256_loadu_ps(gzy[k][jj] + index);
					ADEPML_AVX(SSE_E1H, SSE_E1L, SSE_E2H, SSE_E2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_B);
					//ADEPML(Ex[k][j + 1][i], Ex[k][j][i], Ey[k][j][i + 1], Ey[k][j][i], dso, dso, sigmaX, sigmaY, gzx[k][j][ii], gzy[k][jj][i], SHz);
					SSE_H = _mm256_mul_ps(SSE_B, SSE_IM);
					//Hz[k][j][i] = SHz * Mu0_Inv;
					//Store
					//_mm256_storeu_ps(gzx[k][j] + index_pml, SSE_k1);
					_mm256_storeu_ps(gzy[k][jj] + index, SSE_k2);
					_mm256_storeu_ps(Hz[k][j] + index, SSE_H);
					//Hz
					//SHz = Hz[k][j][i] * Mu0;
					//ADEPML(Ex[k][j + 1][i], Ex[k][j][i], Ey[k][j][i + 1], Ey[k][j][i], dso, dso, sigmaX, sigmaY, gzx[k][j][ii], gzy[k][jj][i], SHz);
					//Hz[k][j][i] = SHz * Mu0_Inv;

				}//X No PML
			}//Y Full
		}//Z+ Z- PML

		//Set 3
		//X No PML //Z No PML // Y- Y+ PML
		ks = Nzs_cal[id];
		ke = Nzs_cal[id] + Nzn_cal[id] - 1;
		for (k = ks + num_pml; k <= ke + num_pml; k++) {
			sigmaZ = 0.0;
			kk = 0;
			for (jj = 1; jj <= num_pml * 2; jj++) {
				if (jj <= num_pml) {//low
					j = jj;
					sigmaY = sigmaEy[num_pml - j];//bigger
				}
				else {//up
					j = jj + (Ny - num_pml) - num_pml;
					sigmaY = sigmaHy[j - Ny + num_pml - 1];
				}
				for (i = 1 + num_pml; i < Nx_cal + num_pml; i += AVX_Count) {
					sigmaX = 0; SSE_sigmaX = _mm256_set1_ps(sigmaX);
					//i_AVX=0:index = 0; i_AVX = 1: index = Nx-num_pml //Should be adjusted
					index = i;
					//Set AVX input
					//Hx
					SSE_H = _mm256_loadu_ps(Hx[k][j] + index);
					SSE_B = _mm256_mul_ps(SSE_H, SSE_M);
					//SHx = Hx[k][j][i] * Mu0;
					SSE_E1H = _mm256_loadu_ps(Ey[k + 1][j] + index);
					SSE_E1L = _mm256_loadu_ps(Ey[k][j] + index);
					SSE_d1 = _mm256_set1_ps(dy);
					SSE_E2H = _mm256_loadu_ps(Ez[k][j + 1] + index);
					SSE_E2L = _mm256_loadu_ps(Ez[k][j] + index);
					SSE_d2 = _mm256_set1_ps(dz);
					SSE_sigma1 = _mm256_set1_ps(sigmaY);
					SSE_sigma2 = _mm256_set1_ps(sigmaZ);
					SSE_k1 = _mm256_loadu_ps(gxy[k][jj] + index);
					SSE_k2 = _mm256_loadu_ps(gxz[kk][j] + index);
					ADEPML_AVX(SSE_E1H, SSE_E1L, SSE_E2H, SSE_E2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_B);
					//ADEPML(Ey[k + 1][j][i], Ey[k][j][i], Ez[k][j + 1][i], Ez[k][j][i], dso, dso, sigmaY, sigmaZ, gxy[k][jj][i], gxz[kk][j][i], SHx);
					SSE_H = _mm256_mul_ps(SSE_B, SSE_IM);
					//Hx[k][j][i] = SHx * Mu0_Inv;
					//Store
					_mm256_storeu_ps(gxy[k][jj] + index, SSE_k1);
					_mm256_storeu_ps(gxz[kk][j] + index, SSE_k2);
					_mm256_storeu_ps(Hx[k][j] + index, SSE_H);
					//Hx
					//SHx = Hx[k][j][i] * Mu0;
					//ADEPML(Ey[k + 1][j][i], Ey[k][j][i], Ez[k][j + 1][i], Ez[k][j][i], dso, dso, sigmaY, sigmaZ, gxy[k][jj][i], gxz[kk][j][i], SHx);
					//Hx[k][j][i] = SHx * Mu0_Inv;

					//Hy
					SSE_H = _mm256_loadu_ps(Hy[k][j] + index);
					SSE_B = _mm256_mul_ps(SSE_H, SSE_M);
					//SHy = Hy[k][j][i] * Mu0;
					SSE_E1H = _mm256_loadu_ps(Ez[k][j] + 1 + index);
					SSE_E1L = _mm256_loadu_ps(Ez[k][j] + index);
					SSE_d1 = _mm256_set1_ps(dz);
					SSE_E2H = _mm256_loadu_ps(Ex[k + 1][j] + index);
					SSE_E2L = _mm256_loadu_ps(Ex[k][j] + index);
					SSE_d2 = _mm256_set1_ps(dx);
					SSE_sigma1 = _mm256_set1_ps(sigmaZ);
					SSE_sigma2 = SSE_sigmaX;
					SSE_k1 = _mm256_loadu_ps(gyz[kk][j] + index);
					SSE_k2 = _mm256_set1_ps(0);
					ADEPML_AVX(SSE_E1H, SSE_E1L, SSE_E2H, SSE_E2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_B);
					//ADEPML(Ez[k][j][i + 1], Ez[k][j][i], Ex[k + 1][j][i], Ex[k][j][i], dso, dso, sigmaZ, sigmaX, gyz[kk][j][i], gyx[k][j][ii], SHy);
					SSE_H = _mm256_mul_ps(SSE_B, SSE_IM);
					//Hy[k][j][i] = SHy * Mu0_Inv;
					//Store
					_mm256_storeu_ps(gyz[kk][j] + index, SSE_k1);
					//_mm256_storeu_ps(gyx[k][j] + index_pml, SSE_k2);
					_mm256_storeu_ps(Hy[k][j] + index, SSE_H);
					//Hy 
					//SHy = Hy[k][j][i] * Mu0;
					//ADEPML(Ez[k][j][i + 1], Ez[k][j][i], Ex[k + 1][j][i], Ex[k][j][i], dso, dso, sigmaZ, sigmaX, gyz[kk][j][i], gyx[k][j][ii], SHy);
					//Hy[k][j][i] = SHy * Mu0_Inv;

					//Hz
					SSE_H = _mm256_loadu_ps(Hz[k][j] + index);
					SSE_B = _mm256_mul_ps(SSE_H, SSE_M);
					//SHz = Hz[k][j][i] * Mu0;
					SSE_E1H = _mm256_loadu_ps(Ex[k][j + 1] + index);
					SSE_E1L = _mm256_loadu_ps(Ex[k][j] + index);
					SSE_d1 = _mm256_set1_ps(dx);
					SSE_E2H = _mm256_loadu_ps(Ey[k][j] + 1 + index);
					SSE_E2L = _mm256_loadu_ps(Ey[k][j] + index);
					SSE_d2 = _mm256_set1_ps(dy);
					SSE_sigma1 = SSE_sigmaX;
					SSE_sigma2 = _mm256_set1_ps(sigmaY);
					SSE_k1 = _mm256_set1_ps(0);
					SSE_k2 = _mm256_loadu_ps(gzy[k][jj] + index);
					ADEPML_AVX(SSE_E1H, SSE_E1L, SSE_E2H, SSE_E2L, SSE_dt, SSE_d1, SSE_d2, SSE_sigma1, SSE_sigma2, SSE_k1, SSE_k2, SSE_B);
					//ADEPML(Ex[k][j + 1][i], Ex[k][j][i], Ey[k][j][i + 1], Ey[k][j][i], dso, dso, sigmaX, sigmaY, gzx[k][j][ii], gzy[k][jj][i], SHz);
					SSE_H = _mm256_mul_ps(SSE_B, SSE_IM);
					//Hz[k][j][i] = SHz * Mu0_Inv;
					//Store
					//_mm256_storeu_ps(gzx[k][j] + index_pml, SSE_k1);
					_mm256_storeu_ps(gzy[k][jj] + index, SSE_k2);
					_mm256_storeu_ps(Hz[k][j] + index, SSE_H);
					//Hz
					//SHz = Hz[k][j][i] * Mu0;
					//ADEPML(Ex[k][j + 1][i], Ex[k][j][i], Ey[k][j][i + 1], Ey[k][j][i], dso, dso, sigmaX, sigmaY, gzx[k][j][ii], gzy[k][jj][i], SHz);
					//Hz[k][j][i] = SHz * Mu0_Inv;
				}//i X No PML
			}//jj Y- Y+ PML
		}//k Z No PML
	}//omp
}
void CFDTD::UpdateE(){
	//Open MP
	omp_set_num_threads(threadNum);
	#pragma omp parallel
	{
		int id =  omp_get_thread_num();
		//iteration
		int i, j, k;	int i_sse;
		int ii,jj,kk;
		float idt = 1/dt;
		float Epso = Eps0f;		
		float dtdx = dt / dx;
		float dtdy = dt / dy;
		float dtdz = dt / dz;
		int ks, ke; 

		//Dielectric With loss using CA and CB
		//No Pml Region Pure Computation Domain
		ks = Nzs_cal[id];
		ke = Nzs_cal[id] + Nzn_cal[id]-1;
		//register __m256 SSE_ds = _mm256_set1_ps(dso); //4
		register __m256 SSE_d1;
		register __m256 SSE_d2;
		register __m256 SSE_E; 
		register __m256 SSE_H1H; register __m256 SSE_H1L;
		register __m256 SSE_H2H; register __m256 SSE_H2L;
		register __m256 SSE_CA;	 register __m256 SSE_CB;
		register __m256 m0;		 register __m256 m1;
		for(kk=ks;kk<=ke;kk++){
			k = kk+num_pml;
			for(jj=1;jj<=Ny_cal;jj++){
				j = jj+num_pml;
				for(ii=1;ii<Nx_cal;ii+=AVX_Count){
					i = ii+num_pml;

					//Ex					
					SSE_E = _mm256_loadu_ps(Ex[k][j]+i);
					SSE_H1H = _mm256_loadu_ps(Hz[k][j]+i);
					SSE_H1L = _mm256_loadu_ps(Hz[k][j-1]+i);
					SSE_d1 = _mm256_set1_ps(dz);
					SSE_H2H = _mm256_loadu_ps(Hy[k][j]+i);
					SSE_H2L = _mm256_loadu_ps(Hy[k-1][j]+i);
					SSE_d2 = _mm256_set1_ps(dy);
					SSE_CA = _mm256_loadu_ps(CA_Ex[k][j]+i);
					SSE_CB = _mm256_loadu_ps(CB_Ex[k][j]+i);

					m0 = _mm256_sub_ps(SSE_H1H, SSE_H1L);
					m0 = _mm256_div_ps(m0, SSE_d2);
					m1 = _mm256_sub_ps(SSE_H2H, SSE_H2L);
					m1 = _mm256_div_ps(m1, SSE_d1);
					m0 = _mm256_sub_ps(m0, m1);

					m0 = _mm256_mul_ps(SSE_CB, m0);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					SSE_E = _mm256_add_ps(m0, m1);
					_mm256_storeu_ps(Ex[k][j]+i, SSE_E);

					//Ey
					SSE_E = _mm256_loadu_ps(Ey[k][j]+i);
					SSE_H1H = _mm256_loadu_ps(Hx[k][j]+i);
					SSE_H1L = _mm256_loadu_ps(Hx[k-1][j]+i);
					SSE_d1 = _mm256_set1_ps(dx);
					SSE_H2H = _mm256_loadu_ps(Hz[k][j]+i);
					SSE_H2L = _mm256_loadu_ps(Hz[k][j]+i-1);
					SSE_d2 = _mm256_set1_ps(dz);
					SSE_CA = _mm256_loadu_ps(CA_Ey[k][j]+i);
					SSE_CB = _mm256_loadu_ps(CB_Ey[k][j]+i);

					m0 = _mm256_sub_ps(SSE_H1H, SSE_H1L);
					m0 = _mm256_div_ps(m0, SSE_d2);
					m1 = _mm256_sub_ps(SSE_H2H, SSE_H2L);
					m1 = _mm256_div_ps(m1, SSE_d1);
					m0 = _mm256_sub_ps(m0, m1);

					m0 = _mm256_mul_ps(SSE_CB,m0);
					m1 = _mm256_mul_ps(SSE_E,SSE_CA);
					SSE_E = _mm256_add_ps(m1,m0);
					_mm256_storeu_ps(Ey[k][j]+i,SSE_E);

					//Ez
					SSE_E = _mm256_loadu_ps(Ez[k][j]+i);
					SSE_H1H = _mm256_loadu_ps(Hy[k][j]+i);
					SSE_H1L = _mm256_loadu_ps(Hy[k][j]+i-1);
					SSE_d1 = _mm256_set1_ps(dy);
					SSE_H2H = _mm256_loadu_ps(Hx[k][j]+i);
					SSE_H2L = _mm256_loadu_ps(Hx[k][j-1]+i);
					SSE_d2 = _mm256_set1_ps(dx);
					SSE_CA = _mm256_loadu_ps(CA_Ez[k][j]+i);
					SSE_CB = _mm256_loadu_ps(CB_Ez[k][j]+i);

					m0 = _mm256_sub_ps(SSE_H1H, SSE_H1L);
					m0 = _mm256_div_ps(m0, SSE_d2);
					m1 = _mm256_sub_ps(SSE_H2H, SSE_H2L);
					m1 = _mm256_div_ps(m1, SSE_d1);
					m0 = _mm256_sub_ps(m0, m1);

					m0 = _mm256_mul_ps(SSE_CB, m0);
					m1 = _mm256_mul_ps(SSE_E, SSE_CA);
					SSE_E = _mm256_add_ps(m0, m1);
					_mm256_storeu_ps(Ez[k][j]+i, SSE_E);
				
				}//ii-AVX
			}//jj
		}//kk
	}//omp
}
void CFDTD::UpdateH(){
	omp_set_num_threads(threadNum);
	#pragma omp parallel
	{
		int id = omp_get_thread_num();

		//iteration
		int i, j, k;			int ii, jj, kk;			register int i_sse;
		//for pml
		float Muo = Mu0f;
		float dtdxMu0 = dt / dx / Mu0f;
		float dtdyMu0 = dt / dy / Mu0f;
		float dtdzMu0 = dt / dz / Mu0f;
		int ks, ke;

		//No Pml Region Pure Computation Domain --AVX 
		ks = Nzs_cal[id];
		ke = Nzs_cal[id] + Nzn_cal[id]-1;

		//register __m256 SSE_dtds = _mm256_set1_ps(dtdsMu0); //4
		
		register __m256 SSE_H;
		register __m256 SSE_dtd1;	register __m256 SSE_dtd2;
		register __m256 SSE_E2L;	register __m256 SSE_E2H;
		register __m256 SSE_E1L;	register __m256 SSE_E1H;
		register __m256 m0;	__m256 m1;
		for(kk=ks;kk<=ke;kk++)	{
			k = kk+num_pml;
			for(jj=1;jj<=Ny_cal;jj++){
				j = jj + num_pml;
				for(ii=1;ii<Nx_cal;ii+=AVX_Count){
					i = ii+num_pml;
					//cout<<m1.m128_f32[0]<<" "<<id<<endl;
					//Hx
					SSE_H = _mm256_loadu_ps(Hx[k][j]+i);
					SSE_E1L = _mm256_loadu_ps(Ey[k][j]+i);
					SSE_E1H = _mm256_loadu_ps(Ey[k+1][j]+i);
					SSE_dtd1 = _mm256_set1_ps(dtdyMu0);
					SSE_E2L = _mm256_loadu_ps(Ez[k][j]+i);
					SSE_E2H = _mm256_loadu_ps(Ez[k][j+1]+i);
					SSE_dtd2 = _mm256_set1_ps(dtdzMu0);

					m0 = _mm256_sub_ps(SSE_E1H, SSE_E1L);
					m0 = _mm256_mul_ps(m0, SSE_dtd2);
					m1 = _mm256_sub_ps(SSE_E2H, SSE_E2L);
					m1 = _mm256_mul_ps(m1, SSE_dtd1);
					m0 = _mm256_sub_ps(m0, m1);

					m0 = _mm256_add_ps(m0, SSE_H);
					_mm256_storeu_ps(Hx[k][j] + i, m0);

					//Hy
					SSE_H = _mm256_loadu_ps(Hy[k][j]+i);
					SSE_E1L = _mm256_loadu_ps(Ez[k][j]+i);
					SSE_E1H = _mm256_loadu_ps(Ez[k][j]+i+1);
					SSE_dtd1 = _mm256_set1_ps(dtdzMu0);
					SSE_E2L = _mm256_loadu_ps(Ex[k][j]+i);
					SSE_E2H = _mm256_loadu_ps(Ex[k+1][j]+i);
					SSE_dtd2 = _mm256_set1_ps(dtdxMu0);

					m0 = _mm256_sub_ps(SSE_E1H, SSE_E1L);
					m0 = _mm256_mul_ps(m0, SSE_dtd2);
					m1 = _mm256_sub_ps(SSE_E2H, SSE_E2L);
					m1 = _mm256_mul_ps(m1, SSE_dtd1);
					m0 = _mm256_sub_ps(m0, m1);

					m0 = _mm256_add_ps(m0,SSE_H);
					_mm256_storeu_ps(Hy[k][j]+i,m0);

					//Hz
					SSE_H = _mm256_loadu_ps(Hz[k][j]+i);
					SSE_E1L = _mm256_loadu_ps(Ex[k][j]+i);
					SSE_E1H = _mm256_loadu_ps(Ex[k][j+1]+i);
					SSE_dtd1 = _mm256_set1_ps(dtdxMu0);
					SSE_E2L = _mm256_loadu_ps(Ey[k][j]+i);
					SSE_E2H = _mm256_loadu_ps(Ey[k][j]+i+1);
					SSE_dtd2 = _mm256_set1_ps(dtdyMu0);

					m0 = _mm256_sub_ps(SSE_E1H, SSE_E1L);
					m0 = _mm256_mul_ps(m0, SSE_dtd2);
					m1 = _mm256_sub_ps(SSE_E2H, SSE_E2L);
					m1 = _mm256_mul_ps(m1, SSE_dtd1);
					m0 = _mm256_sub_ps(m0, m1);

					m0 = _mm256_add_ps(m0, SSE_H);
					_mm256_storeu_ps(Hz[k][j]+i, m0);

				}//ii
			}//jj
		}//kk
	}
}
void CFDTD::Output(){
	int i,j,k,f,th,ph;
	int ii, jj, kk;
	FILE *fdtd;
	if(computed == 0){//checking
		if(nn%Step2Show == 0){
			//cout<<Despath<<" "<<FileoutPath<<endl;
			fstream FieldFile;
			FieldFile.open("./sizeXOZ.txt", ios::out);
			FieldFile << Nx << " " << Nz << endl;
			FieldFile.close();
			FieldFile.open("./XOZ.txt", ios::out);
			j = Ny / 2;
			for (k=1;k<=Nz;k++) {
				for (i=1;i<=Nx;i++) {
					FieldFile << i << " " << k << " " << Ex[k][j][i] << " " << Ey[k][j][i] << " " << Ez[k][j][i]<<" "<<EpsMap[k][j][i] << endl;
				}
			}
			FieldFile.close();
			//cout << Despath << " " << FileoutPath << endl;
			FieldFile.open("./sizeYOZ.txt", ios::out);
			FieldFile << Ny << " " << Nz << endl;
			FieldFile.close();
			FieldFile.open("./YOZ.txt", ios::out);
			i = Nx / 2;
			for (k = 1; k <= Nz; k++) {
				for (j = 1; j <= Ny; j++) {
					FieldFile << j << " " << k << " " << Ex[k][j][i] << " " << Ey[k][j][i] << " " << Ez[k][j][i] << " " << EpsMap[k][j][i] << endl;
				}
			}
			FieldFile.close();
			//cout << Despath << " " << FileoutPath << endl;
			FieldFile.open("./sizeXOY.txt", ios::out);
			FieldFile << Nx << " " << Ny << endl;
			FieldFile.close();
			FieldFile.open("./XOY.txt", ios::out);
			k = Nz_exc+1;
			for (j = 1; j <= Ny; j++) {
				for (i = 1; i <= Nx; i++) {
					FieldFile << i << " " << j << " " << Ex[k][j][i] << " " << Ey[k][j][i] << " " << Ez[k][j][i] << " " << EpsMap[k][j][i] << endl;
				}
			}
			FieldFile.close();
		}
	}
	if (timemode == 0) {//靠这个输出 DFT 
		//Note: The time difference in H and E (half time step), E first n; then H n+1/2
		//Center Difference at n: H: H(n) = ( H(n-1/2)+H(n+1/2) )*0.5;
		if (nn == stopnn - 2 - NN) {//先输出虚部 只有磁场 H(n-1/2)
		//将场值记录到DFT_aperture位置上
		//XOY_1
			kk = Pz_DFT_1;	//Nz_model
			float tempE1, tempE2, tempH1, tempH2;
			for (j = 1; j <= Ny_DFT; j++) {
				jj = j + N_spa;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;

					//Hx
					tempH1 = (Hx[kk + 1][jj][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hy
					tempH2 = (Hy[kk + 1][jj][ii] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;

					Hx_z1[0][j][i] = complex<float>(0, tempH1);
					Hy_z1[0][j][i] = complex<float>(0, tempH2);
				}
			}
			//YOZ_0
			ii = Px_DFT_0;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (j = 1; j <= Ny_DFT; j++) {
					jj = j + N_spa;
					//Hy
					tempH1 = (Hy[kk][jj][ii + 1] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj][ii + 1] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Hy_x0[0][k][j] = complex<float>(0, tempH1);
					Hz_x0[0][k][j] = complex<float>(0, tempH2);
				}
			}
			//YOZ_1
			ii = Px_DFT_1;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (j = 1; j <= Ny_DFT; j++) {
					jj = j + N_spa;

					//Hy
					tempH1 = (Hy[kk][jj][ii + 1] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj][ii + 1] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Hy_x1[0][k][j] = complex<float>(0, tempH1);
					Hz_x1[0][k][j] = complex<float>(0, tempH2);
				}
			}
			//XOZ_0
			jj = Py_DFT_0;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;

					//Hx
					tempH1 = (Hx[kk][jj + 1][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj + 1][ii] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Hx_y0[0][k][i] = complex<float>(0, tempH1);
					Hz_y0[0][k][i] = complex<float>(0, tempH2);
				}
			}

			//XOZ_1
			jj = Py_DFT_1;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;

					//Hx
					tempH1 = (Hx[kk][jj + 1][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj + 1][ii] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Hx_y1[0][k][i] = complex<float>(0, tempH1);
					Hz_y1[0][k][i] = complex<float>(0, tempH2);
				}
			}
		}//nn == stopnn - 2 - NN 只有磁场 H(n-1/2)

		if (nn == stopnn - 1 - NN) {//先输出虚部
			//将场值记录到DFT_aperture位置上
			//XOY_1
			kk = Pz_DFT_1;	//Nz_model
			float tempE1, tempE2, tempH1, tempH2;
			for (j = 1; j <= Ny_DFT; j++) {
				jj = j + N_spa;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;
					//Ex
					tempE1 = (Ex[kk][jj][ii] + Ex[kk + 1][jj][ii] + Ex[kk][jj][ii + 1] + Ex[kk + 1][jj][ii + 1])*0.25;
					//Ey
					tempE2 = (Ey[kk][jj][ii] + Ey[kk + 1][jj][ii] + Ey[kk][jj + 1][ii] + Ey[kk + 1][jj + 1][ii])*0.25;
					//Hx
					tempH1 = (Hx[kk + 1][jj][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hy
					tempH2 = (Hy[kk + 1][jj][ii] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;

					Ex_z1[0][j][i] = complex<float>(0, tempE1);
					Ey_z1[0][j][i] = complex<float>(0, tempE2);
					Hx_z1[0][j][i] += complex<float>(0, tempH1);
					Hy_z1[0][j][i] += complex<float>(0, tempH2);
				}
			}
			//YOZ_0
			ii = Px_DFT_0;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (j = 1; j <= Ny_DFT; j++) {
					jj = j + N_spa;
					//Ey
					tempE1 = (Ey[kk][jj][ii] + Ey[kk][jj][ii + 1] + Ey[kk][jj + 1][ii] + Ey[kk][jj + 1][ii + 1])*0.25;
					//Ez
					tempE2 = (Ez[kk][jj][ii] + Ez[kk][jj][ii + 1] + Ez[kk + 1][jj][ii] + Ez[kk + 1][jj][ii + 1])*0.25;
					//Hy
					tempH1 = (Hy[kk][jj][ii + 1] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj][ii + 1] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;
					
					Ey_x0[0][k][j] = complex<float>(0, tempE1);
					Ez_x0[0][k][j] = complex<float>(0, tempE2);
					Hy_x0[0][k][j] += complex<float>(0, tempH1);
					Hz_x0[0][k][j] += complex<float>(0, tempH2);
				}
			}
			//YOZ_1
			ii = Px_DFT_1;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (j = 1; j <= Ny_DFT; j++) {
					jj = j + N_spa;
					//Ey
					tempE1 = (Ey[kk][jj][ii] + Ey[kk][jj][ii + 1] + Ey[kk][jj + 1][ii] + Ey[kk][jj + 1][ii + 1])*0.25;
					//Ez
					tempE2 = (Ez[kk][jj][ii] + Ez[kk][jj][ii + 1] + Ez[kk + 1][jj][ii] + Ez[kk + 1][jj][ii + 1])*0.25;
					//Hy
					tempH1 = (Hy[kk][jj][ii + 1] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj][ii + 1] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Ey_x1[0][k][j] = complex<float>(0, tempE1);
					Ez_x1[0][k][j] = complex<float>(0, tempE2);
					Hy_x1[0][k][j] += complex<float>(0, tempH1);
					Hz_x1[0][k][j] += complex<float>(0, tempH2);
				}
			}
			//XOZ_0
			jj = Py_DFT_0;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;
					//Ex
					tempE1 = (Ex[kk][jj][ii] + Ex[kk][jj + 1][ii] + Ex[kk][jj][ii + 1] + Ex[kk][jj + 1][ii + 1])*0.25;
					//Ez
					tempE2 = (Ez[kk][jj][ii] + Ez[kk][jj + 1][ii] + Ez[kk + 1][jj][ii] + Ez[kk + 1][jj + 1][ii])*0.25;
					//Hx
					tempH1 = (Hx[kk][jj + 1][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj + 1][ii] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Ex_y0[0][k][i] = complex<float>(0, tempE1);
					Ez_y0[0][k][i] = complex<float>(0, tempE2);
					Hx_y0[0][k][i] += complex<float>(0, tempH1);
					Hz_y0[0][k][i] += complex<float>(0, tempH2);
				}
			}

			//XOZ_1
			jj = Py_DFT_1;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;
					//Ex
					tempE1 = (Ex[kk][jj][ii] + Ex[kk][jj + 1][ii] + Ex[kk][jj][ii + 1] + Ex[kk][jj + 1][ii + 1])*0.25;
					//Ez
					tempE2 = (Ez[kk][jj][ii] + Ez[kk][jj + 1][ii] + Ez[kk + 1][jj][ii] + Ez[kk + 1][jj + 1][ii])*0.25;
					//Hx
					tempH1 = (Hx[kk][jj + 1][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj + 1][ii] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Ex_y1[0][k][i] = complex<float>(0, tempE1);
					Ez_y1[0][k][i] = complex<float>(0, tempE2);
					Hx_y1[0][k][i] += complex<float>(0, tempH1);
					Hz_y1[0][k][i] += complex<float>(0, tempH2);
				}
			}
		}// nn = stopnn - 1 - NN

		if (nn == stopnn - 2) {//再输出实部	只有磁场
		//将场值记录到DFT_aperture位置上
		//XOY_1
			kk = Pz_DFT_1;
			float tempE1, tempE2, tempH1, tempH2;
			for (j = 1; j <= Ny_DFT; j++) {
				jj = j + N_spa;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;

					tempH1 = (Hx[kk + 1][jj][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					tempH2 = (Hy[kk + 1][jj][ii] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;

					Hx_z1[0][j][i] += complex<float>(tempH1, 0);
					Hy_z1[0][j][i] += complex<float>(tempH2, 0);
				}
			}
			//YOZ_0
			ii = Px_DFT_0;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (j = 1; j <= Ny_DFT; j++) {
					jj = j + N_spa;

					//Hy
					tempH1 = (Hy[kk][jj][ii + 1] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj][ii + 1] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Hy_x0[0][k][j] += complex<float>(tempH1, 0);
					Hz_x0[0][k][j] += complex<float>(tempH2, 0);
				}
			}
			//YOZ_1
			ii = Px_DFT_1;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (j = 1; j <= Ny_DFT; j++) {
					jj = j + N_spa;

					//Hy
					tempH1 = (Hy[kk][jj][ii + 1] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj][ii + 1] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Hy_x1[0][k][j] += complex<float>(tempH1, 0);
					Hz_x1[0][k][j] += complex<float>(tempH2, 0);
				}
			}
			//XOZ_0
			jj = Py_DFT_0;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;

					//Hx
					tempH1 = (Hx[kk][jj + 1][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj + 1][ii] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Hx_y0[0][k][i] += complex<float>(tempH1, 0);
					Hz_y0[0][k][i] += complex<float>(tempH2, 0);
				}
			}

			//XOZ_1
			jj = Py_DFT_1;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;

					//Hx
					tempH1 = (Hx[kk][jj + 1][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj + 1][ii] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Hx_y1[0][k][i] += complex<float>(tempH1, 0);
					Hz_y1[0][k][i] += complex<float>(tempH2, 0);
				}//for i
			}//for k

		}//if nn = stopnn - 2;	//只有磁场

		if (nn == stopnn - 1) {//再输出实部
			//将场值记录到DFT_aperture位置上
			//XOY_1
			kk = Pz_DFT_1;
			float tempE1, tempE2, tempH1, tempH2;
			for (j = 1; j <= Ny_DFT; j++) {
				jj = j + N_spa;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;
					//Ex
					tempE1 = (Ex[kk][jj][ii] + Ex[kk + 1][jj][ii] + Ex[kk][jj][ii + 1] + Ex[kk + 1][jj][ii + 1])*0.25;
					tempE2 = (Ey[kk][jj][ii] + Ey[kk + 1][jj][ii] + Ey[kk][jj + 1][ii] + Ey[kk + 1][jj + 1][ii])*0.25;
					tempH1 = (Hx[kk + 1][jj][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					tempH2 = (Hy[kk + 1][jj][ii] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;

					Ex_z1[0][j][i] += complex<float>(tempE1, 0);
					Ey_z1[0][j][i] += complex<float>(tempE2, 0);
					Hx_z1[0][j][i] += complex<float>(tempH1, 0);
					Hy_z1[0][j][i] += complex<float>(tempH2, 0);
				}
			}
			//YOZ_0
			ii = Px_DFT_0;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (j = 1; j <= Ny_DFT; j++) {
					jj = j + N_spa;
					//Ey
					tempE1 = (Ey[kk][jj][ii] + Ey[kk][jj][ii + 1] + Ey[kk][jj + 1][ii] + Ey[kk][jj + 1][ii + 1])*0.25;
					//Ez
					tempE2 = (Ez[kk][jj][ii] + Ez[kk][jj][ii + 1] + Ez[kk + 1][jj][ii] + Ez[kk + 1][jj][ii + 1])*0.25;
					//Hy
					tempH1 = (Hy[kk][jj][ii + 1] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj][ii + 1] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Ey_x0[0][k][j] += complex<float>(tempE1, 0);
					Ez_x0[0][k][j] += complex<float>(tempE2, 0);
					Hy_x0[0][k][j] += complex<float>(tempH1, 0);
					Hz_x0[0][k][j] += complex<float>(tempH2, 0);
				}
			}
			//YOZ_1
			ii = Px_DFT_1;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (j = 1; j <= Ny_DFT; j++) {
					jj = j + N_spa;
					//Ey
					tempE1 = (Ey[kk][jj][ii] + Ey[kk][jj][ii + 1] + Ey[kk][jj + 1][ii] + Ey[kk][jj + 1][ii + 1])*0.25;
					//Ez
					tempE2 = (Ez[kk][jj][ii] + Ez[kk][jj][ii + 1] + Ez[kk + 1][jj][ii] + Ez[kk + 1][jj][ii + 1])*0.25;
					//Hy
					tempH1 = (Hy[kk][jj][ii + 1] + Hy[kk + 1][jj][ii + 1])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj][ii + 1] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Ey_x1[0][k][j] += complex<float>(tempE1, 0);
					Ez_x1[0][k][j] += complex<float>(tempE2, 0);
					Hy_x1[0][k][j] += complex<float>(tempH1, 0);
					Hz_x1[0][k][j] += complex<float>(tempH2, 0);
				}
			}
			//XOZ_0
			jj = Py_DFT_0;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;
					//Ex
					tempE1 = (Ex[kk][jj][ii] + Ex[kk][jj + 1][ii] + Ex[kk][jj][ii + 1] + Ex[kk][jj + 1][ii + 1])*0.25;
					//Ez
					tempE2 = (Ez[kk][jj][ii] + Ez[kk][jj + 1][ii] + Ez[kk + 1][jj][ii] + Ez[kk + 1][jj + 1][ii])*0.25;
					//Hx
					tempH1 = (Hx[kk][jj + 1][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj + 1][ii] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Ex_y0[0][k][i] += complex<float>(tempE1, 0);
					Ez_y0[0][k][i] += complex<float>(tempE2, 0);
					Hx_y0[0][k][i] += complex<float>(tempH1, 0);
					Hz_y0[0][k][i] += complex<float>(tempH2, 0);
				}
			}

			//XOZ_1
			jj = Py_DFT_1;
			for (k = 1; k <= Nz_DFT; k++) {
				kk = k;
				for (i = 1; i <= Nx_DFT; i++) {
					ii = i + N_spa;
					//Ex
					tempE1 = (Ex[kk][jj][ii] + Ex[kk][jj + 1][ii] + Ex[kk][jj][ii + 1] + Ex[kk][jj + 1][ii + 1])*0.25;
					//Ez
					tempE2 = (Ez[kk][jj][ii] + Ez[kk][jj + 1][ii] + Ez[kk + 1][jj][ii] + Ez[kk + 1][jj + 1][ii])*0.25;
					//Hx
					tempH1 = (Hx[kk][jj + 1][ii] + Hx[kk + 1][jj + 1][ii])*0.5*0.5;
					//Hz
					tempH2 = (Hz[kk][jj + 1][ii] + Hz[kk][jj + 1][ii + 1])*0.5*0.5;

					Ex_y1[0][k][i] += complex<float>(tempE1, 0);
					Ez_y1[0][k][i] += complex<float>(tempE2, 0);
					Hx_y1[0][k][i] += complex<float>(tempH1, 0);
					Hz_y1[0][k][i] += complex<float>(tempH2, 0);
				}//for i
			}//for k


		}//if nn = stopnn - 1;


	}//if timemode == 0

	if (nn == stopnn - 1) {//计算完成
		//Write into Huygens Box Data //FiveBox
		HuygensBoxData = Allocate_1D(HuygensBoxData,Nfreq*(Nx_DFT*Ny_DFT + Nx_DFT*Nz_DFT * 2 + Ny_DFT*Nz_DFT * 2) * 4);

		//Writeinto Data
		int ShiftFreq, ShiftFace, ShiftEx, ShiftEy, ShiftEz, ShiftHx, ShiftHy, ShiftHz, vAddress;
		ShiftFreq = (Nx_DFT*Ny_DFT + Nx_DFT*Nz_DFT * 2 + Ny_DFT*Nz_DFT * 2) * 4 * 0;
		for (f = 0; f < Nfreq; f++) {
			//第一个惠更斯面 XOY1
			ShiftFace = 0;	
			ShiftEx = Nx_DFT*Ny_DFT * 0; 
			ShiftEy = Nx_DFT*Ny_DFT * 1; 
			ShiftEz = 0;
			ShiftHx = Nx_DFT*Ny_DFT * 2;
			ShiftHy = Nx_DFT*Ny_DFT * 3;
			ShiftHz = 0;
			for (j = 1; j <= Ny_DFT; j++) {	//注意0和1
				for (i = 1; i <= Nx_DFT; i++) {	//注意0和1
					vAddress = (i - 1) + (j - 1)*Nx_DFT;
					HuygensBoxData[vAddress + ShiftEx + ShiftFace + ShiftFreq] = Ex_z1[f][j][i];	//Ex
					HuygensBoxData[vAddress + ShiftEy + ShiftFace + ShiftFreq] = Ey_z1[f][j][i];	//Ey
					HuygensBoxData[vAddress + ShiftHx + ShiftFace + ShiftFreq] = Hx_z1[f][j][i];	//Hx
					HuygensBoxData[vAddress + ShiftHy + ShiftFace + ShiftFreq] = Hy_z1[f][j][i];	//Hy
				}
			}
			//第二个惠更斯面 XOZ0
			ShiftFace = Nx_DFT*Ny_DFT * 4;	//已有XOY1
			ShiftEx = Nx_DFT*Nz_DFT * 0;
			ShiftEy = 0;
			ShiftEz = Nx_DFT*Nz_DFT * 1;
			ShiftHx = Nx_DFT*Nz_DFT * 2;
			ShiftHy = 0;
			ShiftHz = Nx_DFT*Nz_DFT * 3;
			for (k = 1; k <= Nz_DFT; k++) {	//注意0和1
				for (i = 1; i <= Nx_DFT; i++) {	//注意0和1
					vAddress = (i - 1) + (k - 1)*Nx_DFT;
					HuygensBoxData[vAddress + ShiftEx + ShiftFace + ShiftFreq] = Ex_y0[f][k][i];	//Ex
					HuygensBoxData[vAddress + ShiftEz + ShiftFace + ShiftFreq] = Ez_y0[f][k][i];	//Ez
					HuygensBoxData[vAddress + ShiftHx + ShiftFace + ShiftFreq] = Hx_y0[f][k][i];	//Hx
					HuygensBoxData[vAddress + ShiftHz + ShiftFace + ShiftFreq] = Hz_y0[f][k][i];	//Hz
				}
			}
			//第三个惠更斯面 XOZ1
			ShiftFace = Nx_DFT*Ny_DFT * 4 + Nx_DFT*Nz_DFT * 4;	//已有XOY1和XOZ0
			ShiftEx = Nx_DFT*Nz_DFT * 0;
			ShiftEy = 0;
			ShiftEz = Nx_DFT*Nz_DFT * 1;
			ShiftHx = Nx_DFT*Nz_DFT * 2;
			ShiftHy = 0;
			ShiftHz = Nx_DFT*Nz_DFT * 3;
			for (k = 1; k <= Nz_DFT; k++) {	//注意0和1
				for (i = 1; i <= Nx_DFT; i++) {	//注意0和1
					vAddress = (i - 1) + (k - 1)*Nx_DFT;
					HuygensBoxData[vAddress + ShiftEx + ShiftFace + ShiftFreq] = Ex_y1[f][k][i];	//Ex
					HuygensBoxData[vAddress + ShiftEz + ShiftFace + ShiftFreq] = Ez_y1[f][k][i];	//Ez
					HuygensBoxData[vAddress + ShiftHx + ShiftFace + ShiftFreq] = Hx_y1[f][k][i];	//Hx
					HuygensBoxData[vAddress + ShiftHz + ShiftFace + ShiftFreq] = Hz_y1[f][k][i];	//Hz
				}
			}
			//第四个惠更斯面 YOZ0
			ShiftFace = Nx_DFT*Ny_DFT * 4 + Nx_DFT*Nz_DFT * 4 * 2;	//已有XOY1、XOZ0和XOZ1.
			ShiftEx = 0;
			ShiftEy = Ny_DFT*Nz_DFT * 0;
			ShiftEz = Ny_DFT*Nz_DFT * 1;
			ShiftHx = 0;
			ShiftHy = Ny_DFT*Nz_DFT * 2;
			ShiftHz = Ny_DFT*Nz_DFT * 3;
			for (k = 1; k <= Nz_DFT; k++) {	//注意0和1
				for (j = 1; j <= Ny_DFT; j++) {	//注意0和1
					vAddress = (j - 1) + (k - 1)*Ny_DFT;
					HuygensBoxData[vAddress + ShiftEy + ShiftFace + ShiftFreq] = Ey_x0[f][k][j];	//Ey
					HuygensBoxData[vAddress + ShiftEz + ShiftFace + ShiftFreq] = Ez_x0[f][k][j];	//Ez
					HuygensBoxData[vAddress + ShiftHy + ShiftFace + ShiftFreq] = Hy_x0[f][k][j];	//Hy
					HuygensBoxData[vAddress + ShiftHz + ShiftFace + ShiftFreq] = Hz_x0[f][k][j];	//Hz
				}
			}
			//第五个惠更斯面 YOZ1
			ShiftFace = Nx_DFT*Ny_DFT * 4 + Nx_DFT*Nz_DFT * 4 * 2 + Ny_DFT*Nz_DFT*4;	//已有XOY1、XOZ0、XOZ1和YOZ0.
			ShiftEx = 0;
			ShiftEy = Ny_DFT*Nz_DFT * 0;
			ShiftEz = Ny_DFT*Nz_DFT * 1;
			ShiftHx = 0;
			ShiftHy = Ny_DFT*Nz_DFT * 2;
			ShiftHz = Ny_DFT*Nz_DFT * 3;
			for (k = 1; k <= Nz_DFT; k++) {	//注意0和1
				for (j = 1; j <= Ny_DFT; j++) {	//注意0和1
					vAddress = (j - 1) + (k - 1)*Ny_DFT;
					HuygensBoxData[vAddress + ShiftEy + ShiftFace + ShiftFreq] = Ey_x1[f][k][j];	//Ey
					HuygensBoxData[vAddress + ShiftEz + ShiftFace + ShiftFreq] = Ez_x1[f][k][j];	//Ez
					HuygensBoxData[vAddress + ShiftHy + ShiftFace + ShiftFreq] = Hy_x1[f][k][j];	//Hy
					HuygensBoxData[vAddress + ShiftHz + ShiftFace + ShiftFreq] = Hz_x1[f][k][j];	//Hz
				}
			}
			//写入完成
		}//SweepFreq

		//WriteHuygensBoxintoFiles
		FILE* BinaryWrite;
		BinaryWrite = fopen("./HuygensBoxData.dat", "wb");
		//size first
		fwrite(&Nx_DFT, sizeof(int), 1, BinaryWrite);
		fwrite(&Ny_DFT, sizeof(int), 1, BinaryWrite);
		fwrite(&Nz_DFT, sizeof(int), 1, BinaryWrite);
		//discrete interval
		fwrite(&dx, sizeof(float), 1, BinaryWrite);
		fwrite(&dy, sizeof(float), 1, BinaryWrite);
		fwrite(&dz, sizeof(float), 1, BinaryWrite);
		//Freq
		fwrite(&Nfreq, sizeof(int), 1, BinaryWrite);
		for (f = 0; f < Nfreq; f++) {
			fwrite(&freqs[f], sizeof(float), 1, BinaryWrite);
		}
		//Field
		int Size = (Nx_DFT*Ny_DFT + Nx_DFT*Nz_DFT * 2 + Ny_DFT*Nz_DFT * 2) * 4 * Nfreq;
		for (int i = 0; i < Size; i++) {
			fwrite(&HuygensBoxData[i], sizeof(complex<float>), 1, BinaryWrite);
		}
		fclose(BinaryWrite);

	}// nn = stopnn-1

	
}
void CFDTD::TestPointSource(){
	int i, j, k;	float time;
	i = Nx / 2 + 20;	j = Ny / 2 + 20;	k = Nz_exc;
	Ey[k][j][i] = Ey[k][j][i] + sin(nn*dt*ww);
}
void CFDTD::AddSourceE(){//Excitation Port
	omp_set_num_threads(threadNum);
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		int i, j, k;
		int ii, jj, kk;
		float time;	time = dt*nn;
		k = Nz_exc;
		//timevalue
		float cosnt,sinnt;
		float cosphase, sinphase;
		float timevalue;
		cosnt = Timesource(time,1); sinnt = Timesource(time,2);
		//AVX Version
		register __m256 AVX_cosP;
		register __m256 AVX_sinP;
		register __m256 AVX_cosnt; AVX_cosnt = _mm256_set1_ps(cosnt);
		register __m256 AVX_sinnt; AVX_sinnt = _mm256_set1_ps(sinnt);
		register __m256 AVX_mag;
		register __m256 AVX_timeV;
		register __m256 AVX_E;
		//Add Port
		for (jj = Nys_cal[id]; jj < Nys_cal[id] + Nyn_cal[id]; jj++) {
			j = jj + num_pml;
			//for (ii = 1; ii <= Nx_cal; ii++) {
			for(ii = 1; ii<Nx_cal;ii+=AVX_Count){
				i = ii + num_pml;
				//Ex
				//cosphase = Ex_AP_cos[j][i];	sinphase = Ex_AP_sin[j][i];
				//timevalue = Ex_AP_mag[j][i];
				//timevalue = timevalue*(cosnt*cosphase - sinnt*sinphase);
				//Ex[k][j][i] = Ex[k][j][i] + timevalue;
				//Ey
				//cosphase = Ey_AP_cos[j][i];	sinphase = Ey_AP_sin[j][i];
				//timevalue = Ey_AP_mag[j][i];
				//timevalue = timevalue*(cosnt*cosphase - sinnt*sinphase);
				//Ey[k][j][i] = Ey[k][j][i] + timevalue;

				//Ex
				AVX_E = _mm256_loadu_ps(Ex[k][j]+i);
				AVX_cosP = _mm256_loadu_ps(Ex_AP_cos[j] + i);
				AVX_sinP = _mm256_loadu_ps(Ex_AP_sin[j] + i);
				AVX_mag = _mm256_loadu_ps(Ex_AP_mag[j] + i);
				AVX_timeV = _mm256_sub_ps(_mm256_mul_ps(AVX_cosnt,AVX_cosP), _mm256_mul_ps(AVX_sinnt,AVX_sinP));
				AVX_timeV = _mm256_mul_ps(AVX_mag, AVX_timeV);
				AVX_E = _mm256_add_ps(AVX_E, AVX_timeV);
				_mm256_storeu_ps(Ex[k][j]+i,AVX_E);
				//Ey
				AVX_E = _mm256_loadu_ps(Ey[k][j] + i);
				AVX_cosP = _mm256_loadu_ps(Ey_AP_cos[j] + i);
				AVX_sinP = _mm256_loadu_ps(Ey_AP_sin[j] + i);
				AVX_mag = _mm256_loadu_ps(Ey_AP_mag[j] + i);
				AVX_timeV = _mm256_sub_ps(_mm256_mul_ps(AVX_cosnt, AVX_cosP), _mm256_mul_ps(AVX_sinnt, AVX_sinP));
				AVX_timeV = _mm256_mul_ps(AVX_mag, AVX_timeV);
				AVX_E = _mm256_add_ps(AVX_E, AVX_timeV);
				_mm256_storeu_ps(Ey[k][j] + i, AVX_E);


			}//ii
		}//jj
	}//omp
}
void CFDTD::AddSourceH(){
	omp_set_num_threads(threadNum);
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		int i, j, k;
		int ii, jj, kk;
		float time;
		time = dt*(nn+0.5);
		k = Nz_exc;
		//timevalue
		float cosnt, sinnt;
		float cosphase, sinphase;
		float timevalue;
		cosnt = Timesource(time, 1); sinnt = Timesource(time, 2);
		//AVX Version
		register __m256 AVX_cosP;
		register __m256 AVX_sinP;
		register __m256 AVX_cosnt; AVX_cosnt = _mm256_set1_ps(cosnt);
		register __m256 AVX_sinnt; AVX_sinnt = _mm256_set1_ps(sinnt);
		register __m256 AVX_mag;
		register __m256 AVX_timeV;
		register __m256 AVX_H;

		for (jj = Nys_cal[id]; jj < Nys_cal[id] + Nyn_cal[id]; jj++) {
			j = jj + num_pml;
			for(ii = 1; ii<Nx_cal; ii+=AVX_Count){
			//for (ii = 1; ii <= Nx_cal; ii++) {
				i = ii + num_pml;
				//Hx
				//cosphase = Hx_AP_cos[j][i];	sinphase = Hx_AP_sin[j][i];
				//timevalue = Hx_AP_mag[j][i];
				//timevalue = timevalue*(cosnt*cosphase - sinnt*sinphase);
				//Hx[k][j][i] = Hx[k][j][i] + timevalue;
				//Hy
				//cosphase = Hy_AP_cos[j][i];	sinphase = Hy_AP_sin[j][i];
				//timevalue = Hy_AP_mag[j][i];
				//timevalue = timevalue*(cosnt*cosphase - sinnt*sinphase);
				//Hy[k][j][i] = Hy[k][j][i] + timevalue;

				//Hx
				AVX_H = _mm256_loadu_ps(Hx[k][j] + i);
				AVX_cosP = _mm256_loadu_ps(Hx_AP_cos[j] + i);
				AVX_sinP = _mm256_loadu_ps(Hx_AP_sin[j] + i);
				AVX_mag = _mm256_loadu_ps(Hx_AP_mag[j] + i);
				AVX_timeV = _mm256_sub_ps(_mm256_mul_ps(AVX_cosnt, AVX_cosP), _mm256_mul_ps(AVX_sinnt, AVX_sinP));
				AVX_timeV = _mm256_mul_ps(AVX_mag, AVX_timeV);
				AVX_H = _mm256_add_ps(AVX_H, AVX_timeV);
				_mm256_storeu_ps(Hx[k][j] + i, AVX_H);
				//Ey
				AVX_H = _mm256_loadu_ps(Hy[k][j] + i);
				AVX_cosP = _mm256_loadu_ps(Hy_AP_cos[j] + i);
				AVX_sinP = _mm256_loadu_ps(Hy_AP_sin[j] + i);
				AVX_mag = _mm256_loadu_ps(Hy_AP_mag[j] + i);
				AVX_timeV = _mm256_sub_ps(_mm256_mul_ps(AVX_cosnt, AVX_cosP), _mm256_mul_ps(AVX_sinnt, AVX_sinP));
				AVX_timeV = _mm256_mul_ps(AVX_mag, AVX_timeV);
				AVX_H = _mm256_add_ps(AVX_H, AVX_timeV);
				_mm256_storeu_ps(Hy[k][j] + i, AVX_H);

			}//ii
		}//jj
	}//omp
}
void CFDTD::Cleanup(){
	//cout << "CleanUp Computation Domain" << endl;
	(*Logfile) << "CleanUp Computation Domain" << endl;
	CleanupCom();
	//cout << "CleanUp Computation Record" << endl;
	(*Logfile) << "CleanUp Computation Record" << endl;
	CleanupRecord();
}
void CFDTD::CleanupCom(){
	int i,j,k,ii,jj,kk;
	//Fields:
	for (k = 0; k <= Nz + 1; k++) {
		for (j = 0; j <= Ny + 1; j++) {
			for (i = 0; i <= Nx + 1; i++) {
				Ex[k][j][i] = 0.0;	Ey[k][j][i] = 0.0;	Ez[k][j][i] = 0.0;
				Dx[k][j][i] = 0.0;	Dy[k][j][i] = 0.0;	Dz[k][j][i] = 0.0;
				Hx[k][j][i] = 0.0;	Hy[k][j][i] = 0.0;	Hz[k][j][i] = 0.0;
			}
		}
	}
	//pml:
	for(k=0;k<=Nz+1;k++){
		for (j = 0; j <= Ny + 1; j++) {
			for (ii = 0; ii <= num_pml * 2 + 1; ii++) {
				fyx[k][j][ii] = 0.0;	gzx[k][j][ii] = 0.0;
				gyx[k][j][ii] = 0.0;	fzx[k][j][ii] = 0.0;
			}
		}
	}
	for(kk=0;kk<=num_pml*2+1;kk++){
		for (j = 0; j <= Ny + 1; j++) {
			for (i = 0; i <= Nx + 1; i++) {
				fyz[kk][j][i] = 0.0;	gxz[kk][j][i] = 0.0;
				gyz[kk][j][i] = 0.0;	fxz[kk][j][i] = 0.0;
			}
		}
	}
	for (k = 0; k <= Nz+1; k++) {
		for (jj = 0; jj <= num_pml*2 + 1; jj++) {
			for (i = 0; i <= Nx + 1; i++) {
				fxy[k][jj][i] = 0.0;	gzy[k][jj][i] = 0.0;
				gxy[k][jj][i] = 0.0;	fzy[k][jj][i] = 0.0;
			}
		}
	}
	//Models:
	for(k=0;k<=Nz+1;k++){
		for (j = 0; j <= Ny + 1; j++) {
			for (i = 0; i <= Nx + 1; i++) {
				EpsMap[k][j][i] = 1;		Epsr[k][j][i] = Eps0f;
				Esig[k][j][i] = 0.0;
				//dielectric with loss
				CA_Ex[k][j][i] = 1;			
				CA_Ey[k][j][i] = 1;			
				CA_Ez[k][j][i] = 1;
				CB_Ex[k][j][i] = dt / Eps0f;	
				CB_Ey[k][j][i] = dt / Eps0f;	
				CB_Ez[k][j][i] = dt / Eps0f;
			}
		}
	}
	//model = 0;
	computed = 0;
}
void CFDTD::CleanupRecord(){
	int i,j,k,ph,th,f;
	for(k=0;k<=Nz+1;k++){
		for(i=0;i<=Nx+1;i++){
			Ex_xoz_real[k][i] = 0.0;			Ey_xoz_real[k][i] = 0.0;			Ez_xoz_real[k][i] = 0.0;
			Hx_xoz_real[k][i] = 0.0;			Hy_xoz_real[k][i] = 0.0;			Hz_xoz_real[k][i] = 0.0;
			Ex_xoz_imag[k][i] = 0.0;			Ey_xoz_imag[k][i] = 0.0;			Ez_xoz_imag[k][i] = 0.0;
			Hx_xoz_imag[k][i] = 0.0;			Hy_xoz_imag[k][i] = 0.0;			Hz_xoz_imag[k][i] = 0.0;
		}
	}
	/*
	if(model == 0){//Incident Field
		for (f = 0; f < Nfreq; f++) {
			for (ph = 0; ph < Nphi; ph++) {
				for (th = 0; th < Ntheta; th++) {
					FarPhi[f][ph][th] = complex<float>(0.0, 0.0);		FarTheta[f][ph][th] = complex<float>(0.0, 0.0);
				}//th
			}//ph
		}//f	
	}//model == 0
	else if(model == 1){
		//Scattering DFT Record
		for(f=0;f<Nfreq;f++){
			//z1z0
			for(j=0;j<=Ny+1;j++){
				for(i=0;i<=Nx+1;i++){
					Ex_z1[f][j][i] = complex<float>(0.0,0.0);	Ey_z1[f][j][i] = complex<float>(0.0,0.0);
					Hx_z1[f][j][i] = complex<float>(0.0,0.0);	Hy_z1[f][j][i] = complex<float>(0.0,0.0);
				}//i
			}//j
		}//f
		for(f=0;f<Nfreq;f++){
			for(ph=0;ph<Nphi;ph++){
				for (th=0;th<Ntheta;th++){
					FarPhi[f][ph][th] = complex<float>(0.0, 0.0);
					FarTheta[f][ph][th] = complex<float>(0.0, 0.0);
				}//th
			}//ph
		}//f	
	}//model == 1
	*/
}
void CFDTD::Judge(){
	float MaxTemp;
	int i,j,k;
	int Xs = num_pml+1;
	int Xe = num_pml+Nx_cal;
	int Ys = num_pml + 1;
	int Ye = num_pml + Ny_cal;
	int Zs = num_pml+1;
	int Ze = num_pml+Nz_cal;

	if(nn == 0){//Starting Point
		MaxOld = 0.0;
		MaxNew = 0.0;
		com_period = clock();
	}
	else if(nn%Step2Show == 0){//in Computation
		MaxTemp = 0.0;
		for(k=Zs;k<=Ze;k++){
			for(j=Ys;j<=Ye;j++){
				for(i=Xs;i<=Xe;i++){
					if (MaxTemp <= sqrt(Ex[k][j][i] * Ex[k][j][i] + Ey[k][j][i] * Ey[k][j][i] + Ez[k][j][i] * Ez[k][j][i])) {
						MaxTemp = sqrt(Ex[k][j][i] * Ex[k][j][i] + Ey[k][j][i] * Ey[k][j][i] + Ez[k][j][i] * Ez[k][j][i]);
					}
				}
			}
		}
		MaxNew = MaxTemp;		
		if (MaxNew >= MaxOld) MaxOld = MaxNew;
		//cout << 100.0*nn / stopnn << "%, " << 20 * log10(MaxNew / MaxOld) << "dB " <<" "<<MaxNew<<" "<<MaxOld<<" ";
		(*Logfile) << 100.0*nn / stopnn << "%, " << 20 * log10(MaxNew / MaxOld) << "dB " << " " << MaxNew << " " << MaxOld << " ";
		com_periodu = clock();
		//cout << "Speed:" << (Nx_cal*Ny_cal*Nz_cal) / 1.0e6 * Step2Show / (com_periodu - com_period)*CLOCKS_PER_SEC << "Mcells/s";
		(*Logfile) << "Speed:" << (Nx_cal*Ny_cal*Nz_cal) / 1.0e6 * Step2Show / (com_periodu - com_period)*CLOCKS_PER_SEC << "Mcells/s";
		com_period = com_periodu;
		if((MaxNew / MaxOld) < 3.16e-3){
			nn = stopnn-1;
			computed = 1;
		}
	}
}
//UpdateFunctions:
inline void CFDTD::ADEPML(float H1H, float H1L, float H2H, float H2L, float ds1, float ds2, float sigma1, float sigma2, float &k1, float &k2, float &D){

	float kt1,kt2;
	float T1,T2;

		kt1=k1; kt2=k2;
		T1 = dt*sigma1*0.5;
		T2 = dt*sigma2*0.5;		

		k1=k1*(1-T1)/(1+T1)
			+(T1*2/ds1)/(1+T1)*(H2H-H2L);

		k2=k2*(1-T2)/(1+T2)
			+(T2*2/ds2)/(1+T2)*(H1H-H1L);

		D=D+dt*((H1H-H1L)/ds2-(H2H-H2L)/ds1)
			+dt*0.5*(k1+kt1-k2-kt2);

}
inline void CFDTD::ADEPML_AVX(__m256 H1H, __m256 H1L, __m256 H2H, __m256 H2L, __m256 dti, __m256 ds1, __m256 ds2, __m256 sigma1, __m256 sigma2, __m256 &k1, __m256 &k2, __m256 &D) {

	register __m256 kt1;	 kt1 = k1;
	register __m256 kt2;	 kt2 = k2;
	register __m256 T1, T2;
	register __m256 m1, m2, m3, m4, ra;

	T1 = _mm256_mul_ps(dti, sigma1);	T2 = _mm256_mul_ps(dti, sigma2);
	m1 = _mm256_set1_ps(0.5);
	T1 = _mm256_mul_ps(m1, T1);	T2 = _mm256_mul_ps(m1, T2);
	// T1 = dt*sigma1*0.5;	T2 = dt*sigma2*0.5;

	ra = _mm256_set1_ps(1.0);
	m1 = _mm256_sub_ps(ra, T1);		m2 = _mm256_add_ps(ra, T1);
	m3 = _mm256_div_ps(m1, m2);		m3 = _mm256_mul_ps(k1, m3);
	//k1*(1 - T1) / (1 + T1)
	ra = _mm256_set1_ps(2.0);		m1 = _mm256_mul_ps(T1, ra);
	m1 = _mm256_div_ps(m1, ds1);	m1 = _mm256_div_ps(m1, m2);
	m2 = _mm256_sub_ps(H2H, H2L);	m4 = _mm256_mul_ps(m1, m2);
	//(T1 * 2 / ds1) / (1 + T1)*(H2H - H2L);
	k1 = _mm256_add_ps(m3, m4);
	//k1 = k1*(1 - T1) / (1 + T1) + (T1 * 2 / ds1) / (1 + T1)*(H2H - H2L);
	ra = _mm256_set1_ps(1.0);
	m1 = _mm256_sub_ps(ra, T2);		m2 = _mm256_add_ps(ra, T2);
	m3 = _mm256_div_ps(m1, m2);		m3 = _mm256_mul_ps(k2, m3);
	//k2*(1 - T2) / (1 + T2)
	ra = _mm256_set1_ps(2.0);		m1 = _mm256_mul_ps(T2, ra);
	m1 = _mm256_div_ps(m1, ds2);	m1 = _mm256_div_ps(m1, m2);
	m2 = _mm256_sub_ps(H1H, H1L);	m4 = _mm256_mul_ps(m1, m2);
	//(T2 * 2 / ds2) / (1 + T2)*(H1H - H1L);
	k2 = _mm256_add_ps(m3, m4);
	//k2 = k2*(1 - T2) / (1 + T2) + (T2 * 2 / ds2) / (1 + T2)*(H1H - H1L);

	m1 = _mm256_sub_ps(H1H, H1L);	m1 = _mm256_div_ps(m1, ds2);
	m2 = _mm256_sub_ps(H2H, H2L);	m2 = _mm256_div_ps(m2, ds1);
	m3 = _mm256_sub_ps(m1, m2);		m3 = _mm256_mul_ps(m3, dti);
	//dt*((H1H - H1L) / ds2 - (H2H - H2L) / ds1)
	ra = _mm256_set1_ps(0.5);
	m1 = _mm256_mul_ps(dti, ra);	m2 = _mm256_add_ps(k1, kt1);
	m2 = _mm256_sub_ps(m2, k2);		m2 = _mm256_sub_ps(m2, kt2);
	m4 = _mm256_mul_ps(m1, m2);
	//+ dt*0.5*(k1 + kt1 - k2 - kt2);
	D = _mm256_add_ps(D, m3);		D = _mm256_add_ps(D, m4);
	//D = D + dt*((H1H - H1L) / ds2 - (H2H - H2L) / ds1) + dt*0.5*(k1 + kt1 - k2 - kt2);

}
float CFDTD::Timesourcep(float time, int init){

	float value = 0;
	register int it;
	register float t;

	float dt10 = dt/10;
	float tlen = tao*0.8;
	int Ntlen = 4;

	if(init == 1){

		Nt=floor((tlen*Ntlen+ObliqueShift)/dt10);
		timeline = Allocate_1D(timeline,Nt+5);
		for(it=0;it<=Nt+4;it++){
			t = (it-0.5)*dt10;
			timeline[it] = sin(ww*t);
			timeline[it] = timeline[it] * exp(-4*PIf*(t-tlen*Ntlen*0.5-ObliqueShift)*(t-tlen*Ntlen*0.5-ObliqueShift)/tlen/tlen);
		}
		//cout<<Nt<<endl;
		
		fstream timel;
		timel.open("./timeline_1.txt",ios::out);

		for(it=0;it<=Nt+4;it++){
			timel<<it<<" "<<(it-0.5)*dt10<<" "<<timeline[it]<<endl;
		}

		timel.close();
		
	}
	else{
		if( (time>0) && (time<tlen*Ntlen+ObliqueShift)){

			register float a,b,c;
			it = floor(time/dt10 +0.5);
			//cout<<Nt<<" "<<tlen<<" "<<time<<" "<<dt4<<" "<<it<<endl;
			c = time - (it-0.5)*dt10;
			c = c/dt10;
			a = timeline[it];
			b = timeline[it+1];
			value = a + (b-a)*c;
		}
		else{
			value = 0;
		} 
	}
	return value;
}
float CFDTD::Timesource(float time, int flag) {//Single Frequency flag 1 cos, flag 2 sin
	float value;
	float bar;
	float coe;
	bar = 100 * dt;
	if(timemode == 0){
		if (time < bar) {
			coe = exp(-4.6*(time - bar)*(time - bar) / (bar*bar)); //-20dB in Field Start
		}
		else coe = 1;
		if (flag == 1) {	// cos
			value = cos(time*ww)*coe;
		}
		else if (flag == 2) {	// sin
			value = sin(time*ww)*coe;
		}

	}
	return value;
}