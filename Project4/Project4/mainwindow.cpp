#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include "math.h"

MainWindow* MainWindow::mywindow;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	mywindow = this;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
	Global::screens[0]->MouseMove(QCursor::pos());
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
	Global::screens[0]->MouseClick(QCursor::pos());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	Global::screens[0]->MouseRelease();
}

void MainWindow::on_resolutinonSpin_valueChanged(int arg1)
{
	Global::display_resolution = arg1;
	Global::screens[0]->DrawCurves();
}

void MainWindow::on_deleteCurve_clicked()
{
	Global::screens[0]->DeleteSelectedCurve();
}

void MainWindow::on_deletePoint_clicked()
{
	Global::screens[0]->DeleteSelectedVertex();
}

void MainWindow::on_addSpline_clicked()
{
	Global::screens[0]->AddSpline();
}

void MainWindow::on_addBezier_clicked()
{
	Global::screens[0]->AddBezier();
}

void MainWindow::on_addPoint_clicked()
{
	Global::screens[0]->AddVertex();
}

void MainWindow::on_degreeSpin_valueChanged(int arg1)
{
	Global::screens[0]->SplineDegreeChanged(arg1);
}

void MainWindow::NewSpline(int degree, std::vector<float> knots)
{
	ui->degreeSpin->setValue(degree);

	ui->knotCombo->clear();
	if(!knots.empty())
		for(int i=0; (unsigned) i<knots.size(); i++)
			ui->knotCombo->addItem(QString::number(knots[i]));
}

void MainWindow::on_knotCombo_currentIndexChanged(const QString &arg1)
{
	ui->knotSpin->setValue(arg1.toDouble());
}

void MainWindow::on_knotSpin_valueChanged(double arg1)
{
	int index = ui->knotCombo->currentIndex();
	ui->knotCombo->setItemText(index, QString::number(arg1));
	Global::screens[0]->SetKnot(index, arg1);
}

void MainWindow::on_saveButton_clicked()
{
	Global::Save();
}
