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

#include "../Calculation/Matrix4D.h"
#include "../Calculation/Vector3D.h"
#include <vector>

namespace calculation
{
	class RayTracing
	{
	public:
		RayTracing(Mirror* _mirror);

		~RayTracing();

		void setMirror(Mirror* mirror);

		// 计算直线与mirror相交，并输出交点与法线
		void calcNormalOfLine_Mirror(const Vector3& startPiont,
			const Vector3& direction,
			Vector3 &normal, Vector3 &intersection,
			bool &isIntersect, double &t);

		// 输出反射光线和交点以及是否相交
		void calcReflect(const Vector3& startPiont,
			const Vector3& direction,
			Vector3 &reflex, Vector3 &intersection,
			bool &isIntersect);

		void calcReflectBatch(const vector<vector<Vector3>>& startPiont,
			const vector<vector<Vector3>>& direction,
			vector<vector<Vector3>> &reflex, vector<vector<Vector3>> &intersection,
			vector<vector<bool>> &isIntersect);

		// 根据入射光线和法线求反射光线
		static Vector3 reflectLight(const Vector3& a, const Vector3& n);

		// 如果是解析需要 提前计算好变换矩阵
		void calcMatrix();
	private:

		// 根据模型剖分数据计算反射
		void calcReflectByPolyDataBatch(const vector<vector<Vector3>>& startPiont,
			const vector<vector<Vector3>>& direction,
			vector<vector<Vector3>> &reflex, vector<vector<Vector3>> &intersection,
			vector<vector<bool>> &isIntersect);

		void calcReflectByPolyData(const Vector3& startPiont,
			const Vector3& direction, Vector3 &reflex,
			Vector3 &intersection,
			bool &isIntersect);

		void calcReflectByQuadricSurface(const Vector3& startPiont,
			const Vector3& direction, Vector3 &reflex,
			Vector3 &intersection,
			bool &isIntersect);

		void calcNormalOfLine_MirrorByQuadricSurface(const Vector3& startPiont,
			const Vector3& direction,
			Vector3 &normal, Vector3 &intersection,
			bool &isIntersect, double &t);

		void calcNormalOfLine_MirrorByPolyData(const Vector3& startPiont,
			const Vector3& direction,
			Vector3 &normal, Vector3 &intersection,
			bool &isIntersect, double &t);

		// 三角形与直线相交判断
		bool isIntersect(const Vector3 &orig, const Vector3 &dir,
			const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
			Vector3 &intersection, double &t);

		bool ray_CurvedSurface(const vector<double> & a, Vector3 n, Vector3 org,
			double &t, Vector3 &interPoint);

		// 直线与面相交 可能有两个解
		void line_CurvedSurface(const vector<double> & a, Vector3 n, Vector3 org,
			double &t1, double &t2, bool &isOk1, bool &isOk2, Vector3 &interPoint1,
			Vector3 &interPoint2);

		bool isInRestriction(const Vector3 &intersectionGlobal);

		Mirror* mirror;

		bool isCalcMatrix;

		// 世界坐标系转到模型的相对坐标系矩阵（逆矩阵）先旋转后平移
		vector<Matrix4D> R_rotatMatrix;
		vector<Matrix4D> R_translateMatrix;

		// 模型的相对坐标系转到世界坐标矩阵
		vector<Matrix4D> rotatMatrix;
		vector<Matrix4D> translateMatrix;

	};

}

#endif // RAYTRACING_H

