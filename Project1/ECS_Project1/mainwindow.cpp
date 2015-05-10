#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <iostream>
#include <fstream>
#include "math.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	for(int i=0; (unsigned) i<GLWidget::screen->polygons.size(); i++)
	{
		ui->comboBox->addItem(QString::fromStdString(GLWidget::screen->polygons[i].name));
	}
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	GLWidget::screen->polygons[ui->comboBox->currentIndex()].Translate(
				Vector2(ui->translatex->text().toFloat(),
						ui->translatey->text().toFloat()));

	GLWidget::screen->polygons[ui->comboBox->currentIndex()].ScaleCenter(
				Vector2(ui->scalex->text().toFloat(),
						ui->scaley->text().toFloat()));
	GLWidget::screen->polygons[ui->comboBox->currentIndex()].RotateCenter(ui->rotate->text().toFloat());
	GLWidget::screen->DrawPolygons();
}

void MainWindow::on_save_clicked()
{
	std::ofstream myfile ("input.txt");
	if (myfile.is_open())
	{
		for(int i=0; (unsigned) i<GLWidget::screen->polygons.size(); i++)
		{
			Polygon* poly = &GLWidget::screen->polygons[i];
			myfile << "\\" << poly->name << "\n";
			for(int j=0; (unsigned) j<poly->vertices.size(); j++)
			{
				myfile << poly->vertices[j].x << " " << poly->vertices[j].y << "\n";
			}
		}
	}
}

void MainWindow::on_pushButton_2_clicked()
{
	GLWidget::screen->width = ui->viewportx->text().toFloat();
	GLWidget::screen->height = ui->viewporty->text().toFloat();
	GLWidget::screen->x_min = ui->xmin->text().toFloat();
	GLWidget::screen->x_max = ui->xmax->text().toFloat();
	GLWidget::screen->y_min = ui->ymin->text().toFloat();
	GLWidget::screen->y_max = ui->ymax->text().toFloat();
	GLWidget::screen->BuildWindow();
}

void MainWindow::on_pushButton_3_clicked()
{
	Polygon p;
	p.name = ui->polygonname->text().toStdString();
	QStringList qlist = ui->polygonvertices->text().split(") ");
	for(int i=0; i<qlist.size(); i++)
	{
		qlist[i].remove("(");
		qlist[i].remove(")");

		Vector2 v;
		QStringList q = qlist[i].split(",");
		v.x = q[0].toFloat();
		v.y = q[1].toFloat();
		p.vertices.push_back(v);
	}

	GLWidget::screen->polygons.push_back(p);
	ui->comboBox->addItem(ui->polygonname->text());
	GLWidget::screen->DrawPolygons();
}

void MainWindow::on_drawline_clicked()
{
	QStringList qa = ui->linepointa->text().split(", ");
	Vector2 pointa(qa[0].toFloat(), qa[1].toFloat());

	QStringList qb = ui->linepointb->text().split(", ");
	Vector2 pointb(qb[0].toFloat(), qb[1].toFloat());

	if(ui->ddabutton->isChecked())
	{
		GLWidget::screen->LineDDA(round(pointa.x), round(pointa.y), round(pointb.x), round(pointb.y));
	}
	else
	{
		GLWidget::screen->LineBresenham(round(pointa.x), round(pointa.y), round(pointb.x), round(pointb.y));
	}
	GLWidget::screen->repaint();
}
