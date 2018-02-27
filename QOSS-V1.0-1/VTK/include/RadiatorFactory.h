/*
*	created by liyun 2017/11/29
*   function 生产各种辐射器的类
*   version 1.0
*/

#ifndef RADIATORFACTORY_H  
#define RADIATORFACTORY_H

#include "Radiator.h"

#include <vector>
#include <memory>

class RadiatorFactory
{
public:

	static std::shared_ptr<Radiator> getRadiator(RadiatorType type, 
		shared_ptr<calculation::SourceModeGeneration>);

private:
	RadiatorFactory() {};
	~RadiatorFactory() {};

};



#endif // RADIATORFACTORY_H
