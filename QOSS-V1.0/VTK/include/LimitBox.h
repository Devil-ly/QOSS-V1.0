/*
*	created by liyun 2017/11/23
*   function 限制的盒子大小 
*   version 1.0
*/

#ifndef LIMITBOX_H
#define LIMITBOX_H

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>

class LimitBox
{
public:
	LimitBox(double length, double width, double height);
	~LimitBox();

	void setLength(double _length);
	void setWidth(double _width);
	void setHeight(double _height);

	void updateData();

	vtkSmartPointer<vtkActor> getActor() const;

	double getMaxSize() const;

private:
	vtkSmartPointer<vtkActor> actor;

	bool isTransparent; // 是否显示
	vtkSmartPointer<vtkProperty> property;

	double length;
	double width;
	double height;
};

#endif // LIMITBOX_H
