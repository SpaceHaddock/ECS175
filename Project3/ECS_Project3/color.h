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
	float intensity;

	Color true_color();
	Color Normalize(const Vector2& intensity_extremes) const;
	Color NormalizedColor(const Vector2& intensity_extremes) const;

	static Color Lerp(const Color &a, const Color &b, const float &amt);
};

#endif // COLOR_H
