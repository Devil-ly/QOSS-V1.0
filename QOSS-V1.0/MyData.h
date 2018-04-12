/*
*	created by liyun 2017/10/23
*   function 管理所有数据
*   version 1.0
*/

#ifndef MYDATA_H  
#define MYDATA_H

#include <vector>
#include <memory>
#include "../Calculation/SourceModeGeneration.h"
#include "../VTK/include/LimitBox.h"
#include "MirrorPosition.h"
#include <map>

class Mirror;
class LightShow;
class Radiator;
class Field;

class MyData //单例
{
private:
	MyData();
	~MyData();

public:
	static MyData * getInstance();
	static const MyData * getConstInstance();

	bool getModifiedFlag() const { return isModifiedFlag; }

	void setNumOfMirrors(int num) { numOfMirrors = num; isModifiedFlag = true; }
	int getNumOfMirrors() const { return numOfMirrors; }

	void setFrequency(double fre) { frequency = fre; isModifiedFlag = true; }
	double getFrequency() const { return frequency; }

	void setUnit(double _unit) { unit = _unit; }
	double getUnit() const { return unit; }

	void setPattern(int _pattern) { pattern = _pattern; isModifiedFlag = true; }
	int getPattern() const { return pattern; }

	void setMirror(int index, Mirror* _mirror);
	Mirror* getMirror(int index) const;

	// 构造默认的镜子
	void createDefaultMirror();
	void createModelMirror();
	Mirror* getMirrorByNum(int num) const;

	// 设置源
	void setSource(const shared_ptr<calculation::SourceModeGeneration> &);
	shared_ptr<calculation::SourceModeGeneration> getSource() { return source; }

	// 设置镜子初始位置
	void setMirrorPosition(const shared_ptr<calculation::MirrorPosition> &);
	shared_ptr<calculation::MirrorPosition> getMirrorPosition() { return mirrorPosition; }

	// 设置盒子
	void setLimitBox(const shared_ptr<LimitBox> &);
	shared_ptr<LimitBox> getLimitBox() { return limitBox; }

	// 生成默认的光线（有给定的源生成）
	void createDefaultLigthShow();
	shared_ptr<LightShow> getDefaultLightShow() { return defaultLigthShow; }

	// 生成辐射器
	void createRadiator();
	shared_ptr<Radiator> getRadiator() { return radiator; }

	// 源
	void setSourceField(Field*);
	Field* getSourceField() const; 

	Field* calculateByPVVA(double fre, double dis = 0.5, int N = 2);
	int addField(Field*);
	Field* getFieldByNum(int) const;

	shared_ptr<Field> getPhsCorField() const { return phsCorField; }
	void calcPhsCorField();

	bool getIsNeedCalcPhsCorFlag() const { return isNeedCalcPhsCorFlag; }

private:
	static MyData * _myData;
	bool isModifiedFlag;  // 标志该类数据是否被修改
	bool isNeedCalcPhsCorFlag;  // 标志相位修正输入是否被修改

	int numOfMirrors;
	double frequency;
	int pattern; // 0——Lower order  1——higher order 2——waveguide
	double unit;

	//镜子
	std::vector<Mirror*> mirrors;

	shared_ptr<LimitBox> limitBox;
	shared_ptr<calculation::SourceModeGeneration> source;
	shared_ptr<calculation::MirrorPosition> mirrorPosition;

	shared_ptr<LightShow> defaultLigthShow;
	shared_ptr<Radiator> radiator;

	// 保存计算结果的场 第0个为源，只能拥有一个源
	std::map<int, Field*> fieldMap;
	int fieldNum;

	shared_ptr<Field> phsCorField; //保存用于相位修正的输入场


};



#endif // MYDATA_H