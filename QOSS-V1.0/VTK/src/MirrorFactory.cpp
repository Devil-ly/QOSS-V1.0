#include "../include/MirrorFactory.h"
#include "../include/PlaneMirror.h"
#include "../include/QuadricSurfaceMirror.h"


Mirror * MirrorFactory::getMirror(MirrorsType type, const GraphTrans & graphTrans, 
	const std::vector<double> parameter)
{
	Mirror* res = nullptr;
	switch (type)
	{
	case PLANEMIRROR:
		res = new PlaneMirror(graphTrans, parameter);
		break;
	case QUADRICSURFACE:
		res = new QuadricSurfaceMirror(graphTrans, parameter);
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
	case QUADRICSURFACE:
		res = new QuadricSurfaceMirror(graphTrans);
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
