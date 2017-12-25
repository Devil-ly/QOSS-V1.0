#include "..\include\QuadricSurfaceMirror.h"
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

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
		sample->SetSampleDimensions(40, 40, 20);
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
}

void QuadricSurfaceMirror::updateData()
{
	calPolyData();
	calActor();
}

void QuadricSurfaceMirror::calcReflectedRay(const Vector3 &, Vector3 &, Vector3 &)
{
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
