#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2
{
public:
	Vector2();
	Vector2(float, float);

	float x;
	float y;

	void SetValue(int, float);
	float GetValue(int);

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
};

#endif // VECTOR2_H
