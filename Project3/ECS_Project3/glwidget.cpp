#include <math.h>
#include <iostream>
#include "global.h"
#include "glwidget.h"
#include "colorpoint.h"

using namespace std;

//GLWidget* GLWidget::screen;

GLWidget::GLWidget(QWidget *parent) :
	QGLWidget(parent)
{
	//Setup static values
	buffer.pixel_width = 250;
	buffer.pixel_height = 250;
	buffer.pixels = NULL;

	if(Global::screens.empty())
		Global::Setup();

	Global::screens.push_back(this);
	switch(Global::screens.size())
	{
	case 1:
		plane[0] = 0;
		plane[1] = 1;
		break;
	case 2:
		plane[0] = 0;
		plane[1] = 2;
		break;
	case 3:
		plane[0] = 1;
		plane[1] = 2;
		break;
	}

	BuildWindow();
}

void GLWidget::initializeGL(){
	//Finish up
	DrawPolygons();
}

void GLWidget::BuildWindow()
{
	delete[] buffer.pixels;
	buffer.pixels = new float[buffer.pixel_width * buffer.pixel_height * 3];

	//Setup background color
	glClearColor(.2, .2, .2, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	DrawPolygons();
}

void GLWidget::DrawPolygons()
{
	if(Global::polygons.empty()) return;

	//Background
	for(int i=0; i<(buffer.pixel_width * buffer.pixel_height * 3); i++)
	{
		buffer.pixels[i] = 1;
	}

	//Calculate triangles
	for(int i=0; (unsigned) i<Global::polygons.size(); i++)
		Global::polygons[i].CalculateTriangles();

	//Find min/max values for normalizing
	buffer.min_window = Vector2(0, 0);
	buffer.max_window = Vector2(1, 1);
	for(int i=0; (unsigned) i<Global::polygons.size(); i++)
	{
		for(int j=0; (unsigned) j<Global::polygons[i].vertices.size(); j++)
		{
			for(int k=0; (unsigned) k<2; k++)
			{
				float compare = Global::polygons[i].vertices[j].GetValue(plane[k]);
				buffer.min_window.SetValue(k, min(buffer.min_window.GetValue(k), compare));
				buffer.max_window.SetValue(k, max(buffer.max_window.GetValue(k), compare));
			}
		}
	}

	//Calculate "from point" for this viewport
	//The from point will be the center of the viewport
	Vector3 from_point = Vector3(0,0,0);
	for(int i=0; i<2; i++)
		from_point.SetValue(plane[i],
							(buffer.min_window.GetValue(i) + buffer.max_window.GetValue(i)) / 2);

	//Calculate color of each vertex
	buffer.intensity_extremes = Vector2(-1, -1);
	for(int i=0; (unsigned) i<Global::polygons.size(); i++)
	{
		Global::polygons[i].vertex_colors.clear();
		for(int j=0; (unsigned) j<Global::polygons[i].vertices.size(); j++)
		{
			float parta = Global::ambient_coefficient * Global::ambient_intensity;

			float fpminus = (from_point - Global::polygons[i].vertices[j]).Length();
			float partb = Global::lightsource_intensity/(fpminus + Global::lightsource_distance);

			//Calculate normal of vertex
			int tri_count = 0;
			Vector3 average = Vector3(0,0,0);
			for(int k=0; (unsigned) k<Global::polygons[i].triangles.size(); k++)
			{
				Triangle* t = &Global::polygons[i].triangles[k];
				if(t->indices[0] == k || t->indices[1] == k || t->indices[2] == k)
				{
					average = average + t->Normal();
					tri_count++;
				}
			}
			average = average.Divide(tri_count);

			Vector3 light_vector = (Global::lightsource_position - Global::polygons[i].vertices[j]).Normalized();
			Vector3 normal_vector = average;
			float partc = Global::diffuse_coefficient * light_vector.DotProduct(normal_vector);

			Vector3 reflection_vector = normal_vector*2*(light_vector.DotProduct(normal_vector)) - light_vector;
			Vector3 viewing_vector = (from_point-Global::polygons[i].vertices[j]).Normalized();
			float partd = pow(Global::specular_coefficient * reflection_vector.DotProduct(viewing_vector), Global::phong_constant);

			//Handle special cases
			if(
					(normal_vector.DotProduct(light_vector) > 0 && normal_vector.DotProduct(viewing_vector) < 0)
					|| (normal_vector.DotProduct(light_vector) < 0 && normal_vector.DotProduct(viewing_vector) > 0))
			{
				partb = 0;
			}
			else if(reflection_vector.DotProduct(viewing_vector) < 0)
				partd = 0;

			Color new_color = Color(Global::color_r, Global::color_g, Global::color_b);
			float new_intensity = parta + partb * (partc + partd);
			if(i == 0 && j == 0)
				buffer.intensity_extremes = Vector2(new_intensity, new_intensity);
			buffer.intensity_extremes.x = min(new_intensity, buffer.intensity_extremes.x);
			buffer.intensity_extremes.y = max(new_intensity, buffer.intensity_extremes.y);
			new_color.intensity = new_intensity;
			Global::polygons[i].vertex_colors.push_back(new_color);
		}
	}

	//Draw and raster each triangle, one at a time
	//Use painter's algorithm
	Triangle::sort_priority = 1;
	if(plane[0] == 0 && plane[1] == 1)
		Triangle::sort_priority = 2;
	else if(plane[0] == 1)
		Triangle::sort_priority = 0;

	for(int i=0; (unsigned) i<Global::polygons.size(); i++)
	{
		sort(Global::polygons[i].triangles.begin(), Global::polygons[i].triangles.end());
		for(int j=0; (unsigned) j<Global::polygons[i].triangles.size(); j++)
		{
			//Raster
			//Get pixelspace and color for vertices
			vector<ColorPoint> vertices;
			for(int k=0; (unsigned) k<3; k++)
			{
				int vertex_index = Global::polygons[i].triangles[j].indices[k];
				Vector3 v = Global::polygons[i].vertices[vertex_index];
				Vector2 v2 = Vector2(v.GetValue(plane[0]), v.GetValue((plane[1])));
				v2 = buffer.ConvertToPixel(&v2);

				ColorPoint vertex_color;
				vertex_color.position = v2;
				vertex_color.color = Global::polygons[i].vertex_colors[vertex_index];
				vertices.push_back(vertex_color);
			}

			for(int k=0; k<buffer.height(); k++)
			{
				if(vertices.empty()) break;
				vector<ColorPoint> draw_points;
				ColorPoint previous = vertices.back();
				for(int l=0; (unsigned) l<vertices.size(); l++)
				{
					if(k == previous.position.y && k == vertices[l].position.y) //two vertices on one line
					{
						draw_points.push_back(previous);
						draw_points.push_back(vertices[l]);
					}
					else if(k == vertices[l].position.y)
					{
						Vector2 next = vertices[((l+1) % vertices.size())].position;
						//Check if max or minima
						if(!(
									(vertices[l].position.y > previous.position.y && vertices[l].position.y > next.y)
									|| (vertices[l].position.y < previous.position.y && vertices[l].position.y < next.y)))
							draw_points.push_back(vertices[l]);
					}
					else
					{
						Vector2 result;
						if(Global::GetIntersect(previous.position, vertices[l].position, Vector2(0, k), Vector2(1, k), &result)
								&& result.x >= 0 && result.x <= buffer.width())
						{
							//Make sure this isn't a vertex point
							if(result != previous.position && result != vertices[l].position)
							{
								ColorPoint intersection;
								intersection.position = result;
								intersection.color = Color::Lerp(previous.color, vertices[l].color,
																 Vector2::Distance(result, previous.position) /
																 Vector2::Distance(previous.position, vertices[l].position));
								draw_points.push_back(intersection);
							}
						}
					}
					previous = vertices[l];
				}

				sort(draw_points.begin(), draw_points.end());

				//Draw
				if(!draw_points.empty() && draw_points.size() != 1)
				{
					for(int l=0; (unsigned) l<draw_points.size() - l; l+=2)
					{
						buffer.LineDDA(round(draw_points[l].position.x), round(draw_points[l].position.y),
								round(draw_points[l+1].position.x), round(draw_points[l+1].position.y),
								draw_points[l].color,
								draw_points[l+1].color);
					}
				}
			}
		}
	}

	//Draw non-polygon lines
	if(!Global::lines.empty())
	{
		for(int i=0; (unsigned) i<Global::lines.size() - 1; i += 2)
		{
			Vector2 a = Vector2(Global::lines[i].GetValue(plane[0]), Global::lines[i].GetValue(plane[1]));
			Vector2 b = Vector2(Global::lines[i+1].GetValue(plane[0]), Global::lines[i+1].GetValue(plane[1]));
			b.x *= 100;
			b.y *= 100;
			DrawLine(a, b, Color(0, 0, 1), Color(0, 0, 1));
		}
	}

	repaint();
}

void GLWidget::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(buffer.pixel_width, buffer.pixel_height, GL_RGB, GL_FLOAT, buffer.pixels);
}

void GLWidget::resizeGL(int w, int h){

}

//Takes an input of two lines in world space and draws them onto the screen
void GLWidget::DrawLine(const Vector2 &a, const Vector2 &b)
{
	DrawLine(a, b, Color(0, 0, 0), Color(0, 0, 0));
}

void GLWidget::DrawLine(const Vector2 &a, const Vector2 &b, const Color &color_a, const Color &color_b)
{
	DrawLine(a, b, color_a, color_b, buffer);
}

void GLWidget::DrawLine(const Vector2 &a, const Vector2 &b, const Color &color_a, const Color &color_b, PixelBuffer& in_buffer)
{
	Vector2 pixela = in_buffer.ConvertToPixel(&a);
	Vector2 pixelb = in_buffer.ConvertToPixel(&b);
	in_buffer.LineDDA((int) pixela.x, (int) pixela.y, (int) pixelb.x, (int) pixelb.y, color_a, color_b);
}
