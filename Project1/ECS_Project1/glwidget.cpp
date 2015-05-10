#include "glwidget.h"
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

GLWidget* GLWidget::screen;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
	//Setup static values
	screen = this;
	width = 500;
	height = 500;
	x_min = 0;
	x_max = 10;
	y_min = 0;
	y_max = 10;

	//Read in polygons
	string line;
	ifstream myfile("input.txt");
	if(myfile.is_open())
	{
		while(getline(myfile, line))
		{
			if(line[0] == '\\')
			{
				polygons.push_back(Polygon());
				polygons.back().name = line.substr(1, line.length() - 1);
			}
			else
			{
				int space_index = line.find(' ');

				polygons.back().vertices.push_back(
							Vector2(
								atof(line.substr(0, space_index).c_str()),
								atof(line.substr(space_index + 1, line.length() - space_index + 1).c_str())
								));
			}
		}
		myfile.close();
	}

	BuildWindow();
}

void GLWidget::initializeGL(){
	//Finish up
	DrawPolygons();
}

void GLWidget::BuildWindow()
{
	delete[] pixels;
	pixels = new float[width * height * 3];

	//Setup background color
	glClearColor(.2, .2, .2, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	DrawPolygons();
}

void GLWidget::DrawPolygons()
{
	//Background
	for(int i=0; i<(width * height * 3); i++)
	{
		pixels[i] = 1;
	}

	//Clipping
	vector<Polygon> render_polygons;
	for(int i=0; (unsigned) i<polygons.size(); i++)
	{
		render_polygons.push_back(polygons[i]);
		for(int q=0; q<4; q++)
		{
			Vector2 axis_a, axis_b;
			switch(q)
			{
			case 0:
				axis_a.x = x_min;
				axis_a.y = y_min;
				axis_b.x = x_min;
				axis_b.y = x_max;
				break;
			case 1:
				axis_a.x = x_min;
				axis_a.y = y_max;
				axis_b.x = x_max;
				axis_b.y = y_max;
				break;
			case 2:
				axis_a.x = x_max;
				axis_a.y = y_max;
				axis_b.x = x_max;
				axis_b.y = y_min;
				break;
			default:
				axis_a.x = x_max;
				axis_a.y = y_min;
				axis_b.x = x_min;
				axis_b.y = y_min;
				break;

			}

			vector<Vector2> input = render_polygons.back().vertices;
			render_polygons.back().vertices.clear();

			if(input.empty()) break;
			Vector2 s = input.back();
			for(int j=0; (unsigned) j<input.size(); j++)
			{
				if(
						(q == 0  && input[j].x >= x_min)
						|| (q == 1 && input[j].y <= y_max)
						|| (q == 2 && input[j].x <= x_max)
						|| (q == 3 && input[j].y >= y_min)
					)
				{
					if(
							(q == 0  && s.x < x_min)
							|| (q == 1 && s.y > y_max)
							|| (q == 2 && s.x > x_max)
							|| (q == 3 && s.y < y_min)
							)
					{
						Vector2 result;
						if(GetIntersect(s, input[j], axis_a, axis_b, &result))
							render_polygons.back().vertices.push_back(result);
					}
					render_polygons.back().vertices.push_back(input[j]);
				}
				else if(
						(q == 0  && s.x > x_min)
						|| (q == 1 && s.y < y_max)
						|| (q == 2 && s.x < x_max)
						|| (q == 3 && s.y > y_min)
						)
				{
					Vector2 result;
					if(GetIntersect(s, input[j], axis_a, axis_b, &result))
						render_polygons.back().vertices.push_back(result);
				}
				s = input[j];
			}
		}
	}

	//Raster
	for(int i=0; (unsigned) i<render_polygons.size(); i++)
	{
		vector<Vector2> vertices;
		for(int j=0; (unsigned) j<render_polygons[i].vertices.size(); j++)
			vertices.push_back(ConvertToPixel(&render_polygons[i].vertices[j]));
		for(int j=0; j<height; j++)
		{
			if(vertices.empty()) break;
			vector<Vector2> draw_points;
			Vector2 previous = vertices.back();
			for(int k=0; (unsigned) k<vertices.size(); k++)
			{
				if(j == previous.y && j == vertices[k].y)
				{
					draw_points.push_back(previous);
					draw_points.push_back(vertices[k]);
				}
				else if(j == vertices[k].y)
				{
					if((unsigned) k+1 < vertices.size()) //this line prevents a duplicate point glitch
					{
						draw_points.push_back(vertices[k]);
						//Check if max or minima
						if(!(
									(vertices[k].y > previous.y && vertices[k].y > vertices[k+1].y)
									|| (vertices[k].y < previous.y && vertices[k].y < vertices[k+1].y)))
							draw_points.push_back(vertices[k]);
					}
				}
				else
				{
					Vector2 result;
					if(GetIntersect(previous, vertices[k], Vector2(0, j), Vector2(1, j), &result) && result.x >= 0 && result.x <= width)
					{
						draw_points.push_back(result);
					}
				}
				previous = vertices[k];
			}

			sort(draw_points.begin(), draw_points.end());

			//Draw
			if(!draw_points.empty())
			{
				for(int k=0; (unsigned) k<draw_points.size() - 1; k+=2)
				{
					LineDDA(round(draw_points[k].x), round(draw_points[k].y),
							round(draw_points[k+1].x), round(draw_points[k+1].y));
				}
			}
		}
	}
	repaint();
}

void GLWidget::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
}

void GLWidget::resizeGL(int w, int h){

}

//Colors one pixel, results don't show up until next paintGL is called
void GLWidget::ColorPixel(int x, int y)
{
	int first_index = (min(x, width - 1) + min(y, height - 1) * width) * 3;
	if(first_index > height*width*3) return;
	for(int i=0; i<3; i++)
	{
		pixels[i+first_index] = 0;
	}
}

//Line drawing algorithms, copied from the book
void GLWidget::LineDDA(int x_start, int y_start, int x_end, int y_end)
{
	int dx = x_end - x_start;
	int dy = y_end - y_start;

	float x_pos = x_start;
	float y_pos = y_start;

	int steps = abs(dx) > abs(dy) ?
				abs(dx) : abs(dy);

	float x_increment = dx / (float) steps;
	float y_increment = dy / (float) steps;

	for(int i=0; i < steps + 1; i++)
	{
		ColorPixel(round(x_pos), round(y_pos));
		x_pos += x_increment;
		y_pos += y_increment;
	}
}

void GLWidget::LineBresenham(int xa, int ya, int xb, int yb)
{
	const bool steep = (fabs(yb - ya) > fabs(xb - xa));
	if(steep)
	{
		std::swap(xa, ya);
		std::swap(xb, yb);
	}

	if(xa > xb)
	{
		std::swap(xa, xb);
		std::swap(ya, yb);
	}

	const float dx = xb - xa;
	const float dy = fabs(yb - ya);

	float error = dx / 2.0f;
	const int ystep = (ya < yb) ? 1 : -1;
	int y = (int)ya;

	const int maxX = (int)xb;

	for(int x=(int)xa; x<maxX; x++)
	{
		if(steep)
		{
			ColorPixel(y,x);
		}
		else
		{
			ColorPixel(x,y);
		}

		error -= dy;
		if(error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

//Converts worldspace location to pixelspace location
Vector2 GLWidget::ConvertToPixel(Vector2* input)
{
	Vector2 result;

	float dx = x_max - x_min;
	float dy = y_max - y_min;

	float max_d = max(dx, dy);
	float ox = dx/max_d;
	float oy = dy/max_d;

	result.x = (input->x - x_min) / max_d;
	result.y = (input->y - y_min) / max_d;

	float t_max_d = max(width, height);
	float tx = width / t_max_d;
	float ty = height / t_max_d;
	float t_min = min(tx/ox, ty/oy);

	result.x *= t_min * t_max_d;
	result.y *= t_min * t_max_d;

	//Round
	result.x = round(result.x);
	result.y = round(result.y);

	return result;
}

//Returns bool for whether or not the two lines intersect, saves point of intersection in result vector
bool GLWidget::GetIntersect(Vector2 a_start, Vector2 a_end, Vector2 b_start, Vector2 b_end, Vector2* result)
{
	Vector2 direction1(a_start.x - a_end.x, a_start.y - a_end.y);
	Vector2 direction2(b_start.x - b_end.x, b_start.y - b_end.y);
	float dotPerp = (direction1.x * direction2.y) - (direction1.y * direction2.x);

	// If it's 0, it means the lines are parallel so have infinite intersection points
	if (!(dotPerp > .0001 || dotPerp < -.0001))
	{
		return false;
	}

	Vector2 c(b_start.x - a_start.x, b_start.y - a_start.y);
	float t = (c.x * direction2.y - c.y * direction2.x) / dotPerp;

	//	Return the intersection point
	result->x = a_start.x + (t * direction1.x);
	result->y = a_start.y + (t * direction1.y);

	return (InRange(result->x, a_start.x, a_end.x) && InRange(result->y, a_start.y, a_end.y));
}

bool GLWidget::InRange(float val, float a, float b)
{
	return (val <= a || val <= b) && (val >= a || val >= b);
}

//Clipping algorithm Cohen-Sutherland, from the book
#define LEFT_EDGE 0x1
#define RIGHT_EDGE 0x2
#define BOTTOM_EDGE 0x4
#define TOP_EDGE 0x8
#define INSIDE(a) (!a)
#define REJECT(a,b) (a&b)
#define ACCEPT(a,b) (!(a|b))

unsigned char GLWidget::encode(Vector2 pt)
{
	unsigned char code = 0x00;

	if(pt.x < x_min)
		code = code | LEFT_EDGE;
	if(pt.x > x_max)
		code = code | RIGHT_EDGE;
	if(pt.y < y_min)
		code = code | BOTTOM_EDGE;
	if(pt.y < y_max)
		code = code | TOP_EDGE;
	return (code);
}

void GLWidget::swapPts(Vector2* p1, Vector2* p2)
{
	Vector2 tmp;
	tmp = *p1; *p1 = *p2; *p2 = tmp;
}

void GLWidget::swapCodes(unsigned char* c1, unsigned char* c2)
{
	unsigned char tmp;
	tmp = *c1; *c1 = *c2; *c2 = tmp;
}


void GLWidget::clipLine(Vector2 p1, Vector2 p2)
{
	unsigned char code1, code2;
	int done = 0, draw = 0;
	float m;

	while(!done){
		code1 = encode(p1);
		code2 = encode(p2);
		if(ACCEPT(code1, code2)){
			done = 1;
			draw = 1;
		}
		else
		{
			if(REJECT(code1, code2))
				done = 1;
			else{
				if(INSIDE(code1))
				{
					swapPts(&p1, &p2);
					swapCodes(&code1, &code2);
				}
				if(p2.x != p1.x)
					m = (p2.y - p1.y) / (p2.x - p1.x);
				if(code1 & LEFT_EDGE){
					p1.y += (x_min - p1.x) * m;
					p1.x = x_min;
				}
				else{
					if(code1 & RIGHT_EDGE){
						p1.y *= (x_max - p1.x) * m;
						p1.x = x_max;
					}
					else{
						if(code1 & BOTTOM_EDGE){
							if(p2.x != p1.x)
								p1.x +=(y_min - p1.y) / m;
							p1.y / y_min;
						}
						else{
							if(code1 & TOP_EDGE){
								if(p2.x != p1.x)
									p1.x += (y_max - p1.y) / m;
								p1.y = y_max;
							}
						}
					}
				}
			}
		}
	}
	if(draw)
		LineDDA(round(p1.x), round(p1.y), round(p2.x), round(p2.y));
}
