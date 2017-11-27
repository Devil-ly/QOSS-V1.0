/*
*	created by liyun 2017/11/27
*   function 计算光线追踪 输入镜子的指针 输出反射光线和交点以及是否相交
*   version 1.0
*/

#ifndef RAYTRACING_H  
#define RAYTRACING_H

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include "../util/Vector3.h"
#include "../VTK/include/Mirror.h"

namespace calculation
{
	class RayTracing
	{
	public:
		RayTracing(const vector<Vector3>& _startPiont,
			const vector<Vector3>& _direction);

		~RayTracing();

		void setMirror(Mirror* mirror);

		void setIncidence(const vector<Vector3>& _startPiont,
			const vector<Vector3>& _direction);

		// 输出反射光线和交点以及是否相交
		void calcReflect(vector<Vector3> &reflex,vector<Vector3> &intersection,
			vector<bool> &isIntersect);

	private:

		// 根据模型剖分数据计算反射
		void calcReflectByPolyData(vector<Vector3> &reflex, vector<Vector3> &intersection,
			vector<bool> &isIntersect);

		// 三角形与直线相交判断
		bool isIntersect(const Vector3 &orig, const Vector3 &dir,
			const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
			Vector3 &intersection, double &t);

		// 根据入射光线和法线求反射光线
		Vector3 reflectLight(const Vector3& a, const Vector3& n);

		Mirror* mirror;
		vector<Vector3> startPiont;
		vector<Vector3> direction;


	};

}

#endif // RAYTRACING_H

