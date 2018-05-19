#include "CUDARayTracing.h"
#include "CUDADLL.h"
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>


CUDARayTracing::CUDARayTracing()
	:numPoints(0),
	psourcex(nullptr),
	pdirx(nullptr),
	pdiry(nullptr),
	pdirz(nullptr),
	intersected(nullptr),
	prot(nullptr),
	STLIndex(nullptr),
	inter_x(nullptr),
	inter_y(nullptr),
	inter_z(nullptr),
	numSTL(0),
	stlp1x(nullptr),
	stlp1y(nullptr),
	stlp1z(nullptr),
	stlp2x(nullptr),
	stlp2y(nullptr),
	stlp2z(nullptr),
	stlp3x(nullptr),
	stlp3y(nullptr),
	stlp3z(nullptr)
{

}

CUDARayTracing::~CUDARayTracing()
{
	if (psourcex)
	{
		delete[] psourcex;
		psourcex = nullptr;
	}
	if (pdirx)
	{
		delete[] pdirx;
		pdirx = nullptr;
	}
	if (pdiry)
	{
		delete[] pdiry;
		pdiry = nullptr;
	}
	if (pdirz)
	{
		delete[] pdirz;
		pdirz = nullptr;
	}
	if (intersected)
	{
		delete[] intersected;
		intersected = nullptr;
	}
	if (prot)
	{
		delete[] prot;
		prot = nullptr;
	}
	if (STLIndex)
	{
		delete[] STLIndex;
		STLIndex = nullptr;
	}

	if (inter_x)
	{
		delete[] inter_x;
		inter_x = nullptr;
	}
	if (inter_y)
	{
		delete[] inter_y;
		inter_y = nullptr;
	}
	if (inter_z)
	{
		delete[] inter_z;
		inter_z = nullptr;
	}

	if (stlp1x)
	{
		delete[] stlp1x;
		stlp1x = nullptr;
	}
	if (stlp1y)
	{
		delete[] stlp1y;
		stlp1y = nullptr;
	}
	if (stlp1z)
	{
		delete[] stlp1z;
		stlp1z = nullptr;
	}

	if (stlp2x)
	{
		delete[] stlp2x;
		stlp2x = nullptr;
	}
	if (stlp2y)
	{
		delete[] stlp2y;
		stlp2y = nullptr;
	}
	if (stlp2z)
	{
		delete[] stlp2z;
		stlp2z = nullptr;
	}
}

int CUDARayTracing::getCUDAInfo()
{
	//to do
	DeviceInf();
	return 0;
}

int CUDARayTracing::run()
{
	RunReflectionLine(numPoints, psourcex, psourcey, psourcez,
		pdirx, pdiry, pdirz,
		intersected, prot, STLIndex,
		inter_x, inter_y, inter_z,
		numSTL, stlp1x, stlp1y, stlp1z,
		stlp2x, stlp2y, stlp2z,
		stlp3x, stlp3y, stlp3z);

	return 0;
}

void CUDARayTracing::setSTL(void* _polyData)
{
	vtkPolyData* polyData = (vtkPolyData*)(_polyData);
	numSTL = polyData->GetNumberOfCells();
	stlp1x = new float[numSTL];
	stlp1y = new float[numSTL];
	stlp1z = new float[numSTL];
	stlp2x = new float[numSTL];
	stlp2y = new float[numSTL];
	stlp2z = new float[numSTL];
	stlp3x = new float[numSTL];
	stlp3y = new float[numSTL];
	stlp3z = new float[numSTL];

	vtkIdList * p;
	double * point;
	//读取各点的位置
	for (int i = 0; i < numSTL; i++) {
		p = polyData->GetCell(i)->GetPointIds();
		//点1
		point = polyData->GetPoint(p->GetId(0));
		stlp1x[i] = point[0]; stlp1y[i] = point[1]; stlp1z[i] = point[2];
		//点2
		point = polyData->GetPoint(p->GetId(1));
		stlp2x[i] = point[0]; stlp2y[i] = point[1]; stlp2z[i] = point[2];
		//点3
		point = polyData->GetPoint(p->GetId(2));
		stlp3x[i] = point[0]; stlp3y[i] = point[1]; stlp3z[i] = point[2];
	}
}

void CUDARayTracing::setRays(Vector3 ** Plane, Vector3 ** n_Plane, int n, int m)
{
	numPoints = n*m;

	intersected = new bool[numPoints];
	STLIndex = new int[numPoints];
	prot = new float[numPoints];
	inter_x = new float[numPoints];
	inter_y = new float[numPoints];
	inter_z = new float[numPoints];
	//入射场的输入量
	psourcex = new float[numPoints];
	psourcey = new float[numPoints];
	psourcez = new float[numPoints];
	pdirx = new float[numPoints];
	pdiry = new float[numPoints];
	pdirz = new float[numPoints];

	int index;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			index = j + i*m;
			psourcex[index] = Plane[i][j].x;
			psourcey[index] = Plane[i][j].y;
			psourcez[index] = Plane[i][j].z;

			pdirx[index] = n_Plane[i][j].x;
			pdiry[index] = n_Plane[i][j].y;
			pdirz[index] = n_Plane[i][j].z;
		}
	}
}

void CUDARayTracing::setRays(const std::vector<Vector3>& startPiont, const std::vector<Vector3>& direction)
{
	numPoints = startPiont.size();

	intersected = new bool[numPoints];
	STLIndex = new int[numPoints];
	prot = new float[numPoints];
	inter_x = new float[numPoints];
	inter_y = new float[numPoints];
	inter_z = new float[numPoints];
	//入射场的输入量
	psourcex = new float[numPoints];
	psourcey = new float[numPoints];
	psourcez = new float[numPoints];
	pdirx = new float[numPoints];
	pdiry = new float[numPoints];
	pdirz = new float[numPoints];


	for (int i = 0; i < numPoints; i++) {
		psourcex[i] = startPiont[i].x;
		psourcey[i] = startPiont[i].y;
		psourcez[i] = startPiont[i].z;

		pdirx[i] = direction[i].x;
		pdiry[i] = direction[i].y;
		pdirz[i] = direction[i].z;
		
	}
}

void CUDARayTracing::getRes(std::vector<Vector3> &normal,
	std::vector<Vector3> &intersection,
	std::vector<bool> &isIntersect,
	std::vector<float> &prot)
{
	for (int i = 0; i < numPoints; i++)
	{
		
		Vector3 NodesXYZ1(stlp1x[STLIndex[i]], stlp1y[STLIndex[i]], stlp1z[STLIndex[i]]);
		Vector3 NodesXYZ2(stlp2x[STLIndex[i]], stlp2y[STLIndex[i]], stlp3z[STLIndex[i]]);
		Vector3 NodesXYZ3(stlp3x[STLIndex[i]], stlp2y[STLIndex[i]], stlp3z[STLIndex[i]]);
		Vector3 tempa = NodesXYZ1 - NodesXYZ2;
		Vector3 tempb = NodesXYZ1 - NodesXYZ3;
		normal[i] = tempa.Cross(tempb);  //法向量
		prot[i] = this->prot[i];
		intersection[i] = Vector3(this->inter_x[i], this->inter_y[i], this->inter_z[i]);
		isIntersect[i] = this->intersected[i];
	}
}