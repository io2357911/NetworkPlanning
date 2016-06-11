#include "workersdialog.h"
#include "ui_workersdialog.h"

#include <QSpinBox>

WorkersDialog::WorkersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkersDialog),
    workers(0)
{
    ui->setupUi(this);
}

WorkersDialog::~WorkersDialog()
{
    delete ui;
}

void WorkersDialog::on_pbWorkerAdd_clicked()
{
    emit newWorker();
}

void WorkersDialog::on_pbWorkerDelete_clicked()
{
    IWorker *worker = currentWorker();
    if (worker) emit deleteWorker(worker);
}

void WorkersDialog::setWorkers(QVector<IWorker *> *value)
{
    workers = value;
    updateWorkers();
}

void WorkersDialog::updateWorkers()
{
    ui->cbWorker->clear();
    for (int i = 0; i < workers->size(); i++) {
        ui->cbWorker->addItem(QString::number(workers->at(i)->getID()));
    }
}

void WorkersDialog::updateWorks()
{
    IWorker *worker = currentWorker();
    if (!worker) return;

    Capabilities caps = worker->capabilities();

    ui->tlwWorkerWorks->setRowCount(0);
    ui->tlwWorkerWorks->setRowCount(caps.size());
//    ui->tlwWorkerWorks->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    for (int i = 0; i < caps.size(); i++) {
        IWorkerCapability cap = caps[i];

        QTableWidgetItem *workID = new QTableWidgetItem(QString::number(cap.work->getID()));
        workID->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        workID->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tlwWorkerWorks->setItem(i, 0, workID);

        QSpinBox *workCost = new QSpinBox;
        workCost->setProperty("work", i);
        workCost->setMaximum(999999999);
        workCost->setMinimum(0);
        workCost->setValue(cap.cost);
        connect(workCost, SIGNAL(valueChanged(int)), this, SLOT(onWorkCostChanged(int)));
        ui->tlwWorkerWorks->setCellWidget(i, 1, workCost);

        QSpinBox *workTime = new QSpinBox;
        workTime->setProperty("work", i);
        workTime->setMaximum(999999999);
        workTime->setMinimum(0);
        workTime->setValue(cap.time);
        connect(workTime, SIGNAL(valueChanged(int)), this, SLOT(onWorkTimeChanged(int)));
        ui->tlwWorkerWorks->setCellWidget(i, 2, workTime);
    }
}

void WorkersDialog::on_cbWorker_currentIndexChanged(int)
{
    updateWorks();
}

void WorkersDialog::onWorkCostChanged(int value)
{
    QSpinBox *box = (QSpinBox*)QObject::sender();
    IWork *work = getWork(box);
    if (!work) return;

    IWorker *worker = currentWorker();
    if (worker) worker->setCost(work, value);
}

void WorkersDialog::onWorkTimeChanged(int value)
{
    QSpinBox *box = (QSpinBox*)QObject::sender();
    IWork *work = getWork(box);
    if (!work) return;

    IWorker *worker = currentWorker();
    if (worker) worker->setTime(work, value);
}

IWorker *WorkersDialog::currentWorker()
{
    int index = ui->cbWorker->currentIndex();
    return index > -1 && index < workers->size()
            ? workers->at(index)
            : NULL;
}

IWork *WorkersDialog::getWork(QSpinBox *box)
{
    IWorker *worker = currentWorker();
    if (!worker) return NULL;

    Capabilities caps = worker->capabilities();
    int index = box->property("work").toInt();
    return index > -1 && index < caps.size()
            ? caps[index].work
            : NULL;
}
