#include "vector2.h"

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
	else
		y = value;
}

float Vector2::GetValue(int index)
{
	return index == 0 ? x : y;
}
