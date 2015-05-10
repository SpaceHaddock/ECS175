#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>

class Vector2
{
public:
	Vector2();
	Vector2(float, float);

	float x;
	float y;

	void SetValue(int, float);
	float GetValue(const int) const;
	float Length() const;

	static Vector2 Lerp(const Vector2& a, const Vector2& b, const float val);

	bool operator < (const Vector2& other) const
	{
		return (x < other.x);
	}

	Vector2 operator-() const
	{
	   Vector2 v;
	   v.x = -x;
	   v.y = -y;
	   return v;
	}

	Vector2 operator-(const Vector2 &a)
	{
		Vector2 v;
		v.x = x - a.x;
		v.y = y - a.y;
		return v;
	}

	Vector2 operator+(const Vector2 &a)
	{
		Vector2 v;
		v.x = x + a.x;
		v.y = y + a.y;
		return v;
	}

	Vector2 operator*(const float a)
	{
		return Vector2(x * a, y * a);
	}

	bool operator==(const Vector2 &a)
	{
		return round(x) == round(a.x) && round(y) == round(a.y);
	}

	bool operator!=(const Vector2 &a)
	{
		return !(*this == a);
	}

	Vector2 Divide(const float a)
	{
		Vector2 v;
		v.x = x/a;
		v.y = y/a;
		return v;
	}

	static float Distance(Vector2 a, Vector2 b);
};

#endif // VECTOR2_H
