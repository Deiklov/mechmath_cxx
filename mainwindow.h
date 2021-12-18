#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawarea.h"

class MainWindow;

// Global objects
extern DrawArea* drawArea;
extern MainWindow* mainWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    double xMin;
    double xMax;

private slots:
    void on_xMin_editingFinished();

    void on_xMax_editingFinished();

    void on_redrawButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
