#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <iostream>
#include <vector>
#include "polygon.h"
#include "vector2.h"
#include "color.h"

class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	explicit GLWidget(QWidget *parent = 0);

	//static GLWidget* screen;

	void initializeGL();
	void BuildWindow();
	void DrawPolygons();
	void paintGL();
	void resizeGL(int w, int h);

	void ColorPixel(int x, int y);
	void ColorPixel(int x, int y, Color);

	void DrawLine(Vector2 a, Vector2 b);
	void DrawLine(Vector2 a, Vector2 b, Color);

	float* pixels; //represents the colors on the screen
	int width;
	int height;
	Vector2 min_window;
	Vector2 max_window;
	int plane[2];

	//Algorithms
	void LineDDA(int xa, int ya, int xb, int yb, Color);
	Vector2 ConvertToPixel(Vector2* input);
};

#endif // GLWIDGET_H
