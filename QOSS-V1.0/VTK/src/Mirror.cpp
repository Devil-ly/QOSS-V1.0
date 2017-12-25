#include "../include/Mirror.h"
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

Mirror::Mirror()
{
	property = vtkSmartPointer<vtkProperty>::New();
	property->SetOpacity(1);
	property->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
	//property->SetColor(1, 1, 0);
	actor = vtkSmartPointer<vtkActor>::New();
}

Mirror::~Mirror()
{
}

MirrorsType Mirror::getMirrorsType() const
{
	return type;
}

vtkSmartPointer<vtkActor> Mirror::getActor() const
{
	return actor;
}

void Mirror::calActor()
{
	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polyData);
	mapper->ScalarVisibilityOff();

	actor->SetMapper(mapper);
	actor->SetProperty(property);
}

vtkSmartPointer<vtkPolyData> Mirror::getPolyData() const
{
	return polyData;
}

void Mirror::setData(const vector<double>&parameter)
{
	data.resize(parameter.size());
	for (int i = 0; i < parameter.size(); i++)
	{
		data[i] = parameter[i];
	}
}

void Mirror::setGraphTrans(const GraphTrans &graphTrans)
{
	this->graphTrans = graphTrans;
	updateData();
}

void Mirror::setSelected(bool flag = true)
{
	//actor->GetProperty()->SetOpacity(0.5);
	//actor->GetProperty()->SetColor(0, 1, 0);
	if (flag)
	{
		property->SetOpacity(0.2);
		property->SetColor(0, 0, 1);
	}
	else
	{
		property->SetOpacity(1);
		property->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
	}

}

QTreeWidgetItem * Mirror::getTransformTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "Transform");
	QTreeWidgetItem * treeTranslation = new QTreeWidgetItem;
	string tempTranslation = "Translation: ";
	tempTranslation.append(graphTrans.getTransString());
	treeTranslation->setText(0, tempTranslation.c_str());
	QTreeWidgetItem * treeRotation = new QTreeWidgetItem;
	string tempRotation = "Rotation: ";
	tempRotation.append(graphTrans.getRotateString());
	treeRotation->setText(0, tempRotation.c_str());
	tree->addChild(treeTranslation);
	tree->addChild(treeRotation);
	return tree;
}
