#include "..\include\LowOrderRadiator.h"
#include "../../Calculation/RayTracing.h"

#include <vtkCylinderSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkCubeSource.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkLine.h>

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
	double r = radius;
	double h = 2 * radius*tan(Pi / 2.0 - theta);

	double max = (r + h) * 2;
	//theta = Pi / 4;
	vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
	cube->SetXLength(max);
	cube->SetZLength(max);
	cube->SetYLength(max);
	cube->SetCenter(0, 0, radius * cos(theta)- max/2);
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

	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> pLineCell =
		vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkLine> p1 = vtkSmartPointer<vtkLine>::New();
	int cout = 0;
	for (int i = 0; i < phiNum; i++)
		for (int j = 0; j < cylinderNum; j++)
		{
			double tempRadius = sqrt(rayPosition[i][j].x * rayPosition[i][j].x +
				rayPosition[i][j].y * rayPosition[i][j].y);
			double tempx = radius / tempRadius * rayPosition[i][j].x;
			double tempRadiusVector = sqrt(rayVector[i][j].x * rayVector[i][j].x +
				rayVector[i][j].y * rayVector[i][j].y);

			double tempz = (radius - tempRadius) / tempRadiusVector * rayVector[i][j].z;
			double tempy = 0;
			if (tempz < (tempx + radius) / tan(theta)) // 反射
			{
				tempy = radius / tempRadius * rayPosition[i][j].y;
				points->InsertNextPoint(rayPosition[i][j].x, rayPosition[i][j].y, rayPosition[i][j].z);
				points->InsertNextPoint(tempx, tempy, tempz);
				rayPosition[i][j].x = tempx;
				rayPosition[i][j].y = tempy;
				rayPosition[i][j].z = tempz;
				// 法线
				Vector3 n_ray(-2 * tempx, -2 * tempy, 0);
				// 求反射
				rayVector[i][j] = calculation::RayTracing::reflectLight(rayVector[i][j], n_ray);

				p1->GetPointIds()->SetId(0, cout++);
				p1->GetPointIds()->SetId(1, cout++);
				pLineCell->InsertNextCell(p1);

			}

			points->InsertNextPoint(rayPosition[i][j].x, rayPosition[i][j].y, rayPosition[i][j].z);
			points->InsertNextPoint(rayPosition[i][j].x + rayVector[i][j].x * 0.1,
				rayPosition[i][j].y + rayVector[i][j].y * 0.1,
				rayPosition[i][j].z + rayVector[i][j].z * 0.1);
			

			p1->GetPointIds()->SetId(0, cout++);
			p1->GetPointIds()->SetId(1, cout++);
			pLineCell->InsertNextCell(p1);
		}

	vtkSmartPointer<vtkPolyData>pointsData = vtkSmartPointer<vtkPolyData>::New();
	pointsData->SetPoints(points); //获得网格模型中的几何数据：点集  
	pointsData->SetLines(pLineCell);

	vtkSmartPointer<vtkPolyDataMapper> pointMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	pointMapper->SetInputData(pointsData);
	pointMapper->Update();

	actorRay->SetMapper(pointMapper);
	actorRay->SetProperty(propertyRay);

}
