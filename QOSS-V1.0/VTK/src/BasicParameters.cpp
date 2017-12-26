#include "..\include\BasicParameters.h"

BasicParameters::BasicParameters()
{
}

BasicParameters::~BasicParameters()
{
}

void BasicParameters::setData(const vector<double>& parameter)
{
	data.resize(parameter.size());
	for (int i = 0; i < parameter.size(); i++)
	{
		data[i] = parameter[i];
	}
}

void BasicParameters::setGraphTrans(const GraphTrans &graphTrans)
{
	this->graphTrans = graphTrans;
	updateData();
}
