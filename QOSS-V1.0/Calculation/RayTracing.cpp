#include "RayTracing.h"

#include "../util/Definition.h"


calculation::RayTracing::RayTracing(Mirror* _mirror)
	:mirror(_mirror)
{
	isCalcMatrix = false;
}

calculation::RayTracing::~RayTracing()
{
}

void calculation::RayTracing::setMirror(Mirror * mirror)
{
	this->mirror = mirror;
}

void calculation::RayTracing::calcReflectBatch(const vector<vector<Vector3>>& startPiont,
	const vector<vector<Vector3>>& direction,
	vector<vector<Vector3>> &reflex, vector<vector<Vector3>> &intersection,
	vector<vector<bool>> &isIntersect)
{
	switch (mirror->getMirrorsType())
	{
	case PLANEMIRROR:
		calcReflectByPolyDataBatch(startPiont, direction, reflex, intersection, isIntersect);
		break;
	default:
		break;
	}
}

void calculation::RayTracing::calcReflect(const Vector3 & startPiont, const Vector3 & direction, 
	Vector3 & reflex, Vector3 & intersection, bool & isIntersect)
{
	switch (mirror->getMirrorsType())
	{
	case PLANEMIRROR:
		calcReflectByPolyData(startPiont, direction, reflex, intersection, isIntersect);
		break;
	case PARABOLICCYLINDER:
		calcReflectByQuadricSurface(startPiont, direction, reflex, intersection, isIntersect);
		break;
	default:
		break;
	}
}


void calculation::RayTracing::calcReflectByPolyDataBatch(const vector<vector<Vector3>>& startPiont,
	const vector<vector<Vector3>>& direction,
	vector<vector<Vector3>> &reflex, vector<vector<Vector3>> &intersection,
	vector<vector<bool>> &isIntersect)
{
	for (int i = 0; i < startPiont.size(); i++)
		for (int j = 0; j < startPiont[i].size(); j++)
		{
			bool isTmep = false;
			calcReflectByPolyData(startPiont[i][j], direction[i][j], reflex[i][j],
				intersection[i][j], isTmep);
			isIntersect[i][j] = isTmep;
		}
}

void calculation::RayTracing::calcReflectByPolyData(const Vector3 & startPiont, 
	const Vector3 & direction, Vector3 & reflex, 
	Vector3 & intersection, bool & isIntersect)
{
	vtkSmartPointer<vtkPolyData> polyData = mirror->getPolyData();
	int EleNum = polyData->GetNumberOfCells();
	double t;
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
		if (this->isIntersect(startPiont, direction, NodesXYZ1,
			NodesXYZ2, NodesXYZ3, intersection, t))
		{
			if (t >= 0)
			{
				Vector3 tempa = NodesXYZ1 - NodesXYZ2;
				Vector3 tempb = NodesXYZ1 - NodesXYZ3;
				Vector3 n_light = tempa.Cross(tempb);  //法向量

				isIntersect = true;
				reflex = reflectLight(intersection, n_light);
				break;
			}
		}
	}
	isIntersect = false;
}

void calculation::RayTracing::calcReflectByQuadricSurface(const Vector3 & startPiont, 
	const Vector3 & direction, Vector3 & reflex, Vector3 & intersection, bool & isIntersect)
{
	double t;
	const vector<double>& tempData = mirror->getData();
	
	if (!isCalcMatrix)
	{
		this->calcMatrix();
	}

	// 将世界坐标系转到模型的相对坐标系
	Vector3 tempStartPiont = R_translateMatrix * R_rotatMatrix * startPiont;
	Vector3 tempDirection = R_rotatMatrix * direction; // 向量只用求旋转

	if (ray_CurvedSurface(tempData, tempDirection, tempStartPiont, t, intersection))
	{
		if (tempData[10] - THRESHOLD < intersection.x  &&
			intersection.x < tempData[11] + THRESHOLD &&
			tempData[12] - THRESHOLD < intersection.y && 
			intersection.y < tempData[13] + THRESHOLD &&
			tempData[14] - THRESHOLD < intersection.z &&
			intersection.z < tempData[15] + THRESHOLD)
			// 判断是否在给出的区间内
		{
			double x = 2 * tempData[0] * intersection.x + tempData[3] * intersection.y + 
				tempData[5] * intersection.z + tempData[6];
			double y = 2 * tempData[1] * intersection.y + tempData[3] * intersection.x +
				tempData[4] * intersection.z + tempData[7];
			double z = 2 * tempData[2] * intersection.z + tempData[4] * intersection.y +
				tempData[5] * intersection.x + tempData[8];
			Vector3 tempn(x, y, z);
			if (tempn.Dot(direction) > 0)
				tempn.set(-x, -y, -z);
			reflex = reflectLight(direction, tempn);
			isIntersect = true;

			// 将模型的相对坐标系转到世界坐标系
			intersection = translateMatrix * rotatMatrix * intersection;
			reflex = rotatMatrix * reflex; // 更新方向

		}
		else
		{
			intersection = startPiont; // 让交点等于起点 方向不变 避免对无交点时特殊处理
			isIntersect = false;
		}
			
	}
}

bool calculation::RayTracing::isIntersect(const Vector3 & orig, const Vector3 & dir,
	const Vector3 & v0, const Vector3 & v1, const Vector3 & v2, 
	Vector3 & intersection, double & t)
{
	return false;
}

bool calculation::RayTracing::ray_CurvedSurface(const vector<double> &a, Vector3 n, Vector3 org, double & t, Vector3 & interPoint)
{
	double x0 = org.x, y0 = org.y, z0 = org.z;
	double x1 = n.x, y1 = n.y, z1 = n.z;

	double A = a[0] * x1 * x1 + a[1] * y1 * y1 + a[2] * z1 * z1 + a[3] * x1 * y1 +
		a[4] * z1 * y1 + a[5] * x1 * z1;
	double B = 2 * a[0] * x1 * x0 + 2 * a[1] * y1 * y0 + 2 * a[2] * z1 * z0 +
		a[3] * (x0 * y1 + x1 * y0) + a[4] * (z0 * y1 + z1 * y0) + a[5] * (z0 * x1 + z1 * x0) +
		a[6] * x1 + a[7] * y1 + a[8] * z1;
	double C = a[0] * x0 * x0 + a[1] * y0 * y0 + a[2] * z0 * z0 +
		a[3] * x0 * y0 + a[4] * z0 * y0 + a[5] * x0 * z0 +
		a[6] * x0 + a[7] * y0 + a[8] * z0 + a[9];

	if (A < -THRESHOLD || A > THRESHOLD)
	{
		double temp = B * B - 4 * A * C;
		if (temp >= 0)
			temp = pow(temp, 0.5);
		else
			return false;

		double tempt1, tempt2;
		tempt1 = (-B + temp) / 2.0 / A;
		tempt2 = (-B - temp) / 2.0 / A; // 求根公式的两个解

		if (tempt1 >= 0.0 && tempt2 >= 0.0) // 都大于等于0 取小的
		{
			if (tempt1 > tempt2)
				t = tempt2;
			else
				t = tempt1;
		}
		else if (tempt1 < 0.0 && tempt2 < 0.0) // 都小于0 无解
		{
			return false;
		}
		else                           // 取正值
		{
			if (tempt1 < tempt2)
				t = tempt2;
			else
				t = tempt1;
		}
	}
	else                          // 只有一个交点，与法线平行
		t = -C / B;

	if (t < 0.0)
		return false;
	else
	{
		interPoint.set(x0 + x1 * t, y0 + y1 * t, z0 + z1 * t);
		return true;
	}
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

void calculation::RayTracing::calcMatrix()
{
	GraphTrans tempGraphTrans = mirror->getGraphTrans();
	// 世界坐标系转到模型的相对坐标系矩阵（逆矩阵）先旋转后平移
	Vector3D RotateAsix(tempGraphTrans.getRotate_x(),
		tempGraphTrans.getRotate_y(), tempGraphTrans.getRotate_z());
	R_rotatMatrix = Matrix4D::getRotateMatrix(-tempGraphTrans.getRotate_theta(),
		RotateAsix);
	Vector3D rotatTranslate(tempGraphTrans.getTrans_x(), tempGraphTrans.getTrans_y(),
		tempGraphTrans.getTrans_z());
	rotatTranslate = R_rotatMatrix * rotatTranslate; // 先旋转在平移（把平移的坐标一起旋转）
	R_translateMatrix = Matrix4D::getTranslateMatrix(rotatTranslate * (-1));

	// 模型的相对坐标系转到世界坐标矩阵
	rotatMatrix = Matrix4D::getRotateMatrix(tempGraphTrans.getRotate_theta(),
		RotateAsix);
	translateMatrix = Matrix4D::getTranslateMatrix(tempGraphTrans.getTrans_x(), tempGraphTrans.getTrans_y(),
		tempGraphTrans.getTrans_z());

	isCalcMatrix = true;
}
