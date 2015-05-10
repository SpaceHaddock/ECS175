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
	float GetValue(int);
	float Length();

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

	bool operator==(const Vector2 &a)
	{
		return round(x) == round(a.x) && round(y) == round(a.y);
	}

	bool operator!=(const Vector2 &a)
	{
		return !(*this == a);
	}

	static float Distance(Vector2 a, Vector2 b);
};

#endif // VECTOR2_H
