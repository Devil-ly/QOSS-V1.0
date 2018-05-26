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

	PhsCorMirror(const GraphTrans & _graphTrans,
		const std::vector<double> parameter, 
		const string & filename, int N);

	virtual ~PhsCorMirror();

	virtual void calPolyData(double ds = 0);

	virtual void updateData();

	virtual QTreeWidgetItem* getTree();

	void sampling(double ds, double length, const Vector3& central, Mirror* mirror);

	bool sampling(double ds, double length, const Vector3& central,
		const Vector3& direction, const GraphTrans& graphTrans, Mirror* mirror);

	const vector<vector<Vector3>> & getLattice() const; 

	void setLattice(const vector<vector<Vector3>> &);

	virtual Json::Value getDataJson(const string& dir, int index) const;

	void read(const string & filename, int N);

protected:

	vector<vector<Vector3>> *lattice;


};



#endif // PhsCorMirror_H
