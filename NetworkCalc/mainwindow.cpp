#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#define ALG_PERT        QString("PERT")
#define ALG_MONTE_CARLO QString("Монте-Карло")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    Project *proj = Project::instance();

    connect(ui->aGraphNew, SIGNAL(triggered()),
            proj, SLOT(reset()));

    connect(ui->aGraphOpen, SIGNAL(triggered()),
            this, SLOT(onOpenGraph()));
    connect(ui->aGraphSave, SIGNAL(triggered()),
            this, SLOT(onSaveGraph()));

    connect(proj, SIGNAL(graphChanged()),
            ui->widgetNetworkGraph, SLOT(onGraphChanged()));
    connect(proj, SIGNAL(graphChanged()),
            ui->qcp_f, SLOT(onGraphChanged()));
    connect(proj, SIGNAL(graphChanged()),
            ui->qcp_F, SLOT(onGraphChanged()));

    connect(proj, SIGNAL(resoursesChanged()),
            ui->wResourses, SLOT(onResoursesChanged()));

    ui->qcp_f->setFunctionType(QMyCustomPlot::f);
    ui->qcp_F->setFunctionType(QMyCustomPlot::F);

    proj->setEventFactory(ui->widgetNetworkGraph);
    proj->setWorkFactory(ui->widgetNetworkGraph);
    proj->setResourseFactory(ui->wResourses);

    on_cbAlgorithm_currentTextChanged(ALG_PERT);
}

MainWindow::~MainWindow() {
    delete ui;
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

void MainWindow::on_pbCompute_clicked() {
    NetworkGraph* graph = Project::instance()->graph();

    IAlgorithm *alg = currentAlorithm(graph);
    if (!alg) {
        QMessageBox::warning(NULL, "Ошибка рассчета графа", "Неизвестный алгоритм");
        return;
    }

    if (alg->compute()) {
        double prob = ui->dsbProb->value();
        ui->dsbCost->setValue(graph->cost()->value());
        ui->dsbTime->setValue(graph->time()->invF(prob));

        Project::instance()->graphChanged();

    } else {
        QMessageBox::warning(NULL, "Ошибка рассчета графа", "Невозможно найти решение");
    }

    delete alg;
}

IAlgorithm *MainWindow::currentAlorithm(NetworkGraph *graph) {
    IAlgorithm *res = 0;

    QString alg = ui->cbAlgorithm->currentText();
    if (alg == ALG_PERT) {
        res = new PertNetworkAlgorithm(graph);

    } else if (alg == ALG_MONTE_CARLO) {
        uint iterations = ui->sbIterations->value();
        uint intervals = ui->sbIntervals->value();
        res = new MonteCarloNetworkAlgorithm(graph, iterations, intervals);
    }

    return res;
}

void MainWindow::on_cbAlgorithm_currentTextChanged(const QString &alg) {
    bool visible = alg == ALG_MONTE_CARLO;

    ui->lIterations->setVisible(visible);
    ui->lIntervals->setVisible(visible);
    ui->sbIterations->setVisible(visible);
    ui->sbIntervals->setVisible(visible);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    ui->widgetNetworkGraph->keyPressEvent(event);
}
