#include "../include/MirrorFactory.h"
#include "../include/PlaneMirror.h"
#include "../include/QuadricSurfaceMirror.h"
#include "../include/ParabolicCylinder.h"
#include "../include/Paraboloid.h"
#include "../include/Ellipsoid.h"

Mirror * MirrorFactory::getMirror(MirrorsType type, const GraphTrans & graphTrans, 
	const std::vector<double>& parameter)
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
	case PARABOLICCYLINDER:
		res = new ParabolicCylinder(graphTrans, parameter);
		break;
	case PARABOLOID:
		res = new Paraboloid(graphTrans, parameter);
		break;
	case ELLIPSOID:
		res = new Ellipsoid(graphTrans, parameter);
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
	case PARABOLICCYLINDER:
		res = new ParabolicCylinder(graphTrans);
		break;
	case PARABOLOID:
		res = new Paraboloid(graphTrans);
		break;
	case ELLIPSOID:
		res = new Ellipsoid(graphTrans);
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
