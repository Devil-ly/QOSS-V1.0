/*
*	created by liyun 2017/12/25  Merry Christmas  
*   function 镜子的限制条件 有圆柱和立方体两种选择
*   version 1.0
*/

#ifndef RESTRICTION_H
#define RESTRICTION_H

#include "BasicParameters.h"
#include <vector>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <QTreeWidgetItem>
#include <QString>

class Restriction : public BasicParameters
{
public:
	Restriction(); // 给继承用
	Restriction(const Restriction&); // 给继承用

	// 构造默认的平面镜 没有传入参数
	Restriction(const GraphTrans & _graphTrans);
	// 构造平面镜 传入参数
	Restriction(const GraphTrans & _graphTrans, const std::vector<double> parameter);
	virtual ~Restriction();

	void calPolyData(double ds = 0);
	vtkSmartPointer<vtkPolyData> getPolyData() const;

	virtual void updateData();

	// 输出入射光线计算反射光线和交点
	virtual void calcReflectedRay(const Vector3&, Vector3&, Vector3&) {};

	QTreeWidgetItem* getTree();

	vtkSmartPointer<vtkActor> getActor() const;
	void calActor();

	void setDataByNum(int, double);

private:

	//保存每个模型的显示和剖分数据
	vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkPolyData> polyData;

	bool isTransparent; // 是否显示
	vtkSmartPointer<vtkProperty> property;

};

#endif // RESTRICTION_H