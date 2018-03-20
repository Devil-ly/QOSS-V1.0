/*
*	created by liyun 2017/11/28
*   function 二次曲面 
*   F(x,y,z) = a0*x^2 + a1*y^2 + a2*z^2 + a3*x*y + a4*y*z + a5*x*z + a6*x + a7*y + a8*z + a9
*   a0~a9 对应 data[0]~data[9]
*   xmin xmax ymin ymax zmin zmax 对应 data[10]~data[15]
*   version 1.0
*/

#ifndef QUADRICSURFACEMIRROR_H  
#define QUADRICSURFACEMIRROR_H

#include "Mirror.h"
#include <vector>

class QuadricSurfaceMirror : public Mirror
{
public:
	QuadricSurfaceMirror(); // 给继承用
	// 构造默认的平面镜 没有传入参数
	QuadricSurfaceMirror(const GraphTrans & _graphTrans);
	// 构造平面镜 传入参数
	QuadricSurfaceMirror(const GraphTrans & _graphTrans, const std::vector<double> parameter);
	virtual ~QuadricSurfaceMirror();

	virtual void calPolyData(double ds = 0);

	virtual void updateData();

	virtual QTreeWidgetItem* getTree();

	// 输出x,y 得到Z 若z有多个值时 只返回正值
	double calcZ(double x, double y);

private:

	void calcRestriction();

};



#endif // QUADRICSURFACEMIRROR_H