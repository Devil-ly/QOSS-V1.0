/*
*	created by liyun 2017/12/27
*   function 显示场 3D 2D 相位 幅度 dB
*   version 2.0 重构了保存数据的数据结构
*/
#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <fstream>
#include <complex>
#include <String>

#include "../util/GraphTrans.h"

#include "../util/Vector3.h"

#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include "BasicParameters.h"



class Field: public BasicParameters
{
public:
	Field();
	//Field(const Field & _field); // 重写构造函数
	//Field& operator =(const Field & _field);

	virtual ~Field();

	void init();

	void freeMemory();
	void allocateMemory();

	void setNM(int N, int M);
	void getNM(int& N, int &M) const;
	void setPlane(const GraphTrans & _graphTransPara, double _ds);

	void setField(complex<double> ** _Ex, complex<double> ** _Ey, complex<double> ** _Ez,
		complex<double> ** _Hx, complex<double> ** _Hy, complex<double> ** _Hz);

	const vector<vector<complex<double>>>& getEx() const;
	const vector<vector<complex<double>>>& getEy() const;

	void setDs(double _Ds);
	double getDs() const;

	//得到场的参数
	void getSourcePara(GraphTrans & _graphTransPara, 
		int & _N_width, int & _M_depth, double &_ds) const;

	void setShowPara(int _content, bool _isLinear, bool _isPhs);
	void getShowPara(int &_content, bool &_isLinear, bool &_isPhs);
	void set3D(bool _is3D);
	bool get3D() const;

	void calActor3D();
	void calActor();

	vtkSmartPointer<vtkImageActor> getActor() const;
	vtkSmartPointer<vtkActor> getActor3D() const;

	double getWidth() const { return data[7]; }
	double getDepth() const { return data[8]; }

	void save(const std::string &fileName) const;

	virtual void updateData();

	// 是否显示
	void setIsShow(bool);

	virtual QTreeWidgetItem* getTree(); 

protected:
	// 场分量
	vector<vector<complex<double>>> Ex, Ey, Ez;
	vector<vector<complex<double>>> Hx, Hy, Hz;

	int content;
	bool isLinear;
	bool isPhs;
	bool is3D;
	// 需要画图的内容 content 0~5表示Ex~H
	// isLinear 是否是线性
	// isPhs 是相位 还是幅度

	int N_width, M_depth;  //N_width = para[0] /ds

	double ds;
	
	vtkSmartPointer<vtkImageActor> actor;
	vtkSmartPointer<vtkActor> actor3D;

	bool isSource;
	//int Name; // 判断是什么场
};

#endif // !DRAW_H

