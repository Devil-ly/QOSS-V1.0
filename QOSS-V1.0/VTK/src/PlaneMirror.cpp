#include "../include/PlaneMirror.h"

#include <vtkPlaneSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>

PlaneMirror::PlaneMirror(const GraphTrans & _graphTrans)
{
	type = PLANEMIRROR;
	graphTrans = _graphTrans;
	data.resize(2);
	data[0] = 1;
	data[1] = 1;
	updateData();
}

PlaneMirror::PlaneMirror(const GraphTrans & _graphTrans, 
	const std::vector<double> parameter)
{
	type = PLANEMIRROR;
	graphTrans = _graphTrans;
	setData(parameter);
	updateData();
}

PlaneMirror::~PlaneMirror()
{
}

void PlaneMirror::calPolyData(double ds)
{
	float w = data[0] / 2.0;
	float d = data[1] / 2.0;
	vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();
	//plane->SetCenter(0, 0, 0);
	plane->SetOrigin(-w, -d, 0);
	plane->SetPoint1(w, -d, 0);
	plane->SetPoint2(-w, d, 0);
	plane->Update();

	polyData = plane->GetOutput();
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
	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputData(polyData);
	//triangleFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	triangleFilter->Update();
	polyData = triangleFilter->GetOutput();
}

void PlaneMirror::updateData()
{
	calPolyData();
	calcActorAxes();
	calActor();
}

void PlaneMirror::calcReflectedRay(const Vector3 &incidence, Vector3 &reflex, 
	Vector3 &intersection)
{

}

QTreeWidgetItem * PlaneMirror::getTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "PlaneMirror");
	tree->setData(0, Qt::UserRole, QVariant(1));
	tree->setData(1, Qt::UserRole, QVariant(PLANEMIRROR));

	QTreeWidgetItem * tree1 = new QTreeWidgetItem;
	tree1->setText(0, "PlaneMirror");
	tree1->setText(0, QString("Width: ") + QString::number(data[0]));

	QTreeWidgetItem * tree2 = new QTreeWidgetItem;
	tree2->setText(0, "PlaneMirror");
	tree2->setText(0, QString("Depth: ") + QString::number(data[1]));

	tree->addChild(tree1);
	tree->addChild(tree2);
	tree->addChild(getTransformTree());
	return tree;
}

void PlaneMirror::setWidth(double w)
{
	data[0] = w;
	updateData();
}

void PlaneMirror::setDepth(double d)
{
	data[1] = d;
	updateData();
}
