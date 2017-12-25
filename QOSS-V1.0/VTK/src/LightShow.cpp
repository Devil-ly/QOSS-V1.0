#include "..\include\LightShow.h"
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include "../Calculation/RayTracing.h"


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
	std::vector <std::vector <Vector3>> tempRayPosition(rayPosition);
	std::vector <std::vector <Vector3>> tempRayVector(rayVector);
	std::vector <std::vector <bool>> isIntersect(rayPosition.size(),
		std::vector<bool>(rayPosition[0].size(), false));

	//rayTracing.calcReflectBatch(tempRayPosition, tempRayVector, 
	//	tempRayVector, tempRayPosition, isIntersect);

	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> pLineCell =
		vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkLine> p1 = vtkSmartPointer<vtkLine>::New();
	int cout = 0;
	Vector3 tempIntersect;
	Vector3 tempReflect;
	bool tempIsIntersect;
	for (int n = 0; n < 3; n++)
	{
		calculation::RayTracing rayTracing(mirrors[n]);

		for (int i = 0; i < tempRayPosition.size(); i++)
			for (int j = 0; j < tempRayPosition[i].size(); j++)
			{
				rayTracing.calcReflect(tempRayPosition[i][j], tempRayVector[i][j], tempReflect,
					tempIntersect, tempIsIntersect);
				if (tempIsIntersect)
				{
					points->InsertNextPoint(tempRayPosition[i][j].x,
						tempRayPosition[i][j].y, tempRayPosition[i][j].z);
					points->InsertNextPoint(tempIntersect.x,
						tempIntersect.y, tempIntersect.z);

					p1->GetPointIds()->SetId(0, cout++);
					p1->GetPointIds()->SetId(1, cout++);
					pLineCell->InsertNextCell(p1);

					tempRayVector[i][j] = tempReflect;
					tempRayPosition[i][j] = tempIntersect;

				}

				/*
				points->InsertNextPoint(tempRayPosition[i][j].x,
					tempRayPosition[i][j].y, tempRayPosition[i][j].z);
				points->InsertNextPoint(tempRayPosition[i][j].x + tempRayVector[i][j].x,
					tempRayPosition[i][j].y + tempRayVector[i][j].y,
					tempRayPosition[i][j].z + tempRayVector[i][j].z);
				p1->GetPointIds()->SetId(0, cout++);
				p1->GetPointIds()->SetId(1, cout++);

				pLineCell->InsertNextCell(p1);
				*/

			}
	}
	for (int i = 0; i < tempRayPosition.size(); i++)
		for (int j = 0; j < tempRayPosition[i].size(); j++)
		{
			points->InsertNextPoint(tempRayPosition[i][j].x,
				tempRayPosition[i][j].y, tempRayPosition[i][j].z);
			points->InsertNextPoint(tempRayPosition[i][j].x + 0.5 * tempRayVector[i][j].x,
				tempRayPosition[i][j].y + 0.5 * tempRayVector[i][j].y,
				tempRayPosition[i][j].z + 0.5 * tempRayVector[i][j].z);
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

void LightShow::createStartPointByRadiator(shared_ptr<Radiator> radiator)
{
	radiator->getRay(rayPosition, rayVector);
}
