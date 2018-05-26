#include "comUtil.h"

void packGraphTransToJson(const GraphTrans & graphTrans, Json::Value & js)
{

	js["rotate_theta"] = graphTrans.getRotate_theta();
	js["rotate_x"] = graphTrans.getRotate_x();
	js["rotate_y"] = graphTrans.getRotate_y();
	js["rotate_z"] = graphTrans.getRotate_z();
	js["trans_x"] = graphTrans.getTrans_x();
	js["trans_y"] = graphTrans.getTrans_y();
	js["trans_z"] = graphTrans.getTrans_z();

}

bool parseJsonToGraphTrans(const Json::Value & js, GraphTrans & graphTrans)
{
	if (!js.isMember("trans_x") ||
		!js.isMember("trans_y") ||
		!js.isMember("trans_z") ||
		!js.isMember("trans_x") ||
		!js.isMember("rotate_x") ||
		!js.isMember("rotate_y") ||
		!js.isMember("rotate_z") ||
		!js.isMember("rotate_theta"))
	{
		return false;
	}

	graphTrans.setGraphTransPar(
		js["trans_x"].asDouble(),
		js["trans_y"].asDouble(),
		js["trans_z"].asDouble(),
		js["rotate_x"].asDouble(),
		js["rotate_y"].asDouble(),
		js["rotate_z"].asDouble(),
		js["rotate_theta"].asDouble()
		);
	return true;
}
