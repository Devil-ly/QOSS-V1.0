#include "..\include\LightShow.h"

LightShow::LightShow(int num)
{
	property = vtkSmartPointer<vtkProperty>::New();
	property->SetOpacity(1);
	property->SetColor(1, 0, 0);
	for (int i = 0; i < num; ++i) {
		actors.push_back(vtkSmartPointer<vtkActor>::New());
	}
}

LightShow::~LightShow()
{
}

void LightShow::updateData()
{
}

vtkSmartPointer<vtkActor> LightShow::getActor() const
{
	return vtkSmartPointer<vtkActor>();
}
