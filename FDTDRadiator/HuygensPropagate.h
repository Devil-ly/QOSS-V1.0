#pragma once
//Propagation from HuygensAperture to Arbitrary Aperture
using namespace::std;

#ifndef HUYGENSPROPAGATE_H  // È«´óÐ´
#define HUYGENSPROPAGATE_H
class CHuygens {
public:
	CHuygens(void);
	~CHuygens(void);
private:

public:
	int Mode;
	//0: One Aperture //1: Five Aperture
	float freq;
	float Wx;
	float Wy;
	float Wz;
	int Nx_DFT;
	int Ny_DFT;
	int Nz_DFT;
	float dx;
	float dy;
	float dz;

private:

public:
	Initial(void);
	

};


#endif