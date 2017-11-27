#include "..\include\LimitBox.h"
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>

LimitBox::LimitBox(double length, double width, double height)
	:length(length), width(width), height(height)
{
	property = vtkSmartPointer<vtkProperty>::New();
	property->SetOpacity(0.2);
	property->SetColor(187/255, 1, 1);
	actor = vtkSmartPointer<vtkActor>::New();
	updateData();
}

LimitBox::~LimitBox()
{
}

void LimitBox::setLength(double _length)
{
	length = _length;
}

void LimitBox::setWidth(double _width)
{
	width = _width;
}

void LimitBox::setHeight(double _height)
{
	height = _height;
}

void LimitBox::updateData()
{
	vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
	cube->SetXLength(length);
	cube->SetZLength(width);
	cube->SetYLength(height);
	cube->SetCenter(0, 0, width / 2);
	cube->Update();

	vtkSmartPointer<vtkPolyData> polyData = cube->GetOutput();
	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polyData);

	actor->SetMapper(mapper);
	actor->SetProperty(property);
}

vtkSmartPointer<vtkActor> LimitBox::getActor() const
{
	return actor;
}

double LimitBox::getMaxSize() const
{
	if (length > width)
		if (length > height)
			return length;
		else
			return height;
	else
		if (width > height)
			return width;
		else
			return height;
}
