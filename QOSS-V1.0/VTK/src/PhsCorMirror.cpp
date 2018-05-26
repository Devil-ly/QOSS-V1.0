#include "..\include\PhsCorMirror.h"
#include "..\include\QuadricSurfaceMirror.h"
#include "..\Calculation\Matrix4D.h"
#include "..\Calculation\Vector3D.h"
#include <vtkFloatArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkDelaunay2D.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <../Calculation/RayTracing.h>
#include "../util/comUtil.h"

using namespace std;
PhsCorMirror::PhsCorMirror()
	//:lattice(201,vector<Vector3>(201,Vector3()))
{
	type = PHSCORMIRROR;
	lattice = nullptr;
}

PhsCorMirror::PhsCorMirror(const GraphTrans & _graphTrans,
	const std::vector<double> parameter,
	const string & filename, int N)
{
	type = PHSCORMIRROR;
	graphTrans = _graphTrans;
	setData(parameter);
	read(filename, N);
	updateData();
}

PhsCorMirror::~PhsCorMirror()
{
	if (lattice)
	{
		delete lattice;
		lattice = nullptr;
	}
}

void PhsCorMirror::calPolyData(double ds)
{
	vtkFloatArray *scalars = vtkFloatArray::New();
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	int N = (*lattice).size();
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
		{
			points->InsertNextPoint((*lattice)[i][j].x, (*lattice)[i][j].y, (*lattice)[i][j].z);
		}
	polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(points);

	vtkSmartPointer<vtkDelaunay2D> delaunay =
		vtkSmartPointer<vtkDelaunay2D>::New();
	delaunay->SetInputData(polyData);
	delaunay->Update();
	polyData = delaunay->GetOutput();

	// 用户自定义平移旋转 (先移动后旋转)
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
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

void PhsCorMirror::updateData()
{
	calPolyData();
	calActor();
}

QTreeWidgetItem * PhsCorMirror::getTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "PhsCorMirror");
	tree->setData(0, Qt::UserRole, QVariant(PHSCORMIRROR));


	tree->addChild(getTransformTree());
	return tree;
}

void PhsCorMirror::sampling(double ds, double length,
	const Vector3 & central, Mirror * mirror)
{
	QuadricSurfaceMirror* mirror1 = dynamic_cast<QuadricSurfaceMirror*>(mirror);
	int N = ceil(length / ds) + 1;

	if (lattice)
	{
		delete lattice;
		lattice = nullptr;
	}
	lattice = new vector<vector<Vector3>>(N, vector<Vector3>(N, Vector3()));

	//lattice.resize(N);
	//for (int i = 0; i < N; i++)
	//	lattice[i].resize(N);

	GraphTrans graphTrans = mirror1->getGraphTrans();
	// 平移到mirror的局部坐标系
	Vector3D RotateAsix(graphTrans.getRotate_x(),
		graphTrans.getRotate_y(),
		graphTrans.getRotate_z());
	Matrix4D R_rotatMatrix = Matrix4D::getRotateMatrix(
		-graphTrans.getRotate_theta(), RotateAsix);
	Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(
		-graphTrans.getTrans_x(),
		-graphTrans.getTrans_y(), -graphTrans.getTrans_z());
	Matrix4D R_Matrix = R_rotatMatrix * R_translateMatrix;

	Vector3 tempCentral = R_Matrix * central;

	double x = tempCentral.x - length / 2;
	double y = tempCentral.y - length / 2;

	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(
		graphTrans.getRotate_theta(), RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(
		graphTrans.getTrans_x(),
		graphTrans.getTrans_y(), graphTrans.getTrans_z());
	Matrix4D Matrix = translateMatrix * rotatMatrix;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			double z = mirror1->calcZ(x + i*ds, y + j*ds);
			Vector3 tempVec(x + i*ds, y + j*ds, z);
			tempVec = Matrix * tempVec;
			(*lattice)[i][j] = tempVec;
		}
}

bool PhsCorMirror::sampling(double ds, double length, const Vector3& central,
	const Vector3& direction, const GraphTrans& graphTrans, Mirror* mirror)
{
	Vector3D RotateAsix(graphTrans.getRotate_x(),
		graphTrans.getRotate_y(),
		graphTrans.getRotate_z());
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(
		graphTrans.getRotate_theta(), RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(
		graphTrans.getTrans_x(),
		graphTrans.getTrans_y(), 
		graphTrans.getTrans_z());

	calculation::RayTracing rayTracing(mirror);

	this->graphTrans = mirror->getGraphTrans();

	//将的到镜面坐标转到局部坐标系
	Vector3D RotateAsix1(this->graphTrans.getRotate_x(),
		this->graphTrans.getRotate_y(),
		this->graphTrans.getRotate_z());
	Matrix4D R_rotatMatrix = Matrix4D::getRotateMatrix(
		- this->graphTrans.getRotate_theta(), RotateAsix1);
	Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(
		- this->graphTrans.getTrans_x(),
		- this->graphTrans.getTrans_y(),
		- this->graphTrans.getTrans_z());
	Matrix4D R_Matrix = R_rotatMatrix * R_translateMatrix;

	int N = ceil(length / ds) + 1;
	if (lattice)
	{
		delete lattice;
		lattice = nullptr;
	}
	lattice = new vector<vector<Vector3>>(N, vector<Vector3>(N, Vector3()));

	//(*lattice).resize(N);
	//for (int i = 0; i < N; i++)
	//	this->lattice[i].resize(N);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			Vector3 tempVec(i * ds - length / 2.0, j * ds - length / 2.0, 0);
			tempVec = translateMatrix * rotatMatrix * tempVec;
			bool isInter = false;
			double t;
			rayTracing.calcNormalOfLine_Mirror(tempVec, direction, Vector3(), tempVec, isInter, t);
			if (!isInter)
				return false;

			(*lattice)[i][j] = R_Matrix * tempVec;
		}
	return true;
}

const vector<vector<Vector3>>& PhsCorMirror::getLattice() const
{
	if (lattice)
	{
		return *lattice;
	}
	else
		return vector<vector<Vector3>>(0, vector<Vector3>(0, Vector3()));
}

void PhsCorMirror::setLattice(const vector<vector<Vector3>> & temp)
{
	int N = temp.size();
	if (lattice)
	{
		delete lattice;
		lattice = nullptr;
	}
	lattice = new vector<vector<Vector3>>(N, vector<Vector3>(N, Vector3()));
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			(*lattice)[i][j] = temp[i][j];
		}
}

Json::Value PhsCorMirror::getDataJson(const string & dir, int index) const
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

	// save points
	string fileDir = dir + "/pointsMirror" + to_string(index) + ".pts";;
	ofstream file(fileDir.c_str(), ios::binary);

	FILE *outfile = fopen(fileDir.c_str(), "wb");
	int N = (*lattice).size();
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			fwrite(&(*lattice)[i][j].x, sizeof(double), 1, outfile);
			fwrite(&(*lattice)[i][j].y, sizeof(double), 1, outfile);
			fwrite(&(*lattice)[i][j].z, sizeof(double), 1, outfile);
		}
	fclose(outfile);
	js["path"] = fileDir;
	js["N"] = N;
	return js;

}

void PhsCorMirror::read(const string & filename, int N)
{
	FILE *file = fopen(filename.c_str(), "rb");
	if (!file)
		return;
	(*lattice).resize(N);
	for (int i = 0; i < N; i++)
		(*lattice)[i].resize(N);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			fread(&(*lattice)[i][j].x, sizeof(double), 1, file);
			fread(&(*lattice)[i][j].y, sizeof(double), 1, file);
			fread(&(*lattice)[i][j].z, sizeof(double), 1, file);
		}
}
