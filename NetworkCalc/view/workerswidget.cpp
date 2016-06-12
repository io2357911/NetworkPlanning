#include "workerswidget.h"
#include "ui_workerswidget.h"

WorkersWidget::WorkersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkersWidget),
    workers(0)
{
    ui->setupUi(this);
}

WorkersWidget::~WorkersWidget()
{
    delete ui;
}

void WorkersWidget::on_pbWorkerAdd_clicked()
{
    emit newWorker();
}

void WorkersWidget::on_pbWorkerDelete_clicked()
{
    IWorker *worker = currentWorker();
    if (worker) emit deleteWorker(worker);
}

void WorkersWidget::setWorkers(QVector<IWorker *> *value)
{
    workers = value;
    updateWorkers();
}

void WorkersWidget::updateWorkers()
{
    ui->cbWorker->clear();
    for (int i = 0; i < workers->size(); i++) {
        ui->cbWorker->addItem(QString::number(workers->at(i)->getID()));
    }
}

void WorkersWidget::updateWorks()
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

void WorkersWidget::on_cbWorker_currentIndexChanged(int)
{
    updateWorks();
}

void WorkersWidget::onWorkCostChanged(int value)
{
    QSpinBox *box = (QSpinBox*)QObject::sender();
    IWork *work = getWork(box);
    if (!work) return;

    IWorker *worker = currentWorker();
    if (work->getWorker()) work->setCost(value);
    if (worker) worker->setCost(work, value);

    emit workerChanged();
}

void WorkersWidget::onWorkTimeChanged(int value)
{
    QSpinBox *box = (QSpinBox*)QObject::sender();
    IWork *work = getWork(box);
    if (!work) return;

    IWorker *worker = currentWorker();
    if (work->getWorker()) work->setTime(value);
    if (worker) worker->setTime(work, value);

    emit workerChanged();
}

IWorker *WorkersWidget::currentWorker()
{
    int index = ui->cbWorker->currentIndex();
    return index > -1 && index < workers->size()
            ? workers->at(index)
            : NULL;
}

IWork *WorkersWidget::getWork(QSpinBox *box)
{
    IWorker *worker = currentWorker();
    if (!worker) return NULL;

    Capabilities caps = worker->capabilities();
    int index = box->property("work").toInt();
    return index > -1 && index < caps.size()
            ? caps[index].work
            : NULL;
}

void WorkersWidget::on_pbCapsRandom_clicked()
{
    IWorker *worker = currentWorker();
    if (!worker) return;

    Capabilities caps = worker->capabilities();
    for (int i = 0; i < caps.size(); i++) {
        IWork *work = caps[i].work;
        worker->setCost(work, 1 + rand() % 100);
        worker->setTime(work, 1 + rand() % 100);
    }

    updateWorks();
}
