#ifndef GLOBAL_H
#define GLOBAL_H

#include "polygon.h"
#include "vector3.h"
#include "glwidget.h"
#include <vector>
#include <string>

class Global
{
public:
	Global();

	static void Setup();
	static void Save();

	static std::vector<Polygon> polygons;
	static std::vector<Vector3> lines;
	static std::vector<GLWidget*> screens;

	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> split(const std::string &s, char delim);

	//Variables controlled by UI
	static float ambient_coefficient;
	static float diffuse_coefficient;
	static float specular_coefficient;

	static float ambient_intensity;

	static float lightsource_intensity;
	static float lightsource_distance;
	static Vector3 lightsource_position;

	static int phong_constant;
	static int half_tone_size;

	static float color_r;
	static float color_g;
	static float color_b;

	static float Clamp(const float &clamp_me);
	static float Lerp(const float &start, const float &end, const float &position);
	static float Lerp01(const float& start, const float& end, const float& position);
	static bool GetIntersect(const Vector2&, const Vector2&, const Vector2&, const Vector2&, Vector2*);
	static bool InRange(const float& val, const float &a, const float &b);
};

#endif // GLOBAL_H
