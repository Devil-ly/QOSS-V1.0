/*
*	created by liyun 2017/11/29
*   function 低阶辐射器
*   version 1.0
*/

#ifndef LOWORDORRADIATOR_H  
#define LOWORDORRADIATOR_H

#include "Radiator.h"

class LowOrderRadiator : public Radiator
{
public:

	LowOrderRadiator(shared_ptr<calculation::SourceModeGeneration>);
	virtual ~LowOrderRadiator();

	virtual void calActorModel();

	virtual void calActorRay();

	virtual double getFirstMirrorHeight(double x);

private:
	
	// x^2 + y^2 = R^2 与 射线 startPoint + t * startVector 求交点
	bool getIntersectionOfCircleAndRay(double radius, const Vector3 & startPoint,
		const Vector3 & startVector, double & t);


};


#endif // LOWORDORRADIATOR_H