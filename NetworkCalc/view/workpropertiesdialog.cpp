#include "workpropertiesdialog.h"
#include "ui_workpropertiesdialog.h"

WorkPropertiesDialog::WorkPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkPropertiesDialog),
    work(NULL),
    workers(NULL)
{
    ui->setupUi(this);
}

WorkPropertiesDialog::~WorkPropertiesDialog()
{
    delete ui;
}

void WorkPropertiesDialog::on_buttonBox_accepted()
{
    if (!work) return;

    work->setID(ui->sbID->value());
    work->setCost(ui->sbCost->value());
    work->setTime(ui->sbTime->value());
    work->setIsVirtual(ui->cbIsVirtual->isChecked());
}

void WorkPropertiesDialog::setWork(IWork *value)
{
    if (!value) return;

    work = value;
    updateWork();
}

void WorkPropertiesDialog::setWorkers(QVector<IWorker *> *workers) {
    this->workers = workers;
    updateWorkers();
}

void WorkPropertiesDialog::updateWorkers()
{
    ui->cbWorker->clear();
    ui->cbWorker->addItem("");
    for (int i = 0; i < workers->size(); i++) {
        ui->cbWorker->addItem(QString::number(workers->at(i)->getID()));
    }
}

void WorkPropertiesDialog::on_cbWorker_currentIndexChanged(int)
{
    if (!work) return;

    IWorker *worker = currentWorker();
    work->setWorker(worker);
    updateWork();
}

void WorkPropertiesDialog::updateWork()
{
    ui->sbID->setValue(work->getID());
    ui->sbCost->setValue(work->getCost());
    ui->sbTime->setValue(work->getTime());
    ui->sbFullReserve->setValue(work->getFullReserve());
    ui->cbIsVirtual->setChecked(work->isVirtual());

    ui->sbCost->setEnabled(!work->getWorker());
    ui->sbTime->setEnabled(!work->getWorker());
}

IWorker *WorkPropertiesDialog::currentWorker()
{
    int index = ui->cbWorker->currentIndex()-1;
    return index > -1 && index < workers->size()
            ? workers->at(index)
            : NULL;
}
