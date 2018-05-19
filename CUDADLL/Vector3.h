/*
*	created by liyun 2017/10/20
*   function 定义了三维向量，重载了-，定义了点乘和叉乘
*   version 1.0
*/

#ifndef VECTOR3_H
#define VECTOR3_H

#include<iostream>

class  Vector3
{
public:
	Vector3(double x = 0, double y = 0, double z = 0)
		:x(x), y(y), z(z)
	{

	};

	~Vector3()
	{
	};

	// 重载"-"
	Vector3 operator - (const Vector3 &v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	};

	// 重载"+"
	Vector3 operator + (const Vector3 &v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	};

	// 标量乘
	Vector3 operator * (const double &v) const
	{
		return Vector3(x * v, y * v, z * v);
	};

	// 点乘
	double Dot(const Vector3 &v) const
	{
		return x*v.x + y*v.y + z*v.z;
	};

	// 叉乘
	Vector3 Cross(const Vector3 &v) const
	{
		return Vector3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}

	// 归一化
	void Normalization()
	{
		double temp = pow((x*x + y*y + z*z), 0.5);
		x /= temp;
		y /= temp;
		z /= temp;
	}

	// 计算长度
	double Length() const
	{
		return pow((x*x + y*y + z*z), 0.5);
	}

	double area() const
	{
		return x*x + y*y + z*z;
	}

	void display()
	{
		std::cout << x << " " << y << " " << z << std::endl;
	}

	void set(double x1, double y1, double z1)
	{
		x = x1; y = y1; z = z1;
	}

	double x, y, z;
};



#endif // VECTOR3_H
