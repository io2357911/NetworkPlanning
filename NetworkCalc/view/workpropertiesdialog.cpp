#include "workpropertiesdialog.h"
#include "ui_workpropertiesdialog.h"

WorkPropertiesDialog::WorkPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkPropertiesDialog),
    work(NULL) {

    ui->setupUi(this);
}

WorkPropertiesDialog::~WorkPropertiesDialog() {
    delete ui;
}

void WorkPropertiesDialog::on_buttonBox_accepted() {
    if (!work) return;

    work->setName(ui->leName->text());

    work->setTimeMin(ui->dsbTimeMin->value());
    work->setTimeAvg(ui->dsbTimeAvg->value());
    work->setTimeMax(ui->dsbTimeMax->value());

    work->setResourseCount(ui->dsbResoursCount->value());

    work->setIsVirtual(ui->cbIsVirtual->isChecked());
}

Work *WorkPropertiesDialog::getWork() const {
    return work;
}

void WorkPropertiesDialog::setWork(Work *value) {
    work = value;

    updateWork();
}

void WorkPropertiesDialog::updateWork() {
    if (!work) {
        close();
        return;
    }

    ui->leName->setText(work->name());

    ui->dsbResoursCount->setValue(work->resourseCount());
    ui->dsbCost->setValue(work->cost());

    ui->dsbTimeMin->setValue(work->timeMin());
    ui->dsbTimeAvg->setValue(work->timeAvg());
    ui->dsbTimeMax->setValue(work->timeMax());
    ui->dsbFullReserve->setValue(work->fullReserve());
    ui->dsbTime->setValue(work->time()->value());

    ui->cbIsVirtual->setChecked(work->isVirtual());
}
