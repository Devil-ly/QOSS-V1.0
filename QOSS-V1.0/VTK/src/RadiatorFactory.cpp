#include "..\include\RadiatorFactory.h"
#include "..\include\LowOrderRadiator.h"

std::shared_ptr<Radiator> RadiatorFactory::getRadiator(RadiatorType type,
	shared_ptr<calculation::SourceModeGeneration> source)
{
	std::shared_ptr<Radiator> res;
	switch (type)
	{
	case LOWORDER:
		res = std::make_shared<LowOrderRadiator>(source);
		break;
	default:
		break;
	}
	return res;
}

