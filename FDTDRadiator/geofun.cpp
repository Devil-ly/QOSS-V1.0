#include "geofun.h"

float Phi(float x, float y){
	float phi;
	if(x * y == 0){
		if(x == 0 && y==0) phi = 0;
		else if(x == 0){
			if(y >0) phi = PIf/2;
			else phi = PIf*3/2;
		}
		else if(y == 0){
			if(x >0) phi = 0;
			else phi = PIf;
		}
	}
	else{
		if(x>0){
			if(y>0) phi = atan(y/x);
			else phi = atan(y/x) + 2*PIf;
		}
		if(x<0){
			phi = atan(y/x) + PIf;
		}
	}
	return phi;
}
float Fxy(float x, float y, float l){
	float z;
	float phi = Phi(x,y);
	z = l * phi/(2*PIf);
	if(phi<0) cout<<"error"<<endl;
	return z;
}
int PECStructure(float x, float y, float z){
	int PEC;
	float rr;
	float thick = 1.0e-3;
	float top;
	rr = sqrt(x*x+y*y);
	top = Fxy(x,y,length);
	
	if( rr>radius && rr<radius+thick && z<top) PEC = 0;
	else PEC = 1;
	/*
	if( rr>radius && rr<radius+thick){
		if(Phi(x,y)<PI/180*123){
			if(z<0) PEC = 0;
			else PEC = 1;

		}
		else{ 
			if(z<length) PEC = 0;
			else PEC = 1;			
		}
	}
	else PEC = 1;
	*/	
	return PEC;
}