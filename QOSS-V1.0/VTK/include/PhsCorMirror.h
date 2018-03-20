/*
*	created by liyun 2018/3/17
*   function 相位修正面
*   version 1.0
*/

#ifndef PhsCorMirror_H  
#define PhsCorMirror_H

#include "Mirror.h"
#include <vector>
#include <string>

class PhsCorMirror : public Mirror
{
public:
	PhsCorMirror(); 

	virtual ~PhsCorMirror();

	virtual void calPolyData(double ds = 0);

	virtual void updateData();

	virtual QTreeWidgetItem* getTree();

	void sampling(double ds, double length, const Vector3& central, Mirror* mirror);

private:

	vector<vector<Vector3>> lattice;


};



#endif // PhsCorMirror_H
