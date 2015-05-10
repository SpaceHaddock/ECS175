#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3
{
public:
	Vector3();
	Vector3(float, float, float);

	float x;
	float y;
	float z;

	void SetValue(int, float);
	float GetValue(int);
	float Length();
	Vector3 Normalized();

	Vector3 operator-() const
	{
	   Vector3 v;
	   v.x = -x;
	   v.y = -y;
	   v.z = -z;
	   return v;
	}

	Vector3 operator+(const Vector3 &a)
	{
		Vector3 v;
		v.x = x + a.x;
		v.y = y + a.y;
		v.z = z + a.z;
		return v;
	}

	Vector3 operator-(const Vector3 &a)
	{
		Vector3 v;
		v.x = x - a.x;
		v.y = y - a.y;
		v.z = z - a.z;
		return v;
	}

	Vector3 Divide(const float a)
	{
		Vector3 v;
		v.x = x/a;
		v.y = y/a;
		v.z = z/a;
		return v;
	}
};

#endif // VECTOR3_H
