#include "../include/MirrorFactory.h"
#include "../include/PlaneMirror.h"
#include "../include/QuadricSurfaceMirror.h"
#include "../include/ParabolicCylinder.h"
#include "../include/Paraboloid.h"
#include "../include/Ellipsoid.h"
#include "../include/STLMirror.h"
#include "../include/PhsCorMirror.h"
#include "../include/Restriction.h"

#include "util/comUtil.h"

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
	case STLMIRROR:
		res = new STLMirror(graphTrans);
		break;
	default:
		break;
	}
	return res;
}

Mirror * MirrorFactory::cloneMirror(Mirror * mirror)
{
	Mirror* res = nullptr;
	GraphTrans graphTrans = mirror->getGraphTrans();
	const std::vector<double>& parameter = mirror->getData();

	return getMirror(mirror->getMirrorsType(), graphTrans, parameter);
}

Mirror * MirrorFactory::getDefaultMirror(const GraphTrans & graphTrans)
{
	return getMirror(PLANEMIRROR, graphTrans);
}

Mirror * MirrorFactory::getMirrorByJson(const Json::Value & js)
{
	if (!js.isMember("type") ||
		!js.isMember("Data") ||
		!js.isMember("graphTrans") ||
		!js.isMember("isTransparent") ||
		!js.isMember("isShow"))
	{
		return nullptr;
	}
	Mirror* res = nullptr;
	std::vector<double> parameter(js["Data"].size());
	for (int i = 0; i < js["Data"].size(); i++)
	{
		parameter[i] = js["Data"][i].asDouble();
	}

	GraphTrans graphTrans;
	if (!parseJsonToGraphTrans(js["graphTrans"], graphTrans))
		return nullptr;

	switch (js["type"].asInt())
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
	case STLMIRROR:
		if (!js.isMember("path"))
		{
			return nullptr;
		}
		res = new STLMirror(graphTrans, parameter, js["path"].asString());
		break;
	case PHSCORMIRROR:
		if (!js.isMember("path") || !js.isMember("N"))
		{
			return nullptr;
		}
		res = new PhsCorMirror(graphTrans, parameter, js["path"].asString(), js["N"].asInt());
		break;
	default:
		break;
	}
	if (js.isMember("restrictionNum"))
	{
		int size = js["restrictionNum"].asInt();
		for (int i = 0; i < size; i++)
		{
			const Json::Value & jsRes = js["restriction"][i];
			if (!jsRes.isMember("Data") ||
				!jsRes.isMember("graphTrans"))
			{
				break;
			}

			std::vector<double> parameterRes(jsRes["Data"].size());
			for (int i = 0; i < jsRes["Data"].size(); i++)
			{
				parameterRes[i] = jsRes["Data"][i].asDouble();
			}

			GraphTrans graphTransRes;
			if (!parseJsonToGraphTrans(jsRes["graphTrans"], graphTransRes))
				return nullptr;
			Restriction * restriction = new Restriction(graphTransRes, parameterRes);
			res->addRestriction(restriction);
		}
		
	}
	if (res)
	{
		if (!js["isShow"].asBool())
			res->switchIsShow();
		if (js["isTransparent"].asBool())
			res->switchIsTransparent();
			
	}
	return res;
}
