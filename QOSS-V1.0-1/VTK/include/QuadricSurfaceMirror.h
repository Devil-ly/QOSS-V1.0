/*
*	created by liyun 2017/11/28
*   function �������� 
*   F(x,y,z) = a0*x^2 + a1*y^2 + a2*z^2 + a3*x*y + a4*y*z + a5*x*z + a6*x + a7*y + a8*z + a9
*   a0~a9 ��Ӧ data[0]~data[9]
*   xmin xmax ymin ymax zmin zmax ��Ӧ data[10]~data[15]
*   version 1.0
*/

#ifndef QUADRICSURFACEMIRROR_H  
#define QUADRICSURFACEMIRROR_H

#include "Mirror.h"
#include <vector>

class QuadricSurfaceMirror : public Mirror
{
public:
	QuadricSurfaceMirror(); // ���̳���
	// ����Ĭ�ϵ�ƽ�澵 û�д������
	QuadricSurfaceMirror(const GraphTrans & _graphTrans);
	// ����ƽ�澵 �������
	QuadricSurfaceMirror(const GraphTrans & _graphTrans, const std::vector<double> parameter);
	virtual ~QuadricSurfaceMirror();

	virtual void calPolyData(double ds = 0);

	virtual void updateData();

	// ���������߼��㷴����ߺͽ���
	virtual void calcReflectedRay(const Vector3&, Vector3&, Vector3&);

	virtual QTreeWidgetItem* getTree();

private:

	void calcRestriction();

};



#endif // QUADRICSURFACEMIRROR_H