#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <iostream>
#include <vector>
#include "polygon.h"
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
	void DrawPolygons();
	void paintGL();
	void resizeGL(int w, int h);

	void DrawLine(const Vector2 &a, const Vector2 &b);
	void DrawLine(const Vector2 &a, const Vector2 &b, const Color&, const Color&);
	void DrawLine(const Vector2 &a, const Vector2 &b, const Color&, const Color&, PixelBuffer&);

	PixelBuffer buffer;
	int plane[2];
};

#endif // GLWIDGET_H
