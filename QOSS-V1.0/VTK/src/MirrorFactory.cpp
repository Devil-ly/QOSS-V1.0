#include "../include/MirrorFactory.h"
#include "../include/PlaneMirror.h"

Mirror * MirrorFactory::getMirror(MirrorsType type, const GraphTrans & graphTrans, 
	const std::vector<double> parameter)
{
	Mirror* res = nullptr;
	switch (type)
	{
	case PLANEMIRROR:
		res = new PlaneMirror(graphTrans, parameter);
		break;
	default:
		break;
	}
	return res;
}

Mirror * MirrorFactory::getMirror(MirrorsType type, const GraphTrans & graphTrans)
{
	Mirror* res = nullptr;
	switch (type)
	{
	case PLANEMIRROR:
		res = new PlaneMirror(graphTrans);
		break;
	default:
		break;
	}
	return res;
}

Mirror * MirrorFactory::getDefaultMirror(const GraphTrans & graphTrans)
{
	return getMirror(PLANEMIRROR, graphTrans);
}
