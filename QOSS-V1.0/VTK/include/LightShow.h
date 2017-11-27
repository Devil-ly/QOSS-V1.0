/*
*	created by liyun 2017/11/23
*   function 绘制3D 光线显示
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

class LightShow
{
public:
	LightShow(int num);
	~LightShow();

	void updateData();

	vtkSmartPointer<vtkActor> getActor() const;

private:

	std::list<vtkSmartPointer<vtkActor>> actors;

	bool isTransparent; // 是否显示
	vtkSmartPointer<vtkProperty> property;

};

#endif //LIGHTSHOW_H