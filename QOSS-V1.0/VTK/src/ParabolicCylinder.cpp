#include "..\include\ParabolicCylinder.h"

ParabolicCylinder::ParabolicCylinder(const GraphTrans & _graphTrans)
{
	type = PARABOLICCYLINDER;
	graphTrans = _graphTrans;
	data.resize(16);
	for (auto & x : data)
		x = 0;
	setParameter(0.02, 0.04, 0, 0.2);
	updateData();
}

ParabolicCylinder::ParabolicCylinder(const GraphTrans & _graphTrans, 
	const std::vector<double>& parameter)
{
	type = PARABOLICCYLINDER;
	graphTrans = _graphTrans;
	this->focus = parameter[0];
	this->yMax = parameter[1];
	this->zMin = parameter[2];
	this->zMax = parameter[3];
	data.resize(16);
	for (auto & x : data)
		x = 0;
	data[1] = 1;
	data[6] = -4 * focus;
	data[11] = yMax * yMax / focus;
	data[12] = -yMax;
	data[13] = yMax;
	data[14] = zMin;
	data[15] = zMax;
	updateData();
}

void ParabolicCylinder::setParameter(double focus, double yMax, double zMin, double zMax)
{
	this->focus = focus;
	this->yMax = yMax;
	this->zMin = zMin;
	this->zMax = zMax;
	data[1] = 1;
	data[6] = -4 * focus;
	data[11] = yMax * yMax / focus;
	data[12] = -yMax;
	data[13] = yMax;
	data[14] = zMin;
	data[15] = zMax;
}
