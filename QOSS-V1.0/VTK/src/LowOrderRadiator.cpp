#include "..\include\LowOrderRadiator.h"

#include <vtkCylinderSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkCubeSource.h>
#include <vtkBooleanOperationPolyDataFilter.h>

LowOrderRadiator::LowOrderRadiator(shared_ptr<calculation::SourceModeGeneration> s)
{
	setSource(s);
	type = LOWORDER;
}

LowOrderRadiator::~LowOrderRadiator()
{
}

void LowOrderRadiator::calActorModel()
{
	//theta = Pi / 4;
	vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
	cube->SetXLength(0.5);
	cube->SetZLength(0.5);
	cube->SetYLength(0.5);
	cube->SetCenter(0, 0, radius * cos(theta)-0.25);
	cube->Update();

	vtkSmartPointer<vtkPolyData> polyData1 = cube->GetOutput();
	vtkSmartPointer<vtkTransform> transform1 = vtkSmartPointer<vtkTransform>::New();
	transform1->RotateWXYZ(-(Pi / 2.0 - theta) / Pi * 180, 0, 1, 0);
	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter1 =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter1->SetInputData(polyData1);
	TransFilter1->SetTransform(transform1); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter1->Update();
	polyData1 = TransFilter1->GetOutput();

	vtkSmartPointer<vtkTriangleFilter> triangleFilter1 =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter1->SetInputData(polyData1);
	//triangleFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	triangleFilter1->Update();
	polyData1 = triangleFilter1->GetOutput();

	float r = radius;
	float h = 2 * radius*tan(Pi / 2.0 - theta);
	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
	//plane->SetCenter(0, 0, 0);
	cylinder->SetHeight(h);
	cylinder->SetRadius(r);
	cylinder->SetCenter(0, h / 2, 0);
	cylinder->SetResolution(40);
	cylinder->Update();

	vtkSmartPointer<vtkPolyData> polyData2 = cylinder->GetOutput();
	vtkSmartPointer<vtkTransform> transform2 = vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)

	transform2->RotateWXYZ(90, 1, 0, 0);
	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter2 =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter2->SetInputData(polyData2);
	TransFilter2->SetTransform(transform2); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter2->Update();
	polyData2 = TransFilter2->GetOutput();

	vtkSmartPointer<vtkTriangleFilter> triangleFilter2 =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter2->SetInputData(polyData2);
	//triangleFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	triangleFilter2->Update();
	polyData2 = triangleFilter2->GetOutput();


	vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation =
		vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();

	booleanOperation->SetOperationToIntersection();

	booleanOperation->SetInputData(0, polyData1); // set the input data  
	booleanOperation->SetInputData(1, polyData2);
	booleanOperation->Update();

	vtkSmartPointer<vtkPolyData> polyData = booleanOperation->GetOutput();

	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polyData);
	mapper->ScalarVisibilityOff();
	
	actorModel->SetMapper(mapper);
	actorModel->SetProperty(property);
}

void LowOrderRadiator::calActorRay()
{
	source->GetRayTracingSource(phiNum, cylinderNum, rayPosition, rayVector);

}
