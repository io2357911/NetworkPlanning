#include "workpropertiesdialog.h"
#include "ui_workpropertiesdialog.h"

WorkPropertiesDialog::WorkPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkPropertiesDialog),
    work(NULL)
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
    work->setIsVirtual(ui->cbIsVirtual->isChecked());

    if (!work->getWorker()) {
        work->setCost(ui->sbCost->value());
        work->setTime(ui->sbTime->value());
    }
}

IWork *WorkPropertiesDialog::getWork() const
{
    return work;
}

void WorkPropertiesDialog::setWork(IWork *value)
{
    work = value;

    updateWork();
}

void WorkPropertiesDialog::updateWork()
{
    if (!work) {
        close();
        return;
    }

    ui->sbID->setValue(work->getID());
    ui->sbCost->setValue(work->getCost());
    ui->sbTime->setValue(work->getTime());
    ui->sbFullReserve->setValue(work->getFullReserve());
    ui->cbIsVirtual->setChecked(work->isVirtual());

    IWorker *worker = work->getWorker();

    ui->lWorker->setText(worker ? QString::number(worker->getID())
                                : "нет");

    ui->sbCost->setEnabled(!worker);
    ui->sbTime->setEnabled(!worker);
}
