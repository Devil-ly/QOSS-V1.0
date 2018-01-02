/*
*	created by liyun 2017/12/25  Merry Christmas
*   function 基类
*   version 1.0
*/

#ifndef BASICPARAMETERS_H  
#define BASICPARAMETERS_H

#include "util/GraphTrans.h"

#include <vector>
#include <../util/Vector3.h>

#include <QTreeWidgetItem>

class BasicParameters
{
public:
	BasicParameters();
	virtual ~BasicParameters();

	virtual void updateData() = 0;

	const vector<double>& getData() const { return data; }
	void setData(const vector<double>&);

	GraphTrans getGraphTrans() const { return graphTrans; }
	void setGraphTrans(const GraphTrans&);

protected:
	QTreeWidgetItem* getTransformTree();

	GraphTrans graphTrans; // 旋转平移参数
	vector<double> data;

};



#endif // BASICPARAMETERS_H