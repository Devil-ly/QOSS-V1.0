/*
*	created by liyun 2017/12/28
*   function 生成高斯口面场
*	width --> data[0]  depth --> data[1] 
*	z0    --> data[2]  w0    --> data[3] 
*	fre   --> data[4]  ds    --> data[5]
*   version 1.0
*/

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <string>
#include <complex>
#include <vector>
#include <QString>

#include "Field.h"


using namespace std;

class Gaussain : public Field
{
public:
	Gaussain(const GraphTrans & _graphTrans,
		const std::vector<double> parameter);

	Gaussain(const Gaussain & _Gaussain);
	//Gaussain(int type, float * _data);
	//Gaussain & operator =(const Gaussain & right);
	~Gaussain();

	void setUnit(double _unit);

	void updateData();

	virtual QTreeWidgetItem* getTree();

private:

	void CreateGaussianGaussain();
	void Gaussain::CreateGaussian(double r, double w0, double fre, double z0,
		double &Real, double &Imag) const;
	
	double factor;
	
};




#endif