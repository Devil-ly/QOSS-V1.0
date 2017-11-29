#include "..\include\QuadricSurfaceMirror.h"
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

QuadricSurfaceMirror::QuadricSurfaceMirror(const GraphTrans & _graphTrans)
{
	type = QUADRICSURFACE;
	graphTrans = _graphTrans;
	updateData();
}

QuadricSurfaceMirror::QuadricSurfaceMirror(const GraphTrans & _graphTrans, 
	const std::vector<double> parameter)
{
}

QuadricSurfaceMirror::~QuadricSurfaceMirror()
{
}

void QuadricSurfaceMirror::calPolyData(double ds)
{
	double radius = 1;
	double temp = -4 * 1; // data[7] 表示焦距
	vtkSmartPointer<vtkQuadric>quadric = vtkSmartPointer<vtkQuadric>::New();
	quadric->SetCoefficients(1, 1, 0, 0, 0, 0, 0, 0, temp, 0);

	//二次函数采样分辨率
	vtkSmartPointer<vtkSampleFunction>sample = vtkSmartPointer<vtkSampleFunction>::New();
	if (ds == 0)
		sample->SetSampleDimensions(40, 40, 20);
	else
		sample->SetSampleDimensions(int(radius / ds) * 2,
			int(radius / ds) * 2, int(-temp / ds) * 2); // 采样点和ds有关
	sample->SetImplicitFunction(quadric);
	double xmin = -radius * 2, xmax = radius * 2, ymin = -radius * 2,
		ymax = radius * 2, zmin = 1.0f / temp, zmax = -radius / temp + 1.0f / temp;
	sample->SetModelBounds(xmin, xmax, ymin, ymax, zmin, zmax);
	vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	contourFilter->SetInputConnection(sample->GetOutputPort());
	contourFilter->GenerateValues(1, 1, 1);
	contourFilter->Update();

	polyData = contourFilter->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(graphTrans.getTrans_x(),
		graphTrans.getTrans_y(), graphTrans.getTrans_z());
	transform->RotateWXYZ(graphTrans.getRotate_theta(), graphTrans.getRotate_x(),
		graphTrans.getRotate_y(), graphTrans.getRotate_z());

	transform->Translate(0, 0, -zmin);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();
}

void QuadricSurfaceMirror::updateData()
{
	calPolyData();
	calActor();
}

void QuadricSurfaceMirror::calcReflectedRay(const Vector3 &, Vector3 &, Vector3 &)
{
}
