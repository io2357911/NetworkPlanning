#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ui->widgetNetworkGraph->keyPressEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}
