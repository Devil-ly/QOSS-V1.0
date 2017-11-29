/*
*	created by liyun 2017/11/29
*   function µÍ½×·øÉäÆ÷
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

private:

};


#endif // LOWORDORRADIATOR_H