#include "../include/Mirror.h"
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

Mirror::Mirror()
{
	property = vtkSmartPointer<vtkProperty>::New();
	property->SetOpacity(1);
	property->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
	actor = vtkSmartPointer<vtkActor>::New();
}

Mirror::~Mirror()
{
}

MirrorsType Mirror::getMirrorsType() const
{
	return type;
}

vtkSmartPointer<vtkActor> Mirror::getActor() const
{
	return actor;
}

void Mirror::calActor()
{
	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polyData);
	mapper->ScalarVisibilityOff();

	actor->SetMapper(mapper);
}

vtkSmartPointer<vtkPolyData> Mirror::getPolyData() const
{
	return polyData;
}
