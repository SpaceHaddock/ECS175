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

Color Color::Lerp(const Color &a, const Color &b, const float &amt)
{
	Color result;
	result.r = Global::Lerp(a.r, b.r, amt);
	result.g = Global::Lerp(a.g, b.g, amt);
	result.b = Global::Lerp(a.b, b.b, amt);
	return result;
}

Color Color::Red()
{
	return Color(1, 0, 0);
}

Color Color::Green()
{
	return Color(0, 1, 0);
}

Color Color::Blue()
{
	return Color(0, 0, 1);
}

Color Color::DarkGreen()
{
	return Color(0, .5, 0);
}
