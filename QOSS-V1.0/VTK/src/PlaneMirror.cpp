#include "../include/PlaneMirror.h"

#include <vtkPlaneSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>

PlaneMirror::PlaneMirror(const GraphTrans & _graphTrans)
{
	type = PLANEMIRROR;
	graphTrans = _graphTrans;
	updateData();
}

PlaneMirror::PlaneMirror(const GraphTrans & _graphTrans, 
	const std::vector<double> parameter)
{
	type = PLANEMIRROR;
	graphTrans = _graphTrans;
	updateData();
}

PlaneMirror::~PlaneMirror()
{
}

void PlaneMirror::calPolyData(double ds)
{
	float w = 0.2 / 2.0;
	float d = 0.2 / 2.0;
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
	tree->setData(0, Qt::UserRole, QVariant(PLANEMIRROR));

	tree->addChild(getTransformTree());
	return tree;
}
