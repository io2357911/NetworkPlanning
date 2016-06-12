#include "assignswidget.h"
#include "ui_assignswidget.h"

AssignsWidget::AssignsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AssignsWidget),
    works(0),
    workers(0)
{
    ui->setupUi(this);
}

AssignsWidget::~AssignsWidget()
{
    delete ui;
}

void AssignsWidget::setWorks(QVector<WorkWidget *> *value)
{
    works = value;
    updateWorks();
}

void AssignsWidget::setWorkers(QVector<IWorker *> *value)
{
    workers = value;
    updateWorks();
}

void AssignsWidget::updateWorks()
{
    if (!works || !workers) return;

    QStringList listWorkers;
    listWorkers.append("");
    for (int i = 0; i < workers->size(); i++) {
        listWorkers.append(QString::number(workers->at(i)->getID()));
    }

    ui->tlwAssigns->setRowCount(works->size());
    for (int i = 0; i < works->size(); i++) {
        IWork *work = works->at(i);
        QTableWidgetItem *workID = new QTableWidgetItem(QString::number(work->getID()));
        workID->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        workID->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tlwAssigns->setItem(i, 0, workID);

        IWorker *worker = work->getWorker();

        QComboBox *cbWorker = new QComboBox;
        cbWorker->setProperty("work", i);
        cbWorker->addItems(listWorkers);
        if (worker) cbWorker->setCurrentText(QString::number(worker->getID()));
        connect(cbWorker, SIGNAL(currentIndexChanged(int)), this, SLOT(onWorkerChanged(int)));
        ui->tlwAssigns->setCellWidget(i, 1, cbWorker);
    }
}

void AssignsWidget::updateWorkers()
{
    updateWorks();
}

void AssignsWidget::onWorkerChanged(int)
{
    QComboBox *box = (QComboBox*)QObject::sender();
    IWork *work = getWork(box);
    if (!work) return;

    IWorker *worker = getWorker(box);
    work->setWorker(worker);

    emit assignChanged();
}

IWork *AssignsWidget::getWork(QComboBox *box)
{
    int index = box->property("work").toInt();
    return index > -1 && index < works->size()
            ? works->at(index)
            : NULL;
}

IWorker *AssignsWidget::getWorker(QComboBox *box)
{
    int index = box->currentIndex()-1;
    return index > -1 && index < workers->size()
            ? workers->at(index)
            : NULL;
}
