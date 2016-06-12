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
    connect(ui->aComputeAssigns, SIGNAL(triggered()),
            ui->widgetNetworkGraph, SLOT(computeAssigns()));
    connect(ui->aComputeAssignsWithTime, SIGNAL(triggered()),
            this, SLOT(onComputeAssignsWithTime()));

    connect(ui->widgetNetworkGraph, SIGNAL(graphComputed(NetworkGraph*)),
            this, SLOT(onGraphComputed(NetworkGraph*)));

    connect(ui->aWorkers, SIGNAL(triggered()),
            ui->widgetNetworkGraph, SIGNAL(showWorkers()));

    connect(ui->aAssigns, SIGNAL(triggered()),
            ui->widgetNetworkGraph, SIGNAL(showAssigns()));

    connect(ui->aGraphOpen, SIGNAL(triggered()),
            this, SLOT(onOpenGraph()));
    connect(ui->aGraphSave, SIGNAL(triggered()),
            this, SLOT(onSaveGraph()));

    ui->widgetNetworkGraph->setAssingsWidget(ui->wAssigns);
    ui->widgetNetworkGraph->setWorkersWidget(ui->wWorkers);
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

void MainWindow::onComputeAssignsWithTime()
{
    ui->widgetNetworkGraph->computeAssigns(ui->sbTimeLimit->value());
}

void MainWindow::onGraphComputed(NetworkGraph *graph)
{
    ui->sbCost->setValue(graph->getCost());
    ui->sbTime->setValue(graph->getTime());
}

MainWindow::~MainWindow()
{
    delete ui;
}
