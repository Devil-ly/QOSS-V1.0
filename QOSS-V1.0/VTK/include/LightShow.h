/*
*	created by liyun 2017/11/23
*   function 绘制3D 光线显示 保存光线的起点
*   version 1.0
*/

#ifndef LIGHTSHOW_H  
#define LIGHTSHOW_H

#include <vtkLine.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <list>
#include <vector>
#include "../util/Vector3.h"
#include "../VTK/include/Mirror.h"
#include "../Calculation/SourceModeGeneration.h"
#include <memory>

class LightShow
{
public:
	LightShow(std::vector<Mirror*>, int);
	~LightShow();

	void updateData();
	void calRayActor();

	std::list<vtkSmartPointer<vtkActor>> getActors() const;

	void createStartPointBySource(shared_ptr<calculation::SourceModeGeneration>);

private:

	std::list<vtkSmartPointer<vtkActor>> actors;

	bool isTransparent; // 是否显示
	vtkSmartPointer<vtkProperty> property;

	int phiNum; 
	int cylinderNum;
	std::vector <std::vector <Vector3>> rayPosition;
	std::vector <std::vector <Vector3>> rayVector;
	std::vector<Mirror*> mirrors;
};

#endif //LIGHTSHOW_H