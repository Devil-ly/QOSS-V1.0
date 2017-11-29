#include "..\include\Radiator.h"

Radiator::Radiator()
{
	property = vtkSmartPointer<vtkProperty>::New();
	property->SetOpacity(0.2);
	property->SetColor(0, 0, 1);
	actorModel = vtkSmartPointer<vtkActor>::New();
	phiNum = 4;
	cylinderNum = 4;
}

Radiator::~Radiator()
{
}

void Radiator::setSource(shared_ptr<calculation::SourceModeGeneration> source)
{
	this->source = source;
	double temp;
	source->GetCircularWaveguideProperty(theta, temp, temp);
	radius = source->getRadius();
}

RadiatorType Radiator::getMirrorsType() const
{
	return type;
}
