#pragma once

#include<complex>
#include<cmath>
#include"../util/Constant_Var.h"


using namespace std;

double sig(double _mag, double _z0, double _lc, double _ls, double _z) {
	double value;
	//注意！ 这里是 radius*eps(z) 不是eps(z);
	if (_z >= _z0 - _lc*0.5 - _ls && _z <  _z0 - _lc*0.5) {
		value = (_z - (_z0 - _lc*0.5 - _ls)) / _ls*_mag;
	}
	else if (_z >= _z0 - _lc*0.5 && _z < _z0 + _lc*0.5) {
		value = _mag;
	}
	else if (_z >= _z0 + _lc*0.5 && _z < _z0 + _lc*0.5 + _ls) {
		value = (_z0 + _lc*0.5 + _ls - _z) / _ls*_mag;
	}
	else value = 0;
	/*
	if (Tx[i]>=zc1 - lc1*0.5 - ls1 && Tx[i]< zc1-lc1*0.5) {
	T1y[i] = (Tx[i] - (zc1 - lc1*0.5 - ls1)) / ls1*mag1;
	}
	else if (Tx[i] >= zc1 - lc1*0.5 && Tx[i] < zc1 + lc1*0.5) {
	T1y[i] = mag1;
	}
	else if (Tx[i] >= zc1 + lc1*0.5 && Tx[i] < zc1 + lc1*0.5 + ls1) {
	T1y[i] = (zc1 + lc1*0.5 + ls1 - Tx[i]) / ls1*mag1;
	}
	else T1y[i] = 0;
	*/
	return value;
}
double dsig(double _mag, double _z0, double _lc, double _ls, double _z) {
	double value;
	//注意！ 这里是 radius*eps(z) 不是eps(z);
	if (_z >= _z0 - _lc*0.5 - _ls && _z <  _z0 - _lc*0.5) {//上升沿
		value = _mag/_ls;
	}
	else if (_z >= _z0 - _lc*0.5 && _z < _z0 + _lc*0.5) {
		value = 0;
	}
	else if (_z >= _z0 + _lc*0.5 && _z < _z0 + _lc*0.5 + _ls) {//下降沿
		value = -_mag/_ls;
	}
	else value = 0;
	/*
	if (Tx[i]>=zc1 - lc1*0.5 - ls1 && Tx[i]< zc1-lc1*0.5) {
	T1y[i] = (Tx[i] - (zc1 - lc1*0.5 - ls1)) / ls1*mag1;
	}
	else if (Tx[i] >= zc1 - lc1*0.5 && Tx[i] < zc1 + lc1*0.5) {
	T1y[i] = mag1;
	}
	else if (Tx[i] >= zc1 + lc1*0.5 && Tx[i] < zc1 + lc1*0.5 + ls1) {
	T1y[i] = (zc1 + lc1*0.5 + ls1 - Tx[i]) / ls1*mag1;
	}
	else T1y[i] = 0;
	*/
	return value;
}

complex<double> GAMMA0(double _beta1, double _beta2, int _l1, int _l2, int _m1, int _m2, double _z, double _dis1, double _dis2) {
	complex<double> gamma0(0.0, 0.0);
	if (_l1 != 0 && _l2 != 0) {
		if (_m1 - _m2 == -_l1) {
			gamma0 = (Pi*_dis1)*exp(complex<double>(0.0, -_beta1*_z));
		}
		else if (_m1 - _m2 == _l1) {
			gamma0 = (Pi*_dis1)*exp(complex<double>(0.0, _beta1*_z));
		}
		else if (_m1 - _m2 == -_l2) {
			gamma0 = (Pi*_dis2)*exp(complex<double>(0.0, -_beta2*_z));
		}
		else if (_m1 - _m2 == _l2) {
			gamma0 = (Pi*_dis2)*exp(complex<double>(0.0, _beta2*_z));
		}
		else {
			//do Nothing
		}
	}
	else {
		if (_l1 == 0 && _l2 != 0) {
			if (_m1 - _m2 == 0) {
				gamma0 = 2 * Pi* _dis1 * cos(_beta1*_z);
			}
			else if (_m1 - _m2 == -_l2) {
				gamma0 = (Pi*_dis2)*exp(complex<double>(0.0, -_beta2*_z));
			}
			else if (_m1 - _m2 == _l2) {
				gamma0 = (Pi*_dis2)*exp(complex<double>(0.0, _beta2*_z));
			}
		}
		else if (_l1 != 0 && _l2 == 0) {
			if (_m1 - _m2 == 0) {
				gamma0 = 2 * Pi* _dis2 * cos(_beta2*_z);
			}
			else if (_m1 - _m2 == -_l1) {
				gamma0 = (Pi*_dis1)*exp(complex<double>(0.0, -_beta1*_z));
			}
			else if (_m1 - _m2 == _l1) {
				gamma0 = (Pi*_dis1)*exp(complex<double>(0.0, _beta1*_z));
			}
		}
		else {
			//do Nothing
		}
	}

	return gamma0;
}

complex<double> GAMMAZ(double _beta1, double _beta2, int _l1, int _l2, int _m1, int _m2, double _z, double _a, double _dis1, double _ddis1, double _dis2, double _ddis2) {
	complex<double> gammaz(0.0, 0.0);
	complex<double> imag(0.0, 1.0);
	if (_l1 != 0 && _l2 != 0) {
		if (_m1 - _m2 == _l1) {
			gammaz = (_a*Pi)*(_ddis1 + imag*_beta1*_dis1)*exp(imag*(_beta1*_z));
		}
		else if (_m1 - _m2 == -_l1) {
			gammaz = (_a*Pi)*(_ddis1 - imag*_beta1*_dis1)*exp(imag*(-_beta1*_z));
		}
		else if (_m1 - _m2 == _l2) {
			gammaz = (_a*Pi)*(_ddis2 + imag*_beta2*_dis2)*exp(complex<double>(0, _beta2*_z));
		}

		else if (_m1 - _m2 == -_l2) {
			gammaz = (_a*Pi)*(_ddis2 - imag*_beta2*_dis2)*exp(complex<double>(0, -_beta2*_z));
		}
		else {
			//do Nothing
		}
	}
	else{
		if (_l1 == 0 && _l2 != 0) {
			if (_m1 - _m2 == 0) {
				gammaz = (2 * Pi* _a)*(_ddis1 * cos(_beta1*_z)-_beta1*_dis1*sin(_beta1*_z));
			}
			else if (_m1 - _m2 == _l2) {
				gammaz = (_a*Pi)*(_ddis2 + imag*_beta2*_dis2)*exp(complex<double>(0, _beta2*_z));
			}
			else if (_m1 - _m2 == -_l2) {
				gammaz = (_a*Pi)*(_ddis2 - imag*_beta2*_dis2)*exp(complex<double>(0, -_beta2*_z));
			}
		}
		if (_l1 != 0 && _l2 == 0) {
			if (_m1 - _m2 == 0) {
				gammaz = (2 * Pi* _a)*(_ddis2 * cos(_beta2*_z) - _beta2*_dis2*sin(_beta2*_z));
			}
			else if (_m1 - _m2 == _l1) {
				gammaz = (_a*Pi)*(_ddis1 + imag*_beta1*_dis1)*exp(complex<double>(0, _beta1*_z));
			}
			else if (_m1 - _m2 == -_l1) {
				gammaz = (_a*Pi)*(_ddis1 - imag*_beta1*_dis1)*exp(complex<double>(0, -_beta1*_z));
			}
		}
		else {
		}
	}
	return gammaz;
}	

complex<double> CQR(double freq,double _a, double _beta1, double _beta2, int _l1, int _l2,  double _z, double _dis1, double _ddis1,double _dis2, double _ddis2, double _kz1, double _kz2,  double _Chi1, double _Chi2, int _m1, int _m2) {
	complex<double> CQR(0.0, 0.0);
	complex<double> Q(0.0, 0.0);
	complex<double> R(0.0, 0.0);
	complex<double> imag(0.0, 1.0);
	double ww = 2 * Pi*freq;
	Q = imag*_Chi2*_Chi2*(_m1*_m2 - _Chi1*_Chi1) / (_a*_a*Pi*sqrt(_Chi1*_Chi1 - _m1*_m1)*sqrt(_Chi2*_Chi2 - _m2*_m2));
	Q = Q*GAMMA0(_beta1, _beta2, _l1, _l2, _m1, _m2, _z, _dis1, _dis2);
	R = _m1*_m2 / (_a*Pi*sqrt(_Chi1*_Chi1 - _m1*_m1)*sqrt(_Chi2*_Chi2 - _m2*_m2));
	R = R*GAMMAZ(_beta1,_beta2,_l1,_l2,_m1,_m2,_z,_a,_dis1,_ddis1,_dis2,_ddis2);
	if (_kz1*_kz2 != 0) {
		//CQR = 0.5*((ww*Mu0) / sqrt(_kz1*_kz2) * Q + sqrt(_kz2 / _kz1) * R);
		CQR = 0.5*(1.0 / sqrt(_kz1*_kz2) * Q + sqrt(_kz2 / _kz1) * R);
	}
	

	return CQR;
}




