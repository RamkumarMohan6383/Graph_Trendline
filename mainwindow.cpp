#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qcustomplot.h>
#include <QPainter>
#include <QVector>
#include <QtMath>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->customPlot->xAxis->setLabel("Time(S)");
    ui->customPlot->yAxis->setLabel("Abs");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(1, 5);
    ui->customPlot->yAxis->setRange(-0.2,5);
    ui->customPlot->replot();
    ui->customPlot->update();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // Sample data

    QVector<double> xData{1, 2, 3, 4, 5};
    QVector<double> yData{1, 4, 5, 8, 12};

    ui->customPlot->xAxis->setLabel("Time(S)");
    ui->customPlot->yAxis->setLabel("Abs");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(-0.2, 7);
    ui->customPlot->yAxis->setRange(-0.2,18);
    ui->customPlot->replot();
    ui->customPlot->update();


    // Scatter plot
    QCPGraph *scatterPlot = ui->customPlot->addGraph();
    scatterPlot->setLineStyle(QCPGraph::lsNone) ;
    scatterPlot->setData(xData, yData);

    // Trendline (linear regression)
    QCPGraph *trendLine = ui->customPlot->addGraph();
    double rSquared = 0.0;
    setLinearRegressionTrendline(xData, yData, trendLine, rSquared);

    // Customize appearance
    scatterPlot->setScatterStyle(QCPScatterStyle::ssDisc);
    trendLine->setPen(QPen(Qt::red));

    // Output R-squared value to console
    qDebug() << "R-squared value: " <<QString::number(static_cast<double>(rSquared), 'f', 4); //rSquared;

    ui->label->setText("<html>R<sup>2</sup> = "+QString::number(static_cast<double>(rSquared), 'f', 4)+"</html>");

    // Replot
    ui->customPlot->replot();
}

double MainWindow::setLinearRegressionTrendline(const QVector<double> &xData, const QVector<double> &yData, QCPGraph *trendLine,double &rSquared)
{
    // Calculate linear regression parameters (slope and intercept)
    int n = xData.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (int i = 0; i < n; ++i)
    {
        sumX += xData[i];
        sumY += yData[i];
        sumXY += xData[i] * yData[i];
        sumX2 += xData[i] * xData[i];
    }

    // double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    // Adjusted slope calculation
    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    // Calculate intercept without forcing it to be zero
    double intercept = 0.0;//(sumY - slope * sumX) / n;
    if(intercept==0.0)
        slope = sumXY / sumX2;
    qDebug() << "slope: " << slope << "Intercept : "<<intercept;

    // Generate trendline data
    QVector<double> trendlineX, trendlineY;
    for (double x : xData)
    {
        trendlineX.append(x);
        trendlineY.append(slope * x + intercept);
    }

    // Set trendline data
    trendLine->setData(trendlineX, trendlineY);

    // Calculate R-squared value
    double meanY = sumY / n;
    double ssTotal = 0, ssResidual = 0;

    for (int i = 0; i < n; ++i)
    {
        double yFitted = slope * xData[i] + intercept;
        ssTotal += (yData[i] - meanY) * (yData[i] - meanY);
        ssResidual += (yData[i] - yFitted) * (yData[i] - yFitted);
    }

    rSquared = 1.0 - (ssResidual / ssTotal);

    return rSquared;
}

