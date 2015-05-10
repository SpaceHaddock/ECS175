#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "vector"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	static MainWindow* mywindow;
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent* event);

	void NewSpline(int degree, std::vector<float> knots);

private slots:
	void on_resolutinonSpin_valueChanged(int arg1);

	void on_deleteCurve_clicked();

	void on_deletePoint_clicked();

	void on_addSpline_clicked();

	void on_addBezier_clicked();

	void on_addPoint_clicked();

	void on_degreeSpin_valueChanged(int arg1);

	void on_knotCombo_currentIndexChanged(const QString &arg1);

	void on_knotSpin_valueChanged(double arg1);

	void on_saveButton_clicked();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
