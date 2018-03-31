#include "MyData.h"
#include "VTK/include/Mirror.h"
#include "VTK/include/STLMirror.h"
#include "VTK/include/Restriction.h"
#include "VTK/include/PhsCorMirror.h"
#include "VTK/include/ParabolicCylinder.h"
#include "VTK/include/MirrorFactory.h"
#include "VTK/include/RadiatorFactory.h"
#include "VTK/include/LightShow.h"
#include "util/Definition.h"
#include "../Calculation/PVVA.h"

using namespace calculation;

MyData::MyData()
{
	isModifiedFlag = false;
	isNeedCalcPhsCorFlag = false;
	source = NULL;
	mirrors.resize(MAX_NUM_OF_MIRROS);
	fieldNum = 1;
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
	if (index >= 0 && index < numOfMirrors)
	{
		if (mirrors[index])
		{
			delete mirrors[index];
			mirrors[index] = nullptr;
		}
		mirrors[index] = _mirror;
		if (index < numOfMirrors - 1)
		{
			isNeedCalcPhsCorFlag = true;
		}
	}		
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
	if (0 == pattern) // 低阶
	{
		GraphTrans mirror1Position;
		mirror1Position.updateTranslate(Vector3(position[0].getTrans_x(),
			0, 0));
		double temp = abs(position[0].getTrans_x());
		mirrors[0] = MirrorFactory::getMirror(PARABOLICCYLINDER, mirror1Position);

		dynamic_cast<ParabolicCylinder*>(mirrors[0])->setParameter(temp, temp * 2.5, 0,
			radiator->getFirstMirrorHeight(temp));

		GraphTrans mirror2Position;
		mirror2Position.updateTranslate(Vector3(-0.0396, 0, 0.5029));
		mirror2Position.updateRotate(Vector3(0, 1, 0), -164.94);
		mirrors[1] = MirrorFactory::getMirror(PARABOLOID, mirror2Position);

		//mirrors[1] = MirrorFactory::getMirror(PLANEMIRROR, GraphTrans());
		GraphTrans mirror3Position;
		mirror3Position.updateTranslate(Vector3(0.02, 0, 0.4993));
		mirror3Position.updateRotate(Vector3(0, 1, 0), -17.23-180);
		//mirrors[2] = MirrorFactory::getMirror(ELLIPSOID, position[2]);
		Restriction * restriction = new Restriction;
		restriction->setDataByNum(0, 0.075);
		restriction->setDataByNum(1, 0.5);
		GraphTrans restrictionPosition;
		restrictionPosition.updateTranslate(Vector3(-0.02, 0, 0.0786));
		restrictionPosition.updateRotate(Vector3(0, 1, 0), 15.06);
		restriction->setGraphTrans(restrictionPosition);
		mirrors[1]->addRestriction(restriction);

		mirrors[2] = MirrorFactory::getMirror(ELLIPSOID, mirror3Position);
		//Mirror * test1 = MirrorFactory::getMirror(PARABOLOID, GraphTrans());
		//PhsCorMirror  *test2 = new PhsCorMirror;
		//test2->sampling(0.005, 0.5, Vector3(0, 0.5, 0), test1);
		//test2->updateData();
		//mirrors[2] = test2;
	}	
	else if (1 == pattern) // 高阶
	{
		STLMirror* temp = new STLMirror;
		temp->setNameFile("Mirror1.stl");
		mirrors[0] = temp;
		temp = new STLMirror;
		temp->setNameFile("Mirror2.stl");
		mirrors[1] = temp;
		temp = new STLMirror;
		temp->setNameFile("Mirror3.stl");
		mirrors[2] = temp;
		temp = new STLMirror;
		numOfMirrors = 4; // for test
		temp->setNameFile("TE226DenisovLauncher.stl");
		mirrors[3] = temp;
		
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
	if (0 == pattern) // 低阶
	{
		defaultLigthShow = make_shared<LightShow>(mirrors, numOfMirrors);
		defaultLigthShow->createStartPointByRadiator(radiator);
		defaultLigthShow->updateData();
	}
}

void MyData::createRadiator()
{
	if (0 == pattern) // 低阶
	{
		radiator = RadiatorFactory::getRadiator(LOWORDER, source);
		radiator->calActorModel();
		radiator->calActorRay();
	}
	else if (1 == pattern) // 高阶
	{

	}
	
}

void MyData::setSourceField(Field *ptr)
{
	if (fieldMap.find(0) != fieldMap.end()) // 已存在 需要删除
	{
		delete fieldMap[0];
	}
	fieldMap[0] = ptr;
	isNeedCalcPhsCorFlag = true;
}

Field * MyData::getSourceField() const
{
	if (fieldMap.find(0) != fieldMap.end())
	{
		return fieldMap.at(0);
	}
	return nullptr;
}

Field* MyData::calculateByPVVA(double fre, double dis, int N)
{
	PVVA pvva;
	// 设置单位
	pvva.setUnit(1);
	// 设置频率
	pvva.setFre(fre);
	// 读入源并分配内存
	pvva.setSource(getSourceField());
	//int N = 2;
	for (int i = 1; i <= N; ++i)
	{
		pvva.setMirror(mirrors[i]);
		pvva.CalZ0Theta();
		pvva.Reflect();
		pvva.InterVal();
	}
	pvva.Result(dis);
	Field *tempField = new Field;
	pvva.getField(tempField);
	fieldMap[fieldNum] = tempField;
	fieldNum++;
	return tempField;
}

Field * MyData::getFieldByNum(int index) const
{
	if (fieldMap.find(index) != fieldMap.end()) 
	{
		return fieldMap.at(index);
	}
	return nullptr;
}

void MyData::calcPhsCorField()
{
	PVVA pvva;
	// 设置单位
	pvva.setUnit(1);
	// 设置频率
	double fre = 1e10;
	pvva.setFre(fre);
	// 读入源并分配内存
	pvva.setSource(getSourceField());
	//int N = 2;
	for (int i = 1; i <= getNumOfMirrors() - 2; ++i)
	{
		pvva.setMirror(getMirrorByNum(i));
		pvva.CalZ0Theta();
		pvva.Reflect();
		pvva.InterVal();
	}
	if (!phsCorField)
	{
		phsCorField = make_shared<Field>();
	}
	
	pvva.getVirtualSurface(phsCorField);
}

