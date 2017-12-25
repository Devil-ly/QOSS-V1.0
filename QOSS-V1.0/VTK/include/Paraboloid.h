/*
*	created by liyun 2017/12/6
*   function 二次曲面 的子类
*   x^2 + y^2 = 4Fx  // f为交点
*   R 为x,y的最大值
*   version 1.0
*/

#ifndef PARABOLOID_H
#define PARABOLOID_H

#include "QuadricSurfaceMirror.h"
class Paraboloid : public  QuadricSurfaceMirror
{
public:
	// 构造默认的平面镜 没有传入参数
	Paraboloid(const GraphTrans & _graphTrans);
	// 构造平面镜 传入参数
	Paraboloid(const GraphTrans & _graphTrans, const std::vector<double>& parameter);

	void setParameter(double focus, double radius);

	void setFocus(double);
	double getFocus() const { return focus; }

	void setRadius(double);
	double getRadius() const { return radius; }

	virtual QTreeWidgetItem* getTree();
private:
	double focus;
	double radius;
};

#endif // PARABOLOID_H