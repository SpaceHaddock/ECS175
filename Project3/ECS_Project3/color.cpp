#include "color.h"
#include "global.h"

Color::Color()
{
}

Color::Color(float in_r, float in_g, float in_b)
{
	r = in_r;
	g = in_g;
	b = in_b;
}

Color Color::true_color()
{
	Color result = Color(r * intensity, g * intensity, b * intensity);
	result.intensity = 1;
	return result;
}

Color Color::Normalize(const Vector2& intensity_extremes) const
{
	float normal_coefficient = Global::Lerp01(intensity_extremes.x, intensity_extremes.y, intensity);
	Color new_color = Color(r, g, b);
	new_color.intensity = normal_coefficient;
	return new_color;
}

Color Color::NormalizedColor(const Vector2& intensity_extremes) const
{
	return Normalize(intensity_extremes).true_color();
}

Color Color::Lerp(const Color &a, const Color &b, const float &amt)
{
	Color result;
	result.r = Global::Lerp(a.r, b.r, amt);
	result.g = Global::Lerp(a.g, b.g, amt);
	result.b = Global::Lerp(a.b, b.b, amt);
	result.intensity = Global::Lerp(a.intensity, b.intensity, amt);
	return result;
}
