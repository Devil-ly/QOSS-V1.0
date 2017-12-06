#include "MyData.h"
#include "VTK/include/Mirror.h"
#include "VTK/include/MirrorFactory.h"
#include "VTK/include/RadiatorFactory.h"
#include "../VTK/include/LightShow.h"
#include "util/Definition.h"

using namespace calculation;

MyData::MyData()
{
	isModifiedFlag = false;
	source = NULL;
	mirrors.resize(MAX_NUM_OF_MIRROS);
	for (auto &x : mirrors)
	{
		x = nullptr;
	}
}

MyData::~MyData()
{
	if (_myData) {
		delete _myData;
		_myData = nullptr;
	}
}

MyData * MyData::_myData = new MyData();

MyData * MyData::getInstance()
{
	return _myData;
}

MyData const * MyData::getConstInstance()
{
	return _myData;
}

void MyData::setMirror(int index, Mirror * _mirror)
{
	if (index < MAX_NUM_OF_MIRROS)
		mirrors[index] = _mirror;
}

Mirror * MyData::getMirror(int index) const
{
	if (index < MAX_NUM_OF_MIRROS)
		return mirrors[index];
	else
		return nullptr;
}

void MyData::createDefaultMirror()
{
	vector<GraphTrans> position;
	mirrorPosition->getInitialPosition(position);
	if (0 == pattern) // µÍ½×
	{
		GraphTrans mirror1Position;
		mirror1Position.updateTranslate(Vector3(position[0].getTrans_x(),
			0, 0));
		double temp = abs(position[0].getTrans_x());
		vector<double> parameter(4);
		parameter[0] = temp;
		parameter[1] = temp * 2;
		parameter[2] = 0;
		parameter[3] = radiator->getFirstMirrorHeight(temp);

		mirrors[0] = MirrorFactory::getMirror(PARABOLICCYLINDER, mirror1Position, parameter);


		for (int i = 1; i < position.size(); ++i)
		{
			if (mirrors[i])
			{
				delete mirrors[i];
				mirrors[i] = nullptr;
			}
			mirrors[i] = MirrorFactory::getDefaultMirror(position[i]);
		}
	}	
}

Mirror * MyData::getMirrorByNum(int num) const
{
	if (num >= 0 && num < numOfMirrors)
		return mirrors[num];
	else
		return nullptr;
}

void MyData::setSource(const shared_ptr<calculation::SourceModeGeneration> & ptr)
{
	source = ptr;
}

void MyData::setMirrorPosition(const shared_ptr<calculation::MirrorPosition>&ptr)
{
	mirrorPosition = ptr;
	setNumOfMirrors(mirrorPosition->getMirrorNum());
}

void MyData::setLimitBox(const shared_ptr<LimitBox>&ptr)
{
	limitBox = ptr;
}

void MyData::createDefaultLigthShow()
{
	defaultLigthShow = make_shared<LightShow>(mirrors, numOfMirrors);
	defaultLigthShow->createStartPointByRadiator(radiator);
	defaultLigthShow->updateData();
}

void MyData::createRadiator()
{
	if (0 == pattern) // µÍ½×
	{
		radiator = RadiatorFactory::getRadiator(LOWORDER, source);
	}
	radiator->calActorModel();
	radiator->calActorRay();
}
