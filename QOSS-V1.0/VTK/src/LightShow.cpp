#include "..\include\LightShow.h"
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>


LightShow::LightShow(std::vector<Mirror*> _mirrors, int num)
	:mirrors(_mirrors), phiNum(6), cylinderNum(6)
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
	calRayActor();
}

void LightShow::calRayActor()
{
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> pLineCell =
		vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkLine> p1 = vtkSmartPointer<vtkLine>::New();
	int cout = 0;
	for (int i = 0; i < phiNum; i++)
		for (int j = 0; j < cylinderNum; j++)
		{
			points->InsertNextPoint(rayPosition[i][j].x, rayPosition[i][j].y, rayPosition[i][j].z);
			points->InsertNextPoint(rayPosition[i][j].x + rayVector[i][j].x * 1,
				rayPosition[i][j].y + rayVector[i][j].y * 1,
				rayPosition[i][j].z + rayVector[i][j].z * 1);
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

	(*(actors.begin()))->SetMapper(pointMapper);
	(*(actors.begin()))->SetProperty(property);
}

std::list<vtkSmartPointer<vtkActor>> LightShow::getActors() const
{
	return actors;
}

void LightShow::createStartPointBySource(shared_ptr<calculation::SourceModeGeneration> source)
{
	source->GetRayTracingSource(phiNum, cylinderNum, rayPosition, rayVector);
}
