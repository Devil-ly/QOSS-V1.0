/*
*	created by liyun 2017/11/28
*   function 二次曲面 
*   F(x,y,z) = a0*x^2 + a1*y^2 + a2*z^2 + a3*x*y + a4*y*z + a5*x*z + a6*x + a7*y + a8*z + a9
*   version 1.0
*/

#ifndef QUADRICSURFACEMIRROR_H  
#define QUADRICSURFACEMIRROR_H

#include "Mirror.h"
#include <vector>

class QuadricSurfaceMirror : public Mirror
{
public:
	// 构造默认的平面镜 没有传入参数
	QuadricSurfaceMirror(const GraphTrans & _graphTrans);
	// 构造平面镜 传入参数
	QuadricSurfaceMirror(const GraphTrans & _graphTrans, const std::vector<double> parameter);
	virtual ~QuadricSurfaceMirror();

	virtual void calPolyData(double ds = 0);

	virtual void updateData();

	// 输出入射光线计算反射光线和交点
	virtual void calcReflectedRay(const Vector3&, Vector3&, Vector3&);
private:

};



#endif // QUADRICSURFACEMIRROR_H