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
    work->setCost(ui->sbCost->value());
    work->setTime(ui->sbTime->value());
    work->setIsVirtual(ui->cbIsVirtual->isChecked());
}

void WorkPropertiesDialog::setWork(IWork *value)
{
    if (!value) return;

    work = value;

    ui->sbID->setValue(work->getID());
    ui->sbCost->setValue(work->getCost());
    ui->sbTime->setValue(work->getTime());
    ui->sbFullReserve->setValue(work->getFullReserve());
    ui->cbIsVirtual->setChecked(work->isVirtual());
}

