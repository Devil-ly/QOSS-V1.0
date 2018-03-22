#include "..\include\PhsCorMirror.h"
#include "..\include\QuadricSurfaceMirror.h"
#include "..\Calculation\Matrix4D.h"
#include "..\Calculation\Vector3D.h"
#include <vtkFloatArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkDelaunay2D.h>
#include <../Calculation/RayTracing.h>

PhsCorMirror::PhsCorMirror()
{
	type = PHSCORMIRROR;
}

PhsCorMirror::~PhsCorMirror()
{
}

void PhsCorMirror::calPolyData(double ds)
{
	vtkFloatArray *scalars = vtkFloatArray::New();
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	int N = lattice.size();
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
		{
			points->InsertNextPoint(lattice[i][j].x, lattice[i][j].y, lattice[i][j].z);
		}
	polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(points);

	vtkSmartPointer<vtkDelaunay2D> delaunay =
		vtkSmartPointer<vtkDelaunay2D>::New();
	delaunay->SetInputData(polyData);
	delaunay->Update();
	polyData = delaunay->GetOutput();
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
	lattice.resize(N);
	for (int i = 0; i < N; i++)
		lattice[i].resize(N);
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
			lattice[i][j] = tempVec;
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

	int N = ceil(length / ds) + 1;
	lattice.resize(N);
	for (int i = 0; i < N; i++)
		lattice[i].resize(N);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			Vector3 tempVec(i * ds - length / 2, j * ds - length / 2, 0);
			tempVec = translateMatrix * rotatMatrix * tempVec;
			bool isInter = false;
			double t;
			rayTracing.calcNormalOfLine_Mirror(tempVec, direction, Vector3(), tempVec, isInter, t);
			if (!isInter)
				return false;

			lattice[i][j] = tempVec;
		}
	return true;
}

void PhsCorMirror::setLattice(const vector<vector<Vector3>> & temp)
{
	int N = temp.size();
	lattice.resize(N);
	for (int i = 0; i < N; i++)
		lattice[i].resize(N);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			lattice[i][j] = temp[i][j];
		}
}
