#ifndef COLORPOINT_H
#define COLORPOINT_H

#include "color.h"
#include "vector2.h"

class ColorPoint
{
public:
	ColorPoint();

	Color color;
	Vector2 position;

	bool operator < (const ColorPoint& other) const
	{
		return (position.x < other.position.x);
	}
};

#endif // COLORPOINT_H
