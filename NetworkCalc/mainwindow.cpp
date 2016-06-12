#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->aGraphNew, SIGNAL(triggered()),
            ui->widgetNetworkGraph, SLOT(newGraph()));

    connect(ui->aComputeNetworkGraph, SIGNAL(triggered()),
            ui->widgetNetworkGraph, SLOT(computeNetworkGraph()));

    connect(ui->aWorkers, SIGNAL(triggered()),
            ui->widgetNetworkGraph, SIGNAL(showWorkers()));

    connect(ui->aAssigns, SIGNAL(triggered()),
            ui->widgetNetworkGraph, SIGNAL(showAssigns()));

    connect(ui->aGraphOpen, SIGNAL(triggered()),
            this, SLOT(onOpenGraph()));
    connect(ui->aGraphSave, SIGNAL(triggered()),
            this, SLOT(onSaveGraph()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ui->widgetNetworkGraph->keyPressEvent(event);
}

void MainWindow::onOpenGraph()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select File"),
                                                    "./",
                                                    tr("ini files (*.ini)"));

    if (!filePath.isNull()) ui->widgetNetworkGraph->openGraph(filePath);
}

void MainWindow::onSaveGraph()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Select File"),
                                                    "./",
                                                    tr("ini files (*.ini)"));

    if (!filePath.isNull()) ui->widgetNetworkGraph->saveGraph(filePath);
}

MainWindow::~MainWindow()
{
    delete ui;
}
