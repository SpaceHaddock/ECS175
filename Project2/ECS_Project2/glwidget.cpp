#include <math.h>
#include <iostream>
#include "global.h"
#include "glwidget.h"

using namespace std;

//GLWidget* GLWidget::screen;

GLWidget::GLWidget(QWidget *parent) :
	QGLWidget(parent)
{
	//Setup static values
	width = 250;
	height = 250;
	pixels = NULL;

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
	delete[] pixels;
	pixels = new float[width * height * 3];

	//Setup background color
	glClearColor(.2, .2, .2, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	DrawPolygons();
}

void GLWidget::DrawPolygons()
{
	if(Global::polygons.empty()) return;

	//Background
	for(int i=0; i<(width * height * 3); i++)
	{
		pixels[i] = 1;
	}

	//Find min/max values for normalizing
	min_window = Vector2(0, 0);
	max_window = Vector2(1, 1);
	for(int i=0; (unsigned) i<Global::polygons.size(); i++)
	{
		for(int j=0; (unsigned) j<Global::polygons[i].vertices.size(); j++)
		{
			for(int q=0; (unsigned) q<2; q++)
			{
				float compare = Global::polygons[i].vertices[j].GetValue(plane[q]);
				min_window.SetValue(q, min(min_window.GetValue(plane[q]), compare));
				max_window.SetValue(q, max(max_window.GetValue(plane[q]), compare));
			}
		}
	}

	//Draw wireframes
	for(int i=0; (unsigned) i<Global::polygons.size(); i++)
	{
		Global::polygons[i].CalculateEdges();
		for(int j=0; (unsigned) j<Global::polygons[i].edges.size(); j++)
		{
			Edge e = Global::polygons[i].edges[j];
			Vector2 a = Vector2(e.start->GetValue(plane[0]), e.start->GetValue(plane[1]));
			Vector2 b = Vector2(e.end->GetValue(plane[0]), e.end->GetValue(plane[1]));
			DrawLine(a, b);
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
			DrawLine(a, b, Color(0, 0, 1));
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

//Takes an input of two lines in world space and draws them onto the screen
void GLWidget::DrawLine(Vector2 a, Vector2 b)
{
	DrawLine(a, b, Color(0, 0, 0));
}

void GLWidget::DrawLine(Vector2 a, Vector2 b, Color input_color)
{
	Vector2 pixela = ConvertToPixel(&a);
	Vector2 pixelb = ConvertToPixel(&b);
	LineDDA((int) pixela.x, (int) pixela.y, (int) pixelb.x, (int) pixelb.y, input_color);
}

//Colors one pixel, results don't show up until next paintGL is called
void GLWidget::ColorPixel(int x, int y)
{
	ColorPixel(x, y, Color(0, 0, 0));
}

void GLWidget::ColorPixel(int x, int y, Color color)
{
	if(x > width || x < 0 || y > height || y < 0)
		return;

	int first_index = (x + y * width) * 3;
	pixels[first_index] = color.r;
	pixels[first_index + 1] = color.g;
	pixels[first_index + 2] = color.b;
}

//Line drawing algorithms, copied from the book
void GLWidget::LineDDA(int x_start, int y_start, int x_end, int y_end, Color input_color)
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
		ColorPixel(round(x_pos), round(y_pos), input_color);
		x_pos += x_increment;
		y_pos += y_increment;
	}
}

//Converts worldspace location to pixelspace location by normalizing
Vector2 GLWidget::ConvertToPixel(Vector2* input)
{
	Vector2 result;

	//Normalize world coordinates
	float dx = max_window.x - min_window.x;
	float dy = max_window.y - min_window.y;

	float max_d = max(dx, dy);
	float wx = dx/max_d;
	float wy = dy/max_d;

	result.x = (input->x - min_window.x) / max_d;
	result.y = (input->y - min_window.y) / max_d;

	//Convert into pixel coordinates using "fit"
	float s_max_d = max(width, height);
	float sx = (width - 1) / s_max_d;
	float sy = (height - 1) / s_max_d;
	float s_min = min(sx/wx, sy/wy);

	result.x *= s_min * s_max_d;
	result.y *= s_min * s_max_d;

	//Round
	result.x = round(result.x);
	result.y = round(result.y);

	return result;
}
