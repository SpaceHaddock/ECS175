#ifndef COLOR_H
#define COLOR_H

#include "vector2.h"

class Color
{
public:
	Color();
	Color(float in_r, float in_g, float in_b);

	float r;
	float g;
	float b;

	static Color Lerp(const Color &a, const Color &b, const float &amt);

	static Color Red();
	static Color Green();
	static Color Blue();
	static Color DarkGreen();
};

#endif // COLOR_H
