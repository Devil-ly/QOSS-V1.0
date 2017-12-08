#include "..\include\Radiator.h"
#include <vector>

Radiator::Radiator()
{
	property = vtkSmartPointer<vtkProperty>::New();
	property->SetOpacity(0.2);
	property->SetColor(0, 0, 1);
	propertyRay = vtkSmartPointer<vtkProperty>::New();
	propertyRay->SetOpacity(1);
	propertyRay->SetColor(1, 0, 0);
	actorModel = vtkSmartPointer<vtkActor>::New();
	actorRay = vtkSmartPointer<vtkActor>::New();
	phiNum = 2 * 200;
	cylinderNum = 10;
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

double Radiator::getFirstMirrorHeight(double x)
{	
	return 0.0;
}

void Radiator::getRay(std::vector<std::vector<Vector3>>& rayPosition,
	std::vector<std::vector<Vector3>>& rayVector) const
{
	rayPosition.assign(this->rayPosition.begin(), this->rayPosition.end());
	rayVector.assign(this->rayVector.begin(), this->rayVector.end());
}
