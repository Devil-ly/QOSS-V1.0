/*
*	created by liyun 2018/3/13
*   function 导入STL曲面
*   version 1.0
*/

#ifndef STLMirror_H  
#define STLMirror_H

#include "Mirror.h"
#include <vector>
#include <string>

class STLMirror : public Mirror
{
public:
	STLMirror(); // 给继承用

	// 构造默认的平面镜 没有传入参数
	STLMirror(const GraphTrans & _graphTrans);
	// 构造平面镜 传入参数
	STLMirror(const GraphTrans & _graphTrans,
		const std::vector<double> parameter,
		const string & filename);
	virtual ~STLMirror();

	virtual void calPolyData(double ds = 0);

	virtual void updateData();

	virtual QTreeWidgetItem* getTree();

	void setNameFile(const std::string & file);

	void readData();

	virtual Json::Value getDataJson(const string& dir, int index) const;

private:
	std::string fileName;
	vtkSmartPointer<vtkPolyData> polyDataSTL;

};



#endif // STLMirror_H
