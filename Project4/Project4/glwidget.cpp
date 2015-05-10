#include <math.h>
#include <iostream>
#include "global.h"
#include "glwidget.h"
#include "colorpoint.h"
#include "mainwindow.h"

using namespace std;

GLWidget::GLWidget(QWidget *parent) :
	QGLWidget(parent)
{
	//Setup static values
	buffer.pixel_width = 500;
	buffer.pixel_height = 500;
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

	selected_vertex = -1;
	is_selecting = false;

	BuildWindow();
}

void GLWidget::initializeGL(){
	//Finish up
	DrawCurves();
}

void GLWidget::BuildWindow()
{
	delete[] buffer.pixels;
	buffer.pixels = new float[buffer.pixel_width * buffer.pixel_height * 3];

	//Setup background color
	glClearColor(.2, .2, .2, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	DrawCurves();
}

void GLWidget::DrawCurves()
{
	if(Global::beziers.empty() && Global::splines.empty()) return;

	//Background
	for(int i=0; i<(buffer.pixel_width * buffer.pixel_height * 3); i++)
	{
		buffer.pixels[i] = 1;
	}

	//Find min/max values for normalizing
	buffer.min_window = Vector2(0, 0);
	buffer.max_window = Vector2(1, 1);

	//Draw beziers
	if(!Global::beziers.empty())
	{
		for(int i=0; (unsigned) i<Global::beziers.size(); i++)
		{
			//Draw control polygon
			Bezier* b = &Global::beziers[i];
			for(int j=0; (unsigned) j<b->polygon_points.size()-1; j++)
				DrawLine(b->polygon_points[j], b->polygon_points[j+1], Color::Green(), Color::Green());

			//Draw control polygon points
			for(int j=0; (unsigned) j<b->polygon_points.size(); j++)
				DrawSquare(b->polygon_points[j], 4, Color::DarkGreen());

			//Draw curve
			vector<Vector2> calc_points = b->CalculatedPoints();
			for(int j=0; (unsigned) j<calc_points.size()-1; j++)
				DrawLine(calc_points[j], calc_points[j+1], Color::Red(), Color::Red());
		}
	}

	//Draw splines
	if(!Global::splines.empty())
	{
		for(int i=0; (unsigned) i<Global::splines.size(); i++)
		{
			//Draw control polygon
			Spline* b = &Global::splines[i];
			for(int j=0; (unsigned) j<b->polygon_points.size()-1; j++)
				DrawLine(b->polygon_points[j], b->polygon_points[j+1], Color::Green(), Color::Green());

			//Draw control polygon points
			for(int j=0; (unsigned) j<b->polygon_points.size(); j++)
				DrawSquare(b->polygon_points[j], 4, Color::DarkGreen());

			//Draw curve
			vector<Vector2> calc_points = b->CalculatedPoints();
			for(int j=0; (unsigned) j<calc_points.size()-1; j++)
				DrawLine(calc_points[j], calc_points[j+1], Color::Blue(), Color::Blue());
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

//Take input of point and size, draw square of that size
void GLWidget::DrawSquare(const Vector2 &a, const int size, Color in_color)
{
	Vector2 pixela = buffer.ConvertToPixel(&a);

	int x = pixela.x;
	int y = pixela.y;
	for(int i=x-size; i<=x+size; i++)
	{
		for(int j=y-size; j<=y+size; j++)
		{
			buffer.ColorPixel(i, j, in_color);
		}
	}
}

void GLWidget::MouseClick(QPoint q)
{
	QPoint p = QWidget::mapFromGlobal(q);
	Vector2 click_location = Vector2((float) p.x(), (float) p.y());
	click_location = click_location.Divide(500);
	click_location.y = 1 - click_location.y;

	if(!Global::beziers.empty())
	{
		for(int i=0; (unsigned) i<Global::beziers.size(); i++)
		{
			for(int j=0; (unsigned) j<Global::beziers[i].polygon_points.size(); j++)
			{
				if(Vector2::Distance(click_location, Global::beziers[i].polygon_points[j]) < .05)
				{
					is_selected_spline = false;
					is_selecting = true;
					selected_curve = i;
					selected_vertex = j;
					MainWindow::mywindow->NewSpline(0, vector<float>());
				}
			}
		}
	}

	if (!Global::splines.empty())
	{
		for(int i=0; (unsigned) i<Global::splines.size(); i++)
		{
			for(int j=0; (unsigned) j<Global::splines[i].polygon_points.size(); j++)
			{
				Spline* s = &Global::splines[i];
				if(Vector2::Distance(click_location, s->polygon_points[j]) < .05)
				{
					is_selected_spline = true;
					is_selecting = true;
					selected_curve = i;
					selected_vertex = j;
					MainWindow::mywindow->NewSpline(s->degree, s->knots);
				}
			}
		}
	}
}

void GLWidget::MouseMove(QPoint q)
{
	if(is_selecting && selected_vertex >= 0)
	{
		QPoint p = QWidget::mapFromGlobal(q);
		Vector2 click_location = Vector2((float) p.x(), (float) p.y());
		click_location = click_location.Divide(500);
		click_location.y = 1 - click_location.y;
		click_location.x = Global::Clamp(click_location.x);
		click_location.y = Global::Clamp(click_location.y);

		if(is_selected_spline)
			Global::splines[selected_curve].polygon_points[selected_vertex] = click_location;
		else
			Global::beziers[selected_curve].polygon_points[selected_vertex] = click_location;

		DrawCurves();
	}
}

void GLWidget::MouseRelease()
{
	is_selecting = false;
}

void GLWidget::AddBezier()
{
	Bezier b;
	b.name = "New Bezier";
	b.polygon_points = vector<Vector2>();
	b.polygon_points.push_back(Vector2(.3, .3));
	b.polygon_points.push_back(Vector2(.5, .5));
	b.polygon_points.push_back(Vector2(.6, .3));
	Global::beziers.push_back(b);
	DrawCurves();
}

void GLWidget::AddSpline()
{
	Spline b;
	b.name = "New Spline";
	b.degree = 2;
	b.polygon_points = vector<Vector2>();
	b.polygon_points.push_back(Vector2(.3, .3));
	b.polygon_points.push_back(Vector2(.4, .5));
	b.polygon_points.push_back(Vector2(.5, .5));
	b.polygon_points.push_back(Vector2(.6, .3));
	b.SetupKnots();
	Global::splines.push_back(b);
	DrawCurves();
}

void GLWidget::DeleteSelectedCurve()
{
	if(selected_vertex > -1)
	{
		if(is_selected_spline)
			Global::splines.erase(Global::splines.begin() + selected_curve);
		else
			Global::beziers.erase(Global::beziers.begin() + selected_curve);
		selected_vertex = -1;
		DrawCurves();
	}
}

void GLWidget::DeleteSelectedVertex()
{
	if(selected_vertex > -1)
	{
		if(is_selected_spline)
			Global::splines[selected_curve].DeletePoint(selected_vertex);
		else
			Global::beziers[selected_curve].DeletePoint(selected_vertex);
		selected_vertex = -1;
		DrawCurves();
	}
}


void GLWidget::AddVertex()
{
	if(selected_vertex > -1)
	{
		if(is_selected_spline)
			Global::splines[selected_curve].AddPoint(selected_vertex);
		else
			Global::beziers[selected_curve].AddPoint(selected_vertex);
		DrawCurves();
	}
}

void GLWidget::SplineDegreeChanged(int val)
{
	if(is_selected_spline && selected_vertex > -1)
	{
		Global::splines[selected_curve].SetDegree(val);
		DrawCurves();
	}
}

void GLWidget::SetKnot(int index, double value)
{
	if(selected_vertex > -1 && is_selected_spline)
	{
		Global::splines[selected_curve].knots[index] = value;
		DrawCurves();
	}
}
