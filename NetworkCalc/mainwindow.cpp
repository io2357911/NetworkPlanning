#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    connect(ui->aGraphNew, SIGNAL(triggered()),
            Project::instance(), SLOT(reset()));

    connect(ui->aGraphOpen, SIGNAL(triggered()),
            this, SLOT(onOpenGraph()));
    connect(ui->aGraphSave, SIGNAL(triggered()),
            this, SLOT(onSaveGraph()));

    connect(ui->aComputePert, SIGNAL(triggered()),
            this, SLOT(onComputePert()));
    connect(ui->aComputeMonteCarlo, SIGNAL(triggered()),
            this, SLOT(onComputeMonteCarlo()));

    connect(Project::instance(), SIGNAL(graphChanged()),
            ui->widgetNetworkGraph, SLOT(onGraphChanged()));
    connect(Project::instance(), SIGNAL(graphChanged()),
            ui->qcp_f, SLOT(onGraphChanged()));
    connect(Project::instance(), SIGNAL(graphChanged()),
            ui->qcp_F, SLOT(onGraphChanged()));

    connect(Project::instance(), SIGNAL(resoursesChanged()),
            ui->wResourses, SLOT(onResoursesChanged()));

    ui->qcp_f->setFunctionType(QMyCustomPlot::f);
    ui->qcp_F->setFunctionType(QMyCustomPlot::F);

    Project::instance()->setEventFactory(ui->widgetNetworkGraph);
    Project::instance()->setWorkFactory(ui->widgetNetworkGraph);
    Project::instance()->setResourseFactory(ui->wResourses);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    ui->widgetNetworkGraph->keyPressEvent(event);
}

void MainWindow::onOpenGraph() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select File"),
                                                    "./",
                                                    tr("ini files (*.ini)"));

    if (!filePath.isNull()) Project::instance()->restore(filePath);
}

void MainWindow::onSaveGraph() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Select File"),
                                                    "./",
                                                    tr("ini files (*.ini)"));

    if (!filePath.isNull()) Project::instance()->store(filePath);
}

void MainWindow::onComputePert() {
    NetworkGraph* graph = Project::instance()->graph();

    if (!PertNetworkAlgorithm(graph).compute()) {
        QMessageBox::warning(NULL, "Ошибка рассчета графа", "Невозможно найти решение");
        return;
    }

    ui->dsbCost->setValue(graph->cost()->value());
//    ui->dsbTime->setValue(graph->time()->F(0.7));
    ui->dsbTime->setValue(graph->time()->value());

    Project::instance()->graphChanged();
}

void MainWindow::onComputeMonteCarlo() {
    NetworkGraph* graph = Project::instance()->graph();

    if (!MonteCarloNetworkAlgorithm(graph, 1000).compute()) {
        QMessageBox::warning(NULL, "Ошибка рассчета графа", "Невозможно найти решение");
        return;
    }

    ui->dsbCost->setValue(graph->cost()->value());
//    ui->dsbTime->setValue(graph->time()->F(0.7));
    ui->dsbTime->setValue(graph->time()->value());

    Project::instance()->graphChanged();
}
