#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resizeTable(ui->twCost, ui->sbWorkersCount->value());
    resizeTable(ui->twTime, ui->sbWorkersCount->value());
    resizeTable(ui->twNet, ui->sbStageCount->value(), "Этап", "Этап");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sbWorkersCount_valueChanged(int val)
{
    resizeTable(ui->twCost, val);
    resizeTable(ui->twTime, val);
}

void MainWindow::resizeTable(QTableWidget *table,
                             int rowsColsCount,
                             QString horHeader,
                             QString vertHeader,
                             double defVal)
{
    table->clear();
    table->setRowCount(rowsColsCount);
    table->setColumnCount(rowsColsCount);

    QStringList horHeaders, vertHeaders;
    for (int i = 0; i < rowsColsCount; i++) {

        horHeaders.append(QString("%1 %2").arg(horHeader).arg(i+1));
        vertHeaders.append(QString("%1 %2").arg(vertHeader).arg(i+1));

        for (int j = 0; j < rowsColsCount; j++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item->setText(QString::number(defVal));
            table->setItem(i, j, item);
        }
    }
    table->setHorizontalHeaderLabels(horHeaders);
    table->setVerticalHeaderLabels(vertHeaders);

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void MainWindow::on_sbStageCount_valueChanged(int val)
{
    resizeTable(ui->twNet, val, "Этап", "Этап");
}
