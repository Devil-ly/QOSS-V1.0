/*
*	created by liyun 2017/12/4
*   function 二次曲面 的子类
*   y^2 = 4Fx  // f为交点
*   R 为y的最大值
*   version 1.0
*/

#ifndef PARABOLICCYLINDER_H
#define PARABOLICCYLINDER_H

#include "QuadricSurfaceMirror.h"
class ParabolicCylinder : public  QuadricSurfaceMirror
{
public:
	// 构造默认的平面镜 没有传入参数
	ParabolicCylinder(const GraphTrans & _graphTrans);
	// 构造平面镜 传入参数
	ParabolicCylinder(const GraphTrans & _graphTrans, const std::vector<double>& parameter);

	void setParameter(double focus, double yMax, double zMin, double zMax);

	void setFocus(double);
	double getFocus() const { return focus; }

	void setRadius(double);
	double getRadius() const { return yMax; }

	void setZMin(double);
	double getZMin() const { return zMin; }

	void setZMax(double);
	double getZMax() const { return zMax; }

	virtual QTreeWidgetItem* getTree();
private:
	double focus;
	double yMax;
	double zMin;
	double zMax;
};

#endif // PARABOLICCYLINDER_H