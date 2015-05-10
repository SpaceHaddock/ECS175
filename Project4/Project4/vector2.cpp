#include "vector2.h"
#include "stdexcept"
#include "math.h"
#include "global.h"

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float in_x, float in_y)
{
	x = in_x;
	y = in_y;
}

void Vector2::SetValue(int index, float value)
{
	if(index == 0)
		x = value;
	else if (index == 1)
		y = value;
	else
		throw std::out_of_range("Vector2 got index larger than 1");
}

float Vector2::GetValue(const int index) const
{
	float result = 0;
	if(index == 0)
		result = x;
	else if (index == 1)
		result = y;
	else
		throw std::out_of_range("Vector2 got index larger than 1");
	return result;
}

float Vector2::Length() const
{
	return sqrt(pow(x, 2) + pow(y, 2));
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
	return (a-b).Length();
}


Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, const float val)
{
	Vector2 result;
	for(int i=0; i<2; i++)
		result.SetValue(i, Global::Lerp(a.GetValue(i), b.GetValue(i), val));
	return result;
}
