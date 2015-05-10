#include <stdexcept>
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
	case 2:
		z = value;
		break;
	default:
		throw std::out_of_range("Vector2 got index larger than 1");
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
	case 2:
		return z;
	default:
		throw std::out_of_range("Vector3 got index larger than 2");
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

float Vector3::DotProduct(Vector3 other)
{
	return x*other.x + y*other.y + z*other.z;
}

Vector3 Vector3::CrossProduct(Vector3 other)
{
	return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}
