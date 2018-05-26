#include "..\include\STLMirror.h"
#include <vtkSTLReader.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkSTLWriter.h>

#include "util/comUtil.h"

STLMirror::STLMirror()
{
	type = STLMIRROR;
}

STLMirror::STLMirror(const GraphTrans & _graphTrans)
{
	fileName = "";
	type = STLMIRROR;
	graphTrans = _graphTrans;
	updateData();
}

STLMirror::STLMirror(const GraphTrans & _graphTrans,
	const std::vector<double> parameter,
	const string & filename)
{
	type = STLMIRROR;
	graphTrans = _graphTrans;
	setData(parameter);
	setNameFile(filename);
}

STLMirror::~STLMirror()
{
}

void STLMirror::calPolyData(double ds)
{
	vtkSmartPointer<vtkTransform> transform = 
		vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(graphTrans.getTrans_x(),
		graphTrans.getTrans_y(), graphTrans.getTrans_z());
	transform->RotateWXYZ(graphTrans.getRotate_theta(), graphTrans.getRotate_x(),
		graphTrans.getRotate_y(), graphTrans.getRotate_z());

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyDataSTL);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();

	polyData = TransFilter->GetOutput();
}

void STLMirror::updateData()
{
	if (!fileName.empty())
	{
		calPolyData();
		calActor();
	}

	calcActorAxes();
}

QTreeWidgetItem * STLMirror::getTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "STLMirror");
	tree->setData(0, Qt::UserRole, QVariant(1));
	tree->setData(1, Qt::UserRole, QVariant(STLMIRROR));

	QTreeWidgetItem * tree1 = new QTreeWidgetItem;
	tree1->setText(0, fileName.c_str());

	tree->addChild(tree1);
	tree->addChild(getTransformTree());
	return tree;
}

void STLMirror::setNameFile(const std::string & file)
{
	fileName.clear();
	fileName = file;
	readData();
}

void STLMirror::readData()
{
	vtkSmartPointer<vtkSTLReader> reader =
		vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(fileName.c_str());
	reader->Update();
	polyDataSTL = reader->GetOutput();

	calPolyData();
	calActor();
}

Json::Value STLMirror::getDataJson(const string& dir, int index) const
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

	// save stl
	string fileDir = dir + "/STLMirror" + to_string(index) + ".stl";
	vtkSmartPointer<vtkSTLWriter> writer =
		vtkSmartPointer<vtkSTLWriter>::New();
	writer->SetInputData(polyData);
	writer->SetFileName(fileDir.c_str());
	writer->Update();
	js["path"] = fileDir;

	return js;
}
