#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include "unistd.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	for(int i=0; (unsigned) i<Global::polygons.size(); i++)
	{
		ui->selectedpolygon->addItem(QString::fromStdString(Global::polygons[i].name));
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_ApplyTransform_clicked()
{
	Polygon* selected_polygon = &Global::polygons[ui->selectedpolygon->currentIndex()];
	selected_polygon->ScaleCenter(Vector3(
								ui->scalex->text().toFloat(),
								ui->scaley->text().toFloat(),
								ui->scalez->text().toFloat()));

	selected_polygon->Translate(Vector3(
									ui->translatex->text().toFloat(),
									ui->translatey->text().toFloat(),
									ui->translatez->text().toFloat()));

	float rotate_amount = ui->rotationamount->text().toFloat();
	if(ui->xaxisrot->isChecked())
		selected_polygon->RotateXAxis(rotate_amount);
	else if(ui->yaxisrot->isChecked())
		selected_polygon->RotateYAxis(rotate_amount);
	else if(ui->zaxisrot->isChecked())
		selected_polygon->RotateZAxis(rotate_amount);

	Global::Save();

	for(int i=0; (unsigned)i<Global::screens.size(); i++)
		Global::screens[i]->DrawPolygons();
}

void MainWindow::on_applyrotation_clicked()
{
	Polygon* selected_polygon = &Global::polygons[ui->selectedpolygon->currentIndex()];

	Vector3 a = Vector3(ui->arbitraryx1->text().toFloat(),
			ui->arbitraryy1->text().toFloat(),
			ui->arbitraryz1->text().toFloat());

	Vector3 b = Vector3(ui->arbitraryx2->text().toFloat(),
			ui->arbitraryy2->text().toFloat(),
			ui->arbitraryz2->text().toFloat());

	//This is so it will draw the line
	Global::lines.clear();
	Global::lines.push_back(a);
	Global::lines.push_back(b);

	selected_polygon->RotateArbitrary(ui->arbitraryrotation->text().toFloat(), a, b);

	Global::Save();

	for(int i=0; (unsigned)i<Global::screens.size(); i++)
		Global::screens[i]->DrawPolygons();
}
