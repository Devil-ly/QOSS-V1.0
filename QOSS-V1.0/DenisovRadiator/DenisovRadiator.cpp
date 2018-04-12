#include "DenisovRadiator.h"
#include "CodeJin/OMTfunctions.h"
#include "CodeJin/Mathematical_Functions_Jin.h"
#include "CodeJin/SourceModeGenerationD.h"	//modified class for Denisov Design
#include "CodeJin/eigen/Eigen/Core"
#include "CodeJin/eigen/Eigen/Dense"
#include "CodeJin/eigen/Eigen/src/Core/Array.h"
#include <fstream>

using namespace Eigen;
using namespace calculation;
using namespace std;

DenisovRadiator::DenisovRadiator()
{
	frequency = 140e9;
	radius = 16.8e-3;
	m = 22;
	n = 6;
	Denisovlog.open("./Denisov.log",ios::out);
}

DenisovRadiator::~DenisovRadiator()
{
	Denisovlog.close();
}

void DenisovRadiator::SetupDenisov(double _freq, double _radius, int _m, int _n) 
{	
	frequency = _freq;
	radius = _radius;
	m = _m;
	n = _n;
	//m = 6; n = 2;
	Denisovlog << "Denisov Basic Parameters Set" << endl;
	QString message = "Denisov Basic Paremeters Set";
	emit SendText(message);
	emit SendValue(100.0);
	
	Denisovlog << "Test for Root Finding" << endl;
	Denisovlog << m << "th order, " << n << "th zero points is " << rootbessel(m, n) << endl;
	Denisovlog << m << "th order, " << n << "th zero points of derivate is " << rootdbessel(m, n) << endl;

}

void DenisovRadiator::SetTangentialEField(int _Nx, int _Ny) {
	DenisovRadiator::Nx = _Nx;
	DenisovRadiator::Ny = _Ny;
	//Allocate Memory
	DenisovRadiator::Ex.resize(Nx);
	DenisovRadiator::Ey.resize(Nx);
	for (int i = 0; i < Nx; i++) {
		for (int j = 0; j < Ny; j++) {
			Ex[i].resize(Ny);
			Ey[i].resize(Ny);
		}
	}
}

void DenisovRadiator::SetSurfaceCurrent(int _Nphi, int _Sparse) {
	DenisovRadiator::Nphi = _Nphi;
	DenisovRadiator::Nsparse = _Sparse;
	DenisovRadiator::Nheight = DenisovRadiator::Nz / _Sparse;
	DenisovRadiator::J.resize(DenisovRadiator::Nphi);
	for (int p = 0; p < DenisovRadiator::Nphi; p++) {
		DenisovRadiator::J[p].resize(DenisovRadiator::Nheight);
	}
}

void DenisovRadiator::GetTangentialEField(vector<vector<complex<double>>> &_Ex, vector<vector<complex<double>>> &_Ey, int _Nx, int _Ny) {
	for (int i = 0; i < _Nx; i++) {
		for (int j = 0; j < _Ny; j++) {
			_Ex[i][j] = Ex[i][j];
			_Ey[i][j] = Ey[i][j];
		}
	}
}

void DenisovRadiator::GetSurfaceCurrent(vector<vector<double>> &_J, int _Nphi, int _Nheight) {
	for (int p = 0; p < _Nphi; p++) {
		for (int h = 0; h < _Nheight; h++) {
			_J[p][h] = J[p][h];
		}
	}
}

void DenisovRadiator::SetupDenisovParas(double _delBeta1, double _delBeta2, int _l1, int _l2, double _z0_1, double _z0_2, double _ls_1, double _ls_2, double _lc_1, double _lc_2, double _Hz, double _dz, int _Nz, double _mag1, double _mag2) {
	DenisovRadiator::delBeta1 = _delBeta1;
	DenisovRadiator::delBeta2 = _delBeta2;
	DenisovRadiator::l1 = _l1;
	DenisovRadiator::l2 = _l2;
	DenisovRadiator::z0_1 = _z0_1;
	DenisovRadiator::z0_2 = _z0_2;
	DenisovRadiator::ls_1 = _ls_1;
	DenisovRadiator::ls_2 = _ls_2;
	DenisovRadiator::lc_1 = _lc_1;
	DenisovRadiator::lc_2 = _lc_2;
	DenisovRadiator::Hz = _Hz;
	DenisovRadiator::dz = _dz;
	DenisovRadiator::Nz = _Nz;
	DenisovRadiator::mag1 = _mag1;
	DenisovRadiator::mag2 = _mag2;
	DenisovRadiator::Zz.resize(DenisovRadiator::Nz);	for (int i = 0; i < DenisovRadiator::Nz; i++) { DenisovRadiator::Zz[i] = (i + 0.5)*dz; }
	DenisovRadiator::sig1.resize(DenisovRadiator::Nz);	for (int i = 0; i < DenisovRadiator::Nz; i++) { DenisovRadiator::sig1[i] = sig(mag1, z0_1, lc_1, ls_1, Zz[i]); }
	DenisovRadiator::sig2.resize(DenisovRadiator::Nz);	for (int i = 0; i < DenisovRadiator::Nz; i++) { DenisovRadiator::sig2[i] = sig(mag2, z0_2, lc_2, ls_2, Zz[i]); }
	DenisovRadiator::dsig1.resize(DenisovRadiator::Nz);	for (int i = 0; i < DenisovRadiator::Nz; i++) { DenisovRadiator::dsig1[i] = dsig(mag1, z0_1, lc_1, ls_1, Zz[i]); }
	DenisovRadiator::dsig2.resize(DenisovRadiator::Nz);	for (int i = 0; i < DenisovRadiator::Nz; i++) { DenisovRadiator::dsig2[i] = dsig(mag2, z0_2, lc_2, ls_2, Zz[i]); }


}

void DenisovRadiator::GetDenisovTurbulence(vector<double>& _sig1, vector<double>& _sig2) {
	for (int i = 0; i < Nz; i++) {
		_sig1[i] = DenisovRadiator::sig1[i];
		_sig2[i] = DenisovRadiator::sig2[i];
	}
}

void DenisovRadiator::SetupModeField(int _Nx, int _Ny) {

}

void DenisovRadiator::run(){
	QString message = "Denisov Simulation is Runing";
	emit SendText(message);
	double ii = 0.0;
	emit SendValue(ii);
	emit SendBtnClose();

	double lambda;  lambda = C_Speed / DenisovRadiator::frequency;
	double k0;		k0 = 2 * Pi / lambda;
	double ww;		ww = 2 * Pi * DenisovRadiator::frequency;

	Coes9.resize(0);	Coes9.resize(10);

	//启动Denisov计算
	//申请各模式内存
	for (int i = 0; i < DenisovRadiator::Nx; i++) {
		for (int j = 0; j < DenisovRadiator::Ny; j++) {
			DenisovRadiator::Ex[i][j] = 0;
			DenisovRadiator::Ey[i][j] = 0;
		}
	}
	for (int p = 0; p < DenisovRadiator::Nphi; p++) {
		for (int h = 0; h < DenisovRadiator::Nheight; h++) {
			DenisovRadiator::J[p][h] = 0.0;
		}
	}

	//各个模式向量
	//Eigen::VectorXi MVec;
	//Eigen::VectorXi NVec;
	//Chi root of dbessel
	//Eigen::VectorXd ChiVec;
	//kp 横向波数  
	//Eigen::VectorXd KpVec;
	//kz 纵向波数
	//Eigen::VectorXd KzVec;

	//各个模式向量
	vector<int> MVec;
	vector<int> NVec;
	//Chi root of dbessel
	vector<double> ChiVec;
	//kp 横向波数  
	vector<double> KpVec;
	//kz 纵向波数
	vector<double> KzVec;
	//模式系数矩阵
	Eigen::VectorXcd CoeVec;
	//转换矩阵
	Eigen::MatrixXcd TransArray;
	//Store
	Eigen::ArrayXXcd StoreCoeVec;

	int SpanM;	int SpanN;
	if (DenisovRadiator::m == 22 && DenisovRadiator::n == 6) {
		SpanM = 15;
		SpanN = 9;
	}
	else if (DenisovRadiator::m == 28 && DenisovRadiator::n == 8) {
		SpanM = 15;
		SpanN = 9;
	}
	else if (DenisovRadiator::m == 6 && DenisovRadiator::n == 2) {
		SpanM = 9;
		SpanN = 3;
	}
	
	
	CoeVec.resize(SpanM*SpanN);
	ChiVec.resize(SpanM*SpanN);
	KpVec.resize(SpanM*SpanN);
	KzVec.resize(SpanM*SpanN);
	TransArray.resize(SpanM*SpanN, SpanM*SpanN);
	StoreCoeVec.resize(SpanM*SpanN, DenisovRadiator::Nz+1);

	//Set Up M.N Vector
	MVec.resize(SpanM*SpanN);
	NVec.resize(SpanM*SpanN);
	for (int mm = 0; mm < SpanM; mm++) {
		for (int nn = 0; nn < SpanN; nn++) {
			MVec[nn + mm*SpanN] = DenisovRadiator::m + mm - (SpanM - 1) / 2;
			NVec[nn + mm*SpanN] = DenisovRadiator::n + nn - (SpanN - 1) / 2;
		}
	}
	for (int i = 0; i < SpanM*SpanN; i++) {
		//Initialize CoeVec0
		if (MVec[i] == DenisovRadiator::m && NVec[i] == DenisovRadiator::n) {
			CoeVec[i] = 1.0;
		}
		else {
			CoeVec[i] = 0.0;
		}
		StoreCoeVec(i, 0) = CoeVec(i);
		//Initialize ChiVec
		ChiVec[i] = rootdbessel(MVec[i],NVec[i]);
		//Initialize KpVec
		KpVec[i] = ChiVec[i] / DenisovRadiator::radius;
		//Initialize KzVec
		if (k0*k0 - KpVec[i] * KpVec[i] > 0) {
			KzVec[i] = sqrt(k0*k0 - KpVec[i] * KpVec[i]);
		}
		else {
			KzVec[i] = 0.0;
		}


	}
	fstream Matrixout;

	for (int nn = 0; nn < Nz; nn++) {//Propagation Distance
		float zp = (nn + 0.5)*DenisovRadiator::dz;
		int rr, cc;
		TransArray.setZero();

		//Fill TransArray
		for (rr = 0; rr < SpanM*SpanN; rr++) {	//row
			int mr = MVec[rr];	//m0 in Matlab
			int nr = NVec[rr];	//n0 in Matlab
			//TransArray(ir, ir) = -j*kkzmn_alfa(m0, n0)*exp(-j*kkzmn_alfa(m0, n0)*dz / 2);
			double middle = (-1.0*0.5)*KzVec[rr] * DenisovRadiator::dz;
			TransArray(rr, rr) = (complex<double>(0.0, -1.0)) * KzVec[rr] * exp(complex<double>(0.0, middle));
			
			complex<double> see = TransArray(rr, rr)*DenisovRadiator::dz;
			see = see;
			
			double sig1a = DenisovRadiator::sig1[nn] / DenisovRadiator::radius;
			double dsig1a = DenisovRadiator::dsig1[nn] / DenisovRadiator::radius;
			double sig2a = DenisovRadiator::sig2[nn] / DenisovRadiator::radius;
			double dsig2a = DenisovRadiator::dsig2[nn] / DenisovRadiator::radius;
			double freq = DenisovRadiator::frequency;
			double ra = DenisovRadiator::radius;
			double db1 = DenisovRadiator::delBeta1;
			double db2 = DenisovRadiator::delBeta2;
			int ll1 = DenisovRadiator::l1;
			int ll2 = DenisovRadiator::l2;
			double kzr = KzVec[rr];
			double Chir = ChiVec[rr];

			for (cc = 0; cc < SpanM*SpanN; cc++) {	//column
				int mc = MVec[cc];	//m1 in Matlab
				int nc = NVec[cc];	//m1 in Matlab
				complex < double > Temp;


				double kzc = KzVec[cc];

				double Chic = ChiVec[cc];

				Temp = CQR(freq, ra, db1, db2, ll1, ll2, zp, sig1a, dsig2a, sig2a, dsig2a, kzr,kzc,Chir,Chic,mr,mc);
				//Temp = CQR(freq,ra,db1,db2,ll1,ll2,)
				Temp = Temp * exp(complex<double>(0.0, -1.0*kzc * DenisovRadiator::dz*0.5));
				TransArray(rr, cc) = TransArray(rr, cc) + Temp;
				Temp = Temp;
			}//cc
			see = see;
		}//rr
		/*
		Matrixout.open("./Matrixout.txt", ios::out);
		for (int rr = 0; rr < SpanM*SpanN; rr++) {
			for (int cc = 0; cc < SpanM*SpanN; cc++) {
				Matrixout << TransArray(rr, cc) <<" ";
			}
			Matrixout << endl;
		}
		Matrixout.close();

	
		Matrixout.open("./CoeBefore.txt", ios::out);
		
		for (int rr = 0; rr < SpanM*SpanN; rr++) {
			Matrixout << CoeVec(rr) << endl;
		}
		Matrixout.close();

		*/
		CoeVec = CoeVec + (TransArray*CoeVec)*DenisovRadiator::dz;
/*
		Matrixout.open("./CoeAfter.txt", ios::out);
		for (int rr = 0; rr < SpanM*SpanN; rr++) {
			Matrixout << CoeVec(rr) << endl;
		}
		Matrixout.close();
		*/
		for (int rr = 0; rr < SpanM*SpanN; rr++) {
			StoreCoeVec(rr, nn + 1) = CoeVec(rr);
		}
		

		double temp;
		//Take Coefficients
		int midN = (SpanN - 1) / 2;
		int midM = (SpanM - 1) / 2;
		if (DenisovRadiator::m == 22 && DenisovRadiator::n == 6) {
			//11 TE m-2 n+1 22 6 - 20 7	%TE m - 2 n + 1  22 6 - 20 7
			DenisovRadiator::Coes9[0] = pow(abs(CoeVec(midN + 1 + (midM - 2)*SpanN)), 2);
			//12 TE m+1 n 22 6 - 23 6	%TE m + 1 n  22 6 - 23 6
			DenisovRadiator::Coes9[1] = pow(abs(CoeVec(midN + 0 + (midM + 1)*SpanN)), 2);
			//13 TE m+4 n-1 22 6 - 26 5	%TE m + 4 n - 1 22 6 - 26 5
			DenisovRadiator::Coes9[2] = pow(abs(CoeVec(midN - 1 + (midM + 4)*SpanN)), 2);
			//21 TE m-3 n+1 22 6 - 19 7	%TE m - 3 n + 1  22 6 - 19 7
			DenisovRadiator::Coes9[3] = pow(abs(CoeVec(midN + 1 + (midM - 3)*SpanN)), 2);
			//22 TE m n 22 6 - 22 6		%TE m n    22 6 - 22 6
			DenisovRadiator::Coes9[4] = pow(abs(CoeVec(midN + 0 + (midM + 0)*SpanN)), 2);
			//temp = abs(CoeVec[midN + 0 + (midM)*SpanN]); temp = temp*temp;
			//DenisovRadiator::Coes9[4] = temp;
			//23 TE m+3 n-1 22 6 - 25 5	%TE m + 3 n - 1   22 6 - 25 5
			DenisovRadiator::Coes9[5] = pow(abs(CoeVec(midN - 1 + (midM + 3)*SpanN)), 2);
			//31 TE m-4 n+1 22 6 - 18 7	%TE m - 4 n + 1  22 6 - 18 7
			DenisovRadiator::Coes9[6] = pow(abs(CoeVec(midN + 1 + (midM - 4)*SpanN)), 2);
			//32 TE m-1 n 22 6 - 21 6	%TE m - 1 n  22 6 - 21 6
			DenisovRadiator::Coes9[7] = pow(abs(CoeVec(midN + 0 + (midM - 1)*SpanN)), 2);
			//33 TE m+2 n-1 22 6 - 24 5	%TE m + 2 n - 1 22 6 - 24 5
			DenisovRadiator::Coes9[8] = pow(abs(CoeVec(midN - 1 + (midM + 2)*SpanN)), 2);

			DenisovRadiator::Coes9[9] = CoeVec.cwiseAbs2().sum();
		}
		//把计算得到的各卫星模式的功率比率发回界面
		//total main neighbor, corner
		double CoeTotal = Coes9[9];
		double CoeMain = Coes9[4];
		double CoeNeighbor = Coes9[1] + Coes9[3] + Coes9[5] + Coes9[7];
		double CoeCorner = Coes9[0] + Coes9[2] + Coes9[6] + Coes9[8];
		emit SendCoefficients(CoeTotal, CoeMain, CoeNeighbor, CoeCorner, nn);


		if ((nn + 1) % 30 == 0) { 
			
			
			for (int rr = 0; rr < SpanM*SpanN; rr++) {
				if (abs(CoeVec(rr))*abs(CoeVec(rr)) > 0.01) {
					int mr = MVec[rr];
					int nr = NVec[rr];
					SourceModeGenerationD Source(2, 1, 2, mr, nr, DenisovRadiator::frequency, DenisovRadiator::radius,0, 0, DenisovRadiator::Nx);
					//2, 1, 2, m, n, frequency, radius, 0, 0, Nx
					//Source.SetSource_Circular(2,1,2,mr,nr,DenisovRadiator::frequency,DenisovRadiator::radius);
					//Source.SetOutputProperty(Nx);
					Source.FieldCalculation_Circular();
					

					vector<vector<complex<double>>> TEx;
					vector<vector<complex<double>>> TEy;
					TEx.resize(Nx);	TEy.resize(Ny);
					for (int i = 0; i < Nx; i++) {
						TEx[i].resize(Ny);
						TEy[i].resize(Nz);
					}

					Source.GetEX(TEx); Source.GetEY(TEy);

					for (int i = 0; i < Nx; i++) {
						for (int j = 0; j < Ny; j++) {
							Ex[i][j] += TEx[i][j] * CoeVec[rr];
							Ey[i][j] += TEy[i][j] * CoeVec[rr];
						}
					}
				}//if
			}//rr
			for (int i = 0; i < Nx; i++) {
				for (int j = 0; j < Ny; j++) {
					//Ex[i][j] = sqrt(Ex[i][j].real()*Ex[i][j].real()+ Ex[i][j].imag()*Ex[i][j].imag());
					//Ey[i][j] = sqrt(Ey[i][j].real()*Ey[i][j].real() + Ey[i][j].imag()*Ey[i][j].imag());
				}
			}
			
			//finished add
			//emit SendTangentialEField(Ex, Ey, Nx, Ny);
			emit SendFieldUpdate();

			emit SendValue(double(nn*1.0 / Nz * 100));
		}
	}//nn

	//emit SendValue(double(Max));
	message = "Denisov Simulation is Done";
	emit SendText(message);
	//for (int p = 0; p < DenisovRadiator::Nphi; p++) {
		//for (int h = 0; h < DenisovRadiator::Nheight; h++) {
			//DenisovRadiator::J[p][h] = h - p*2;
		//}
	//}
	message = "Generating Surface Currents";
	emit SendText(message);

	for(int h = 0; h<DenisovRadiator::Nheight;h++){//Height
		vector<complex<double>> HPhi_r;	HPhi_r.resize(DenisovRadiator::Nphi);
		vector<complex<double>> HZ_r;	HZ_r.resize(DenisovRadiator::Nphi);
		for (int p = 0; p < DenisovRadiator::Nphi; p++) {
			HPhi_r[p] = complex<double>(0.0, 0.0);
			HZ_r[p] = complex<double>(0.0, 0.0);
		}
		int hh = h*DenisovRadiator::Nsparse;
		for (int rr = 0; rr < SpanM*SpanN; rr++) {//Mode
			complex<double> Coe = StoreCoeVec(rr, hh);
			if (abs(CoeVec(rr))*abs(CoeVec(rr)) > 0.01) {//Judge
				int mr = MVec[rr];
				int nr = NVec[rr];
				SourceModeGenerationD Source(2, 1, 2, mr, nr, DenisovRadiator::frequency, DenisovRadiator::radius, 0, 0, DenisovRadiator::Nx);

				vector<complex<double>> THPhi;
				vector<complex<double>> THz;
				THPhi.resize(DenisovRadiator::Nphi);	THz.resize(DenisovRadiator::Nphi);

				Source.GetJ_R(THPhi,THz,DenisovRadiator::Nphi);

				for (int p = 0; p < DenisovRadiator::Nphi; p++) {
					HPhi_r[p] += THPhi[p] * Coe;
					HZ_r[p] += THz[p] * Coe;
				}
			}//if
		}//rr
		for (int p = 0; p < Nphi; p++) {
			DenisovRadiator::J[p][h] = sqrt(HPhi_r[p].real()*HPhi_r[p].real() + HPhi_r[p].imag()*HPhi_r[p].imag() + HZ_r[p].real()*HZ_r[p].real() + HZ_r[p].imag()*HZ_r[p].imag());
		}
		emit SendValue(double(h*1.0 / DenisovRadiator::Nheight * 100));
	}//for h
	//Send SurfaceCurrent
	emit SendCurrentUpdate();

	emit SendBtnOpen();
	emit SendValue(ii);
}
#include "moc_DenisovRadiator.cpp"