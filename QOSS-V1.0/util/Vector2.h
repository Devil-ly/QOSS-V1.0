/*
*	created by liyun 2017/10/20
*   function 定义了二维向量，重载了-，定义了点乘
*   version 1.0
*/

#ifndef VECTOR2_H
#define VECTOR2_H

struct Vector2 {
public:
	
	Vector2(double x1 = 0, double y1 = 0)
		:x(x1), y(y1)
	{};
	~Vector2() {};

	Vector2 operator - (const Vector2 v) const
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 operator + (const Vector2 v) const
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 operator * (const double k) const
	{
		return Vector2(x * k, y * k);
	}

	double dot(const Vector2 v) const
	{
		return x*v.x + y*v.y;
	}

	Vector2 Vertical() const
	{
		return Vector2(y, -x);
	}

	double x, y;
};

#endif // !Vector2_H
