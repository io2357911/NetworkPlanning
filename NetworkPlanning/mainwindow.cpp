#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace PlanningAlgoritms;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->aCountAssesments, SIGNAL(triggered(bool)),
            this, SLOT(countAsignments(bool)));
    connect(ui->aCountAssignmentsWithTime, SIGNAL(triggered(bool)),
            this, SLOT(countAsignmentsWithTime(bool)));

    resizeTable(ui->twCost,
                ui->sbWorkersCount->value(),
                ui->sbWorkersCount->value(),
                "Исполнитель",
                "Работа");

    resizeTable(ui->twTime,
                ui->sbWorkersCount->value(),
                ui->sbWorkersCount->value(),
                "Исполнитель",
                "Работа");

    resizeTable(ui->twNet,
                ui->sbStageCount->value(),
                ui->sbStageCount->value(),
                "Этап",
                "Этап");

    resizeTable(ui->twAssesments,
                ui->sbWorkersCount->value(),
                1,
                "Работа",
                "Исполнитель");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sbWorkersCount_valueChanged(int val)
{
    resizeTable(ui->twCost,
                val,
                val,
                "Исполнитель",
                "Работа");

    resizeTable(ui->twTime,
                val,
                val,
                "Исполнитель",
                "Работа");

    resizeTable(ui->twAssesments,
                ui->sbWorkersCount->value(),
                1,
                "Работа",
                "Исполнитель");
}

void MainWindow::on_sbStageCount_valueChanged(int val)
{
    resizeTable(ui->twNet,
                ui->sbStageCount->value(),
                ui->sbStageCount->value(),
                "Этап",
                "Этап");
}

void MainWindow::on_pbCalcExpenses_clicked()
{
    Matrix costs = values(ui->twCost);
    Matrix assignments = values(ui->twAssesments, true);

    ui->lCost->setText(QString::number(projectCost(costs, assignments)));

    Matrix adjacency = values(ui->twNet, true, false);
    Matrix times = values(ui->twTime);

    ProjectGraph graph(adjacency, &assignments, &times);

    ui->lTime->setText(QString::number(DPCPAlgoritm().compute(graph)));
}

void MainWindow::countAsignments(bool)
{
    Matrix costs = values(ui->twCost);

    HungarianAlgorithm alg;
    setValues(ui->twAssesments, alg.compute(costs), false);

    on_pbCalcExpenses_clicked();
}

void MainWindow::countAsignmentsWithTime(bool)
{
    int workersCount = ui->sbWorkersCount->value();
    int maxTime = ui->sbDirTime->value();
    Matrix costs = values(ui->twCost);
    Matrix times = values(ui->twTime);
    Matrix adjacency = values(ui->twNet, true, false);

    NetworkPlanningAlgorithm alg;
    Matrix assigns = alg.compute(workersCount, maxTime, costs, times, adjacency);
    if (!assigns.isNull()) {
        setValues(ui->twAssesments, assigns, false);

        on_pbCalcExpenses_clicked();

    } else {
        QMessageBox::warning(this, "Нахождение назначений",
                             "Задача не имеет решения при заданом директивном времени выполнения проекта");
    }
}

Matrix MainWindow::values(QTableWidget *table, bool zeroFirst, bool greaterZero)
{
    Matrix m(table->rowCount(), table->columnCount());

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.cols(); j++) {
            double val = table->item(i, j)->text().toDouble();
            if (greaterZero && !val) continue;
            m[i][j] = zeroFirst ? val - 1 : val;
        }
    }

    return m;
}

void MainWindow::setValues(QTableWidget *table, Matrix values, bool zeroFirst)
{
    table->setRowCount(values.rows());
    table->setColumnCount(values.cols());

    for (int i = 0; i < values.rows(); i++) {
        for (int j = 0; j < values.cols(); j++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            double val = values[i][j];
            item->setText(QString::number(zeroFirst ? val : val + 1));
            table->setItem(i, j, item);
        }
    }
}

void MainWindow::resizeTable(QTableWidget *table,
                             int rowsCount,
                             int colsCount,
                             QString rowHeader,
                             QString colHeader,
                             double defVal)
{
    table->clear();
    table->setRowCount(rowsCount);
    table->setColumnCount(colsCount);

    table->setHorizontalHeaderLabels(createTableHeader(colHeader, colsCount));
    table->setVerticalHeaderLabels(createTableHeader(rowHeader, rowsCount));

    for (int i = 0; i < rowsCount; i++) {
        for (int j = 0; j < colsCount; j++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item->setText(QString::number(defVal));
            table->setItem(i, j, item);
        }
    }
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
}

QStringList MainWindow::createTableHeader(QString name, int count)
{
    QStringList header;

    if (count == 1) {
        header.append(name);

    } else {
        for (int i = 0; i < count; i++) {
            header.append(QString("%1 %2").arg(name).arg(i+1));
        }
    }

    return header;
}
