#include "MirrorPosition.h"
#include "../util/Definition.h"
#include "../Calculation/Matrix4D.h"
#include "../util/Constant_Var.h"

using namespace calculation;

MirrorPosition::MirrorPosition():
	lightDirection(std::vector<Vector3>(MAX_NUM_OF_MIRROS+1)),
	lightLength(std::vector<double>(MAX_NUM_OF_MIRROS + 1)), org(Vector3(0, 0, 0)),
	lightPhi(std::vector<double>(MAX_NUM_OF_MIRROS + 1))
{
	mirrorNum = 0;
}

MirrorPosition::~MirrorPosition()
{
}

void MirrorPosition::setMirrorNum(int num)
{
	mirrorNum = num;
}

void MirrorPosition::setLightDirection(int num, const Vector3 & light)
{
	if (num >= 0 && num <= 4)
	{
		lightDirection[num] = light;
		lightDirection[num].Normalization();
	}
}

Vector3 MirrorPosition::getLightDirection(int num) const
{
	if (num <= mirrorNum && num >= 0)
		return lightDirection[num];
	else
		return Vector3(0, 0, 0);
}

void MirrorPosition::setLightLength(int num, double light)
{
	if (num >= 0 && num <= 4)
	{
		lightLength[num] = light;
	}
}

double MirrorPosition::getLightLength(int num) const
{
	if (num <= mirrorNum && num >= 0)
		return lightLength[num];
	else
		return 0;
}

void MirrorPosition::setStartingPoint(const Vector3 & light)
{
	org = light;
}

Vector3 MirrorPosition::getStartingPoint() const
{
	return org;
}

void calculation::MirrorPosition::setLightPhi(int num, double phi)
{
	if (num < 0 || num > mirrorNum)
		return;
	lightPhi[num] = phi;
}

void calculation::MirrorPosition::calcLightPhi(int num, double phi)
{
	if (num < 0 || num > mirrorNum)
		return;
	lightPhi[num] = phi;
	calcLightPhi(num);			
}

void MirrorPosition::getBoundaryByDefault(double & _length, double & _width)
{
	Vector3 temp = org;
	Vector3 next;
	// 找到画图的左右上下边界
	double xmax = org.x;
	double xmin = org.x;
	double ymax = org.z;
	double ymin = org.z;
	for (int i = 0; i < mirrorNum; ++i) {
		next = temp + lightDirection[i] * lightLength[i];
		if (next.x  > xmax)
			xmax = next.x;
		if (next.x < xmin)
			xmin = next.x;
		if (next.z > ymax)
			ymax = next.z;
		if (next.z < ymin)
			ymin = next.z;
		temp = next;
	}
	next = temp + lightDirection[mirrorNum] * lightLength[mirrorNum - 1];
	if (next.x  > xmax)
		xmax = next.x;
	if (next.x < xmin)
		xmin = next.x;
	if (next.z > ymax)
		ymax = next.z;
	if (next.z < ymin)
		ymin = next.z;

	_length = (xmax - xmin) * MIRROR_POSITION_2DSHOW_SCALE;
	_width = (ymax - ymin) * MIRROR_POSITION_2DSHOW_SCALE;

}

void calculation::MirrorPosition::getInitialPosition(vector<GraphTrans>& position)
{
	Vector3 temp = org;
	Vector3 next;
	Vector3 tempMirror;
	position.resize(mirrorNum);
	for (int i = 0; i < mirrorNum; ++i) {
		GraphTrans graphTrans;
		// 计算位置
		next = temp + lightDirection[i] * lightLength[i];
		temp = next;
		graphTrans.updateTranslate(next);
		// 计算旋转 平面镜与x轴的夹角
		tempMirror = lightDirection[i] + lightDirection[i + 1];
		double phi = acos(tempMirror.x);
		if (tempMirror.Cross(Vector3(1, 0, 0)).y > 0)
		{
			phi = -phi;
		}
		graphTrans.updateRotate(Vector3(0, 1, 0), phi / Pi * 180);
		position[i] = graphTrans;
	}
}

void calculation::MirrorPosition::calcLightPhi(int num)
{
	for (int i = num; i < mirrorNum; i++)
	{
		Matrix4D mat = Matrix4D::getRotateMatrix(180 - lightPhi[i], 0, 1, 0);
		Vector3 temp1 = mat * lightDirection[i];

		mat = Matrix4D::getRotateMatrix(180 - lightPhi[i], 0, -1, 0);
		Vector3 temp2 = mat * lightDirection[i];

		if (temp1.z > temp2.z)
			lightDirection[i + 1] = temp1;
		else
			lightDirection[i + 1] = temp2;
	}
	
}
