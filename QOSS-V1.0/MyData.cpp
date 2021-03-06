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

#include "VTK/include/Paraboloid.h"
#include <vtkjsoncpp/json/json.h>

#include <direct.h> 
#include <fstream>

using namespace calculation;

MyData::MyData()
{
	isModifiedFlag = false;
	isNeedCalcPhsCorFlag = false;
	source = NULL;
	mirrors.resize(MAX_NUM_OF_MIRROS);
	fieldNum = 1;
	nameProject = "project";
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
	isModifiedFlag = true;
}

Mirror * MyData::getMirror(int index) const
{
	if (index < MAX_NUM_OF_MIRROS)
		return mirrors[index];
	else
		return nullptr;
}

void MyData::createModelMirror()
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
	else // 波导
	{
		GraphTrans graphTrans;
		graphTrans.setGraphTransPar(4, 0, 5, 0, 1, 0, -130);
		mirrors[0] = MirrorFactory::getMirror(PARABOLOID, graphTrans);
		dynamic_cast<Paraboloid*>(mirrors[0])->setFocus(4);
		dynamic_cast<Paraboloid*>(mirrors[0])->setRadius(4);
		mirrors[1] = MirrorFactory::getMirror(PARABOLOID, GraphTrans());
		mirrors[2] = MirrorFactory::getMirror(PARABOLOID, GraphTrans());
	}
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
		vector<double> dataPlane(2);
		dataPlane[0] = 0.2;
		dataPlane[1] = 0.2;
		//mirrors[0] = MirrorFactory::getMirror(PLANEMIRROR, position[0], dataPlane);
		mirrors[1] = MirrorFactory::getMirror(PLANEMIRROR, position[1], dataPlane);
		mirrors[2] = MirrorFactory::getMirror(PLANEMIRROR, position[2], dataPlane);
		
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
	else // 波导
	{
		GraphTrans graphTrans;
		graphTrans.setGraphTransPar(4, 0, 5, 0, 1, 0, -130);
		mirrors[0] = MirrorFactory::getMirror(PARABOLOID, graphTrans);
		dynamic_cast<Paraboloid*>(mirrors[0])->setFocus(4);
		dynamic_cast<Paraboloid*>(mirrors[0])->setRadius(4);
		mirrors[1] = MirrorFactory::getMirror(PARABOLOID, GraphTrans());
		mirrors[2] = MirrorFactory::getMirror(PARABOLOID, GraphTrans());
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
	isModifiedFlag = true;
}

void MyData::setMirrorPosition(const shared_ptr<calculation::MirrorPosition>&ptr)
{
	mirrorPosition = ptr;
	setNumOfMirrors(mirrorPosition->getMirrorNum());
	isModifiedFlag = true;
}

void MyData::setLimitBox(const shared_ptr<LimitBox>&ptr)
{
	limitBox = ptr;
	isModifiedFlag = true;
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
	else
	{
		//radiator = RadiatorFactory::getRadiator(LOWORDER, source);
		//radiator->calActorModel();
		//radiator->calActorRay();
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
	isModifiedFlag = true;
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

int MyData::addField(Field *tempField)
{
	fieldMap[fieldNum] = tempField;
	int resNum = fieldNum;
	fieldNum++;
	return resNum;
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
	//double fre = 4.4e10;
	pvva.setFre(frequency);
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

int MyData::save(const string & dir)
{
	if (mkdir(dir.c_str()))
		return -1;

	string jsQOSS = dir + "/" + nameProject + ".QOSS";
	// 判断文件是否存在
	fstream _file;
	_file.open(jsQOSS, ios::in);
	if (_file)
	{
		_file.close();
		return -1;
	}

	string jsDir = dir + "/" + nameProject + ".QOSS";
	Json::Value js;

	js["Version"] = 1.0;

	Json::Value baseInfo;
	
	baseInfo["frequency"] = frequency;
	baseInfo["pattern"] = pattern;
	baseInfo["unit"] = unit;
	baseInfo["nameProject"] = nameProject;
	baseInfo["Box"] = limitBox->getDataJson();

	js["baseInfo"] = baseInfo;

	//source
	Json::Value jsSource;
	int SourceKind = source->getSourceKind();
	int SourceType = source->getSourceType();
	int Rotation = source->getRotation();
	int m = source->getM();
	int n = source->getN();
	double Radius = source->getRadius();
	jsSource["SourceKind"] = SourceKind;
	jsSource["SourceType"] = SourceType;
	jsSource["Rotation"] = Rotation;
	jsSource["m"] = m;
	jsSource["n"] = n;
	jsSource["Radius"] = Radius;
	js["Source"] = jsSource;

	// mirror
	js["numOfMirrors"] = numOfMirrors;
	for (int i = 0; i < numOfMirrors; ++i)
	{		
		js["Mirror"].append(mirrors[i]->getDataJson(dir, i));
	}

	ofstream outfile(jsDir);
	if (!outfile.is_open())
	{
		return -1;
	}
	outfile << js.toStyledString();


	outfile.close();
	// 校验 以后来
	isModifiedFlag = false;

	return 0;
}

int MyData::open(const string & dir)
{
	ifstream file(dir);
	if (!file.is_open())
	{
		return -1;
	}
	Json::Reader reader;
	Json::Value js;
	if (!reader.parse(file, js))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		return -1;
	}

	// 确定版本
	if (!js.isMember("Version"))
	{
		return -1;
	}
	if (js["Version"].asDouble() < 0.0) //控制版本
	{
		return -1;
	}

	// baseInfo
	if (!js.isMember("baseInfo"))
	{
		return -1;
	}
	const Json::Value & baseInfo = js["baseInfo"];

	if (!baseInfo.isMember("frequency") ||
		!baseInfo.isMember("pattern") ||
		!baseInfo.isMember("unit") ||
		!baseInfo.isMember("nameProject") ||
		!baseInfo["Box"])
	{
		return -1;
	}
	frequency = baseInfo["frequency"].asDouble();
	pattern = baseInfo["pattern"].asInt();
	unit = baseInfo["unit"].asDouble();
	nameProject = baseInfo["nameProject"].asString();

	if (limitBox->setDataJson(baseInfo["Box"]) != 0)
	{
		return -1;
	}

	// source
	if (!js.isMember("Source"))
	{
		return -1;
	}
	const Json::Value &jsSource = js["Source"];
	if (!jsSource.isMember("SourceKind") ||
		!jsSource.isMember("SourceType") || 
		!jsSource.isMember("Rotation") || 
		!jsSource.isMember("m") || 
		!jsSource.isMember("n") || 
		!jsSource.isMember("Radius"))
	{
		return -1;
	}
	int SourceKind = jsSource["SourceKind"].asInt();
	int SourceType = jsSource["SourceType"].asInt();
	int Rotation = jsSource["Rotation"].asInt();
	int m = jsSource["m"].asInt();
	int n = jsSource["n"].asInt();
	double Radius = jsSource["Radius"].asDouble();

	shared_ptr<calculation::SourceModeGeneration> tempSource = make_shared<calculation::SourceModeGeneration>();
	tempSource->SetSource_Circular(SourceKind, SourceType, Rotation, m, n, frequency, Radius);

	if (!tempSource->FieldCalculation_Circular())
	{
		return -1;
	}

	// mirror
	if (!js.isMember("numOfMirrors"))
	{
		return -1;
	}
	numOfMirrors = js["numOfMirrors"].asInt();	

	// 创建 mirror
	vector<Mirror*> tempMirrors(numOfMirrors); // 创建一个临时mirror 如果打开失败 不影响原有mirror
	for (int i = 0; i < numOfMirrors; i++)
	{
		if ((tempMirrors[i] = MirrorFactory::getMirrorByJson(js["Mirror"][i])) == nullptr)
		{
			for (int j = 0; j < i; j++) // 创建失败 析构之前的mirror
			{
				delete tempMirrors[i];
				tempMirrors[i] = nullptr;
			}
			return -1;
		}
	}

	clear();


	// 替换
	for (int i = 0; i < numOfMirrors; i++)
	{
		mirrors[i] = tempMirrors[i];
		tempMirrors[i] = nullptr;
	}
	source = tempSource;

	return 0;
}

void MyData::clear()
{
	// 释放申请mirror 和 field的内存
	// 其他的有智能指针管理
	for (int i = 0; i < mirrors.size(); i++)
	{
		if (mirrors[i])
		{
			delete mirrors[i];
			mirrors[i] = nullptr;
		}
	}
	for (auto & x : fieldMap)
	{
		if (x.second)
		{
			delete x.second;
			x.second = nullptr;
		}
	}
	fieldMap.clear();
	isModifiedFlag = true;
	isNeedCalcPhsCorFlag = true;
}

