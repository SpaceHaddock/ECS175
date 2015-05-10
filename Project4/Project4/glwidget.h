#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <iostream>
#include <vector>
#include "vector2.h"
#include "color.h"
#include "pixelbuffer.h"

class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	explicit GLWidget(QWidget *parent = 0);

	//static GLWidget* screen;

	void initializeGL();
	void BuildWindow();
	void DrawCurves();
	void paintGL();
	void resizeGL(int w, int h);

	void DrawLine(const Vector2 &a, const Vector2 &b);
	void DrawLine(const Vector2 &a, const Vector2 &b, const Color&, const Color&);
	void DrawLine(const Vector2 &a, const Vector2 &b, const Color&, const Color&, PixelBuffer&);
	void DrawSquare(const Vector2 &a, const int size, Color in_color);

	void AddBezier();
	void AddSpline();
	void DeleteSelectedCurve();
	void AddVertex();
	void DeleteSelectedVertex();
	void SplineDegreeChanged(int val);
	void SetKnot(int index, double value);

	PixelBuffer buffer;
	int plane[2];

	bool is_selecting;
	bool is_selected_spline;
	int selected_curve;
	int selected_vertex;
	void MouseClick(QPoint q);
	void MouseMove(QPoint q);
	void MouseRelease();
};

#endif // GLWIDGET_H
