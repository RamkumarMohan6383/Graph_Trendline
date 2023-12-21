#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <qcustomplot.h>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    double setLinearRegressionTrendline(const QVector<double> &xData, const QVector<double> &yData, QCPGraph *trendLine,double &rSquared);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
