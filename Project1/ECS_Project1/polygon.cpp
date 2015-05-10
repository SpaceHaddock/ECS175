#include "polygon.h"
#include "math.h"

Polygon::Polygon()
{
}

void Polygon::Translate(Vector2 translate_amount)
{
	if(vertices.empty()) return;

	for(int i=0; i<vertices.size(); i++)
	{
		vertices[i].x += translate_amount.x;
		vertices[i].y += translate_amount.y;
	}
}

void Polygon::Scale(Vector2 scale_amount)
{
	if(vertices.empty()) return;

	for(int i=0; i<vertices.size(); i++)
	{
		vertices[i].x *= scale_amount.x;
		vertices[i].y *= scale_amount.y;
	}
}

void Polygon::Rotate(float rotate_amount)
{
	if(vertices.empty()) return;

	for(int i=0; i<vertices.size(); i++)
	{
		float convert = 3.14159/180;
		float new_x = cos(rotate_amount * convert) * vertices[i].x - sin(rotate_amount * convert) * vertices[i].y;
		float new_y = sin(rotate_amount * convert) * vertices[i].x + cos(rotate_amount * convert) * vertices[i].y;

		vertices[i].x = new_x;
		vertices[i].y = new_y;
	}

}

void Polygon::ScaleCenter(Vector2 scale_amount)
{
	if(vertices.empty()) return;
	Vector2 centroid = Centroid();
	Translate(-centroid);
	Scale(scale_amount);
	Translate(centroid);
}

void Polygon::RotateCenter(float rotate_amount)
{
	if(vertices.empty()) return;
	Vector2 centroid = Centroid();
	Translate(-centroid);
	Rotate(rotate_amount);
	Translate(centroid);
}

Vector2 Polygon::Centroid()
{
	Vector2 result;
	for(int i=0; i<vertices.size(); i++)
	{
		result.x += vertices[i].x;
		result.y += vertices[i].y;
	}
	result.x /= vertices.size();
	result.y /= vertices.size();

	return result;
}
