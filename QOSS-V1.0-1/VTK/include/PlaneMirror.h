/*
*	created by liyun 2017/10/24
*   function 平面镜(默认的镜子)	  
*	width --> data[0]
*	depth --> data[1];
*   version 1.0
*/

#ifndef PLANEMIRROR_H  
#define PLANEMIRROR_H

#include "Mirror.h"
#include <vector>

class PlaneMirror: public Mirror
{
public:
	// 构造默认的平面镜 没有传入参数
	PlaneMirror(const GraphTrans & _graphTrans);
	// 构造平面镜 传入参数
	PlaneMirror(const GraphTrans & _graphTrans, const std::vector<double> parameter);
	virtual ~PlaneMirror();

	virtual void calPolyData(double ds = 0);

	virtual void updateData();

	// 输出入射光线计算反射光线和交点
	virtual void calcReflectedRay(const Vector3&, Vector3&, Vector3&);

	virtual QTreeWidgetItem* getTree();

	void setWidth(double);
	double getWidth() const { return data[0]; }

	void setDepth(double);
	double getDepth() const { return data[1]; }

private:




};



#endif // PLANEMIRROR_H