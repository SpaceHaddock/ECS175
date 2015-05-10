#include "vector3.h"
#include "math.h"

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float in_x, float in_y, float in_z)
{
	x = in_x;
	y = in_y;
	z = in_z;
}

void Vector3::SetValue(int index, float value)
{
	switch(index)
	{
	case 0:
		x = value;
		break;
	case 1:
		y = value;
		break;
	default:
		z = value;
	}
}

float Vector3::GetValue(int index)
{

	switch(index)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		return z;
	}
}

float Vector3::Length()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3 Vector3::Normalized()
{
	return this->Divide(Length());
}
