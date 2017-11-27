/*
*	created by liyun 2017/11/22
*   function 保存镜子2D位置以及计算生成3D位置 保存限制的尺寸大小
*   version 1.0
*/

#ifndef MIRRORPOSITION_H
#define MIRRORPOSITION_H

#include <vector>
#include "../util/Vector3.h"
#include "../util/GraphTrans.h"
namespace calculation
{
	class MirrorPosition
	{
	public:
		MirrorPosition();
		~MirrorPosition();

		void setMirrorNum(int num);
		int getMirrorNum() const { return mirrorNum; }

		void setLightDirection(int num, const Vector3& light);
		Vector3 getLightDirection(int num) const;

		void setLightLength(int num, double light);
		double getLightLength(int num) const;

		void setStartingPoint(const Vector3& light);
		Vector3 getStartingPoint() const;

		// 只设置不计算
		void setLightPhi(int num, double phi);
		// 通过修改夹角 影响下一条光线
		void calcLightPhi(int num, double phi);

		void getBoundaryByDefault(double &_length, double &_width);


		// 计算生成镜子的初始位置
		void getInitialPosition(vector<GraphTrans> &position);

	private:
		void calcLightPhi(int num);

		std::vector<Vector3> lightDirection;
		std::vector<double> lightLength;
		std::vector<double> lightPhi;

		Vector3 org;
		int mirrorNum;
		double scale;
	};
}


#endif // MIRRORPOSITION_H
