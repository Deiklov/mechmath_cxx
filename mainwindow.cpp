#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

// Global objects
DrawArea* drawArea = nullptr;
MainWindow* mainWindow = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    xMin(-8.),
    xMax(8.),
    ui(new Ui::MainWindow)
{
    mainWindow = this;
    ui->setupUi(this);
    ui->xMin->setText(QString::number(xMin));
    ui->xMax->setText(QString::number(xMax));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_xMin_editingFinished()
{
    QString strXMin = ui->xMin->text();
    bool ok = false;
    double x_min = strXMin.toDouble(&ok);
    if (!ok)
        return;
    xMin = x_min;
}

void MainWindow::on_xMax_editingFinished()
{
    QString strXMax = ui->xMax->text();
    bool ok = false;
    double x_max = strXMax.toDouble(&ok);
    if (!ok)
        return;
    xMax = x_max;
}

void MainWindow::on_redrawButton_clicked()
{
    drawArea->setCoordinates(xMin, xMax);
    //... drawArea->update();
    drawArea->onDraw();
}

void MainWindow::on_clearButton_clicked()
{
    drawArea->clear();
}
