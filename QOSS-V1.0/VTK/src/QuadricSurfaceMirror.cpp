#include "..\include\QuadricSurfaceMirror.h"
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangle.h>

#include "../Calculation/Matrix4D.h"
#include "../Calculation/Vector3D.h"

#include <vtkBooleanOperationPolyDataFilter.h>

#include "../util/comUtil.h"

QuadricSurfaceMirror::QuadricSurfaceMirror()
{
}

QuadricSurfaceMirror::QuadricSurfaceMirror(const GraphTrans & _graphTrans)
{
	type = QUADRICSURFACE;
	graphTrans = _graphTrans;
	updateData();
}

QuadricSurfaceMirror::QuadricSurfaceMirror(const GraphTrans & _graphTrans, 
	const std::vector<double> parameter)
{
	type = QUADRICSURFACE;
	graphTrans = _graphTrans;
	setData(parameter);
	updateData();
}

QuadricSurfaceMirror::~QuadricSurfaceMirror()
{
}

void QuadricSurfaceMirror::calPolyData(double ds)
{
	double radius = 1;
	double temp = -4 * 1; // data[7] 表示焦距
	vtkSmartPointer<vtkQuadric>quadric = vtkSmartPointer<vtkQuadric>::New();
	quadric->SetCoefficients(data[0], data[1], data[2], data[3], data[4], data[5], data[6], 
		data[7], data[8], data[9]+1);

	//二次函数采样分辨率
	vtkSmartPointer<vtkSampleFunction>sample = vtkSmartPointer<vtkSampleFunction>::New();
	if (ds == 0)
		sample->SetSampleDimensions(60, 60, 30);
	else
		sample->SetSampleDimensions(int(radius / ds) * 2,
			int(radius / ds) * 2, int(-temp / ds) * 2); // 采样点和ds有关
	sample->SetImplicitFunction(quadric);
	sample->SetModelBounds(data[10], data[11], data[12], data[13], data[14], data[15]);
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

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();

	calcRestriction();
}

void QuadricSurfaceMirror::updateData()
{
	calPolyData();
	calcActorAxes();
	calActor();
}

QTreeWidgetItem * QuadricSurfaceMirror::getTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "QUADRICSURFACE");
	tree->setData(0, Qt::UserRole, QVariant(QUADRICSURFACE));

	for (int i = 0; i < data.size(); i++)
	{
		QTreeWidgetItem * treeData = new QTreeWidgetItem;
		treeData->setText(0, QString("a") + QString::number(i) 
			+ QString(": ") + QString::number(data[0]));
		tree->addChild(treeData);
	}	

	tree->addChild(getTransformTree());
	return tree;
}

double QuadricSurfaceMirror::calcZ(double x, double y)
{
	double res = 0;
	if (abs(data[2]) < 0.000000001)
	{
		double temp = data[4] * y + data[5] * x + data[8];
		if (abs(temp) > 0.000000001) // 被除数不等于0
		{
			res = -(data[0] * x*x + data[1] * y*y + data[3] * x*y + data[6] * x + data[7] * y + data[9])
				/ temp;
		}	
	}
	else
	{
		double A = data[2];
		double C = data[0] * x*x + data[1] * y*y + data[3] * x*y + data[6] * x + data[7] * y + data[9];
		double B = data[4] * y + data[5] * x + data[8];

		double temp = B * B - 4 * A * C;
		if (temp >= 0)
			temp = sqrt(temp);
		double res1 = (-B + temp) / 2 / A;
		double res2 = (-B - temp) / 2 / A;
		if (res2 < res1)
			return res2;
		else
			return res1;
	}
	return res;
}

Json::Value QuadricSurfaceMirror::getDataJson(const string& dir, int index) const
{
	Json::Value js;
	js["type"] = type;

	for (const auto &x : data)
	{
		js["Data"].append(x);
	}
	js["isTransparent"] = isTransparent;
	js["isShow"] = isShow;
	Json::Value jsGraphTrans;
	packGraphTransToJson(graphTrans, jsGraphTrans);
	js["graphTrans"] = jsGraphTrans;

	js["restrictionNum"] = restrictions.size();
	for (const auto &x : restrictions)
	{
		js["restriction"].append(x->getDataJson());
	}

	return js;
}

void QuadricSurfaceMirror::calcRestriction()
{
	if (restrictions.empty())
		return;

	vtkSmartPointer<vtkPolyData> polyDataRestriction;

	polyDataRestriction = restrictions[0]->getPolyData();
		
	if (restrictions.size() > 1)
	{
		for (int i = 1; i < restrictions.size(); ++i)
		{

			vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation =
				vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();

			booleanOperation->SetOperationToIntersection();

			booleanOperation->SetInputData(0, polyDataRestriction); // set the input data  
			booleanOperation->SetInputData(1, restrictions[i]->getPolyData());
			booleanOperation->Update();

			polyDataRestriction = booleanOperation->GetOutput();
		}	
	}	
	vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation =
		vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();

	booleanOperation->SetOperationToIntersection();

	booleanOperation->SetInputData(0, polyData); // set the input data  
	booleanOperation->SetInputData(1, polyDataRestriction);
	booleanOperation->Update();

	polyData = booleanOperation->GetOutput();

	// 世界坐标系转到模型的相对坐标系矩阵（逆矩阵）先旋转后平移
	Vector3D RotateAsix(graphTrans.getRotate_x(),
		graphTrans.getRotate_y(), graphTrans.getRotate_z());
	Matrix4D R_rotatMatrix = Matrix4D::getRotateMatrix(
		-graphTrans.getRotate_theta(), RotateAsix);
	//Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(-surfaceData[0], -surfaceData[1], -surfaceData[2]);

	Vector3D rotatTranslate(graphTrans.getTrans_x(),
		graphTrans.getTrans_y(), graphTrans.getTrans_z());
	rotatTranslate = R_rotatMatrix * rotatTranslate; // 先旋转在平移（把平移的坐标一起旋转）
	Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(rotatTranslate * (-1));

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkTriangle> p1 = vtkSmartPointer<vtkTriangle>::New();
	vtkSmartPointer<vtkCellArray>pLineCell = vtkCellArray::New();

	int EleNum = polyData->GetNumberOfCells();
	int couter = 0; // 计数器
	for (int i = 0; i < EleNum; i++)
	{
		vtkIdList * p = polyData->GetCell(i)->GetPointIds();
		bool ok = true;
		for (int j = 0; j < 3; j++)
		{
			double * temppoint = polyData->GetPoint(p->GetId(j));
			Vector3 point = R_translateMatrix*R_rotatMatrix*
				Vector3(temppoint[0], temppoint[1], temppoint[2]);
			double temp = data[0] * point.x * point.x + data[1] * point.y * point.y
				+ data[2] * point.z * point.z + data[3] * point.x * point.y
				+ data[4] * point.y * point.z + data[5] * point.x * point.z
				+ data[6] * point.x + data[7] * point.y + data[8] * point.z
				+ data[9];

			if (temp < -0.001 || temp > 0.001)
			{
				ok = false;
				break;
			}
		}
		if (ok)
		{
			points->InsertNextPoint(polyData->GetPoint(p->GetId(0)));
			points->InsertNextPoint(polyData->GetPoint(p->GetId(1)));
			points->InsertNextPoint(polyData->GetPoint(p->GetId(2)));
			p1->GetPointIds()->SetId(0, couter++);
			p1->GetPointIds()->SetId(1, couter++);
			p1->GetPointIds()->SetId(2, couter++);
			pLineCell->InsertNextCell(p1);
		}
	}

	vtkSmartPointer<vtkPolyData> polyDataNew = vtkSmartPointer<vtkPolyData>::New();
	polyDataNew->SetPoints(points); //获得网格模型中的几何数据：点集  
	polyDataNew->SetStrips(pLineCell);

	polyData = polyDataNew;
	//polyData = polyDataRestriction;
	
}
