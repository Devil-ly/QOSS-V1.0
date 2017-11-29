/*
*	created by liyun 2017/11/29
*   function 辐射器的虚基类
*   version 1.0
*/

#ifndef RADIATOR_H  
#define RADIATOR_H

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <memory>

#include "../../Calculation/SourceModeGeneration.h"

enum RadiatorType
{
	LOWORDER = 0
};


class Radiator
{
public:

	Radiator();
	virtual ~Radiator();

	void setSource(shared_ptr<calculation::SourceModeGeneration>);

	RadiatorType getMirrorsType() const;

	virtual void calActorModel() = 0;
	vtkSmartPointer<vtkActor> getActorModel() const { return actorModel; }

	virtual void calActorRay() = 0;

protected:

	RadiatorType type;

	//保存每个模型的显示
	vtkSmartPointer<vtkActor> actorModel;
	vtkSmartPointer<vtkActor> actorRay;

	bool isTransparent; // 是否显示
	vtkSmartPointer<vtkProperty> property;

	shared_ptr<calculation::SourceModeGeneration> source;

	//布里渊角
	double theta;
	double radius;

	// 光线
	int phiNum;
	int cylinderNum;
	std::vector <std::vector <Vector3>> rayPosition;
	std::vector <std::vector <Vector3>> rayVector;
};


#endif // RADIATOR_H

