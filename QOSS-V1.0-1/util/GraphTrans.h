/*
*	created by liyun 2017/10/23
*   function 图形的变换（平移和旋转）
*   version 1.0
*/
#ifndef GRAPHTRANS_H
#define GRAPHTRANS_H

#include <fstream>
#include <sstream>
#include "Vector3.h"
using namespace std;
class GraphTrans
{
public:
	GraphTrans()
	{
		trans_x = 0;
		trans_y = 0;
		trans_z = 0;
		rotate_x = 1;
		rotate_y = 0;
		rotate_z = 0;
		rotate_theta = 0;
	}
	void setGraphTransPar(const double& _trans_x, const double& _trans_y, const double& _trans_z,
		const double& _rotate_x, const double& _rotate_y, const double& _rotate_z, const double& _rotate_theta)
	{
		trans_x = _trans_x;
		trans_y = _trans_y;
		trans_z = _trans_z;
		rotate_x = _rotate_x;
		rotate_y = _rotate_y;
		rotate_z = _rotate_z;
		rotate_theta = _rotate_theta;
	}
	void getGraphTransPar(double &_trans_x, double &_trans_y, double &_trans_z,
		double &_rotate_x, double &_rotate_y, double &_rotate_z, double &_rotate_theta) const
	{
		_trans_x = trans_x;
		_trans_y = trans_y;
		_trans_z = trans_z;
		_rotate_x = rotate_x;
		_rotate_y = rotate_y;
		_rotate_z = rotate_z;
		_rotate_theta = rotate_theta;
	}

	void normalization(double fator) //变为标准单位 m
	{
		trans_x *= fator;
		trans_y *= fator;
		trans_z *= fator;
	}

	double getTrans_x() const { return trans_x; }
	double getTrans_y() const { return trans_y; }
	double getTrans_z() const { return trans_z; }

	double getRotate_x() const { return rotate_x; }
	double getRotate_y() const { return rotate_y; }
	double getRotate_z() const { return rotate_z; }

	double getRotate_theta() const { return rotate_theta; }

	void updateRotate(Vector3 rotate, double theta)
	{
		rotate_x = rotate.x;
		rotate_y = rotate.y;
		rotate_z = rotate.z;
		rotate_theta = theta;
	}

	void updateTranslate(Vector3 tran)
	{
		trans_x = tran.x;
		trans_y = tran.y;
		trans_z = tran.z;
	}

	void updateGraphTransPar(Vector3 tran, double theta)
	{
		trans_x = tran.x;
		trans_y = tran.y;
		trans_z = tran.z;
		rotate_theta = theta;
	}

	string getTransString() const
	{
		string ss;
		stringstream stream;
		stream << "(" << trans_x
			<< "," << trans_y
			<< "," << trans_z
			<< ")";
		stream >> ss;
		return ss;
	}

	string getRotateString() const
	{
		string ss;
		stringstream stream;
		if (rotate_theta == 0.0)
			return "0";

		stream << "(" << rotate_x
			<< "," << rotate_y
			<< "," << rotate_z
			<< ") " << rotate_theta;
		stream >> ss;
		return ss;
	}

	void save(ofstream & savefile) const
	{
		savefile << trans_x << " "
			<< trans_y << " "
			<< trans_z << " "
			<< rotate_x << " "
			<< rotate_y << " "
			<< rotate_z << " "
			<< rotate_theta << endl;
	}

	void open(ifstream & readfile)
	{
		readfile >> trans_x >> trans_y >> trans_z >> rotate_x
			>> rotate_y >> rotate_z >> rotate_theta;

	}
private:
	double trans_x, trans_y, trans_z; // 平移的点
	double rotate_x, rotate_y, rotate_z; // 旋转轴
	double rotate_theta; // 旋转角度 右手定则
};

#endif // !GRAPHTRANS_H

