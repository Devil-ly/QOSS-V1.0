#include "RayTracing.h"

calculation::RayTracing::RayTracing()
{
}

calculation::RayTracing::RayTracing(const vector<Vector3>& _startPiont, const vector<Vector3>& _direction)
{
}

calculation::RayTracing::~RayTracing()
{
}

void calculation::RayTracing::setMirror(Mirror * mirror)
{
	this->mirror = mirror;
}

void calculation::RayTracing::setIncidence(const vector<Vector3>& _startPiont,
	const vector<Vector3>& _direction)
{
	startPiont.assign(_startPiont.begin(), _startPiont.end());
	direction.assign(_direction.begin(), _direction.end());
}

void calculation::RayTracing::calcReflect(vector<Vector3> &reflex, vector<Vector3> &intersection,
	vector<bool> &isIntersect)
{
	switch (mirror->getMirrorsType())
	{
	case PLANEMIRROR:
		calcReflectByPolyData(reflex, intersection, isIntersect);
		break;
	default:
		break;
	}
}

void calculation::RayTracing::calcReflectByPolyData(vector<Vector3> &reflex, 
	vector<Vector3> &intersection, vector<bool> &isIntersect)
{
	vtkSmartPointer<vtkPolyData> polyData = mirror->getPolyData();
	int EleNum = polyData->GetNumberOfCells();
	double t;
	for (int j = 0; j < startPiont.size(); j++)
	{
		for (int i = 0; i < EleNum; i++)  //求与反射面的交点
		{
			vtkIdList * p;
			p = polyData->GetCell(i)->GetPointIds();
			double * point;
			point = polyData->GetPoint(p->GetId(0));
			Vector3 NodesXYZ1(point[0], point[1], point[2]);
			point = polyData->GetPoint(p->GetId(1));
			Vector3 NodesXYZ2(point[0], point[1], point[2]);
			point = polyData->GetPoint(p->GetId(2));
			Vector3 NodesXYZ3(point[0], point[1], point[2]);
			if (this->isIntersect(startPiont[j], direction[j], NodesXYZ1,
				NodesXYZ2, NodesXYZ3, intersection[j], t))
			{
				if (t >= 0)
				{
					Vector3 tempa = NodesXYZ1 - NodesXYZ2;
					Vector3 tempb = NodesXYZ1 - NodesXYZ3;
					Vector3 n_light = tempa.Cross(tempb);  //法向量

					isIntersect[j] = true;
					reflex[j] = reflectLight(intersection[j], n_light);
					break;
				}				
			}
		}
		isIntersect[j] = false;
	}
	
}

bool calculation::RayTracing::isIntersect(const Vector3 & orig, const Vector3 & dir,
	const Vector3 & v0, const Vector3 & v1, const Vector3 & v2, 
	Vector3 & intersection, double & t)
{
	return false;
}

Vector3 calculation::RayTracing::reflectLight(const Vector3 & a, const Vector3 & n)
{
	//先单位化
	double absa = pow(a.Dot(a), 0.5);
	double absn = pow(n.Dot(n), 0.5);
	Vector3 tempa = a * (1 / absa);
	Vector3 tempn = n * (1 / absn);
	double I = 2 * tempn.Dot(tempa);
	if (I < 0)
		I = -I;
	else
		tempa = Vector3(0.0, 0.0, 0.0) - tempa;

	return tempn * I + tempa;
}
