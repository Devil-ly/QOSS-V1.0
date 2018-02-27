/*
*	created by liyun 2017/12/7
*   function 二次曲面 的子类
*   x^2 / a^2 + y^2 / b^2 + z^2 / c^2 = 1 
*   R 为x,y的最大值
*   version 1.0
*/
#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "QuadricSurfaceMirror.h"
class Ellipsoid : public  QuadricSurfaceMirror
{
public:
	// 构造默认的平面镜 没有传入参数
	Ellipsoid(const GraphTrans & _graphTrans);
	// 构造平面镜 传入参数
	Ellipsoid(const GraphTrans & _graphTrans, const std::vector<double>& parameter);

	void setParameter(double a, double b, double c, double theta);

	virtual QTreeWidgetItem* getTree();
private:
	double a;
	double b;
	double c;
	double theta;
};

#endif // ELLIPSOID_H