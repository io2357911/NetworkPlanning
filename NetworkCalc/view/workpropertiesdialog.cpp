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

    work->setResourse(currentResourse());
    work->setResourseCount(ui->dsbResoursCount->value());

    work->setIsVirtual(ui->cbIsVirtual->isChecked());
}

Resourse *WorkPropertiesDialog::currentResourse() {
    QVector<ProjectResourse*>* resourses = Project::instance()->resourses();


    int ind = ui->cbResourse->currentIndex()-1;
    if (ind < 0 || ind >= resourses->size()) return 0;

    return resourses->at(ind);
}

Work *WorkPropertiesDialog::getWork() const {
    return work;
}

void WorkPropertiesDialog::setWork(Work *value) {
    work = value;

    onUpdateWork();
}

void WorkPropertiesDialog::onUpdateWork() {
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

    onUpdateResourses();
}

void WorkPropertiesDialog::onUpdateResourses() {
    QVector<ProjectResourse*>* resourses = Project::instance()->resourses();

    ui->cbResourse->clear();
    ui->cbResourse->addItem("");
    for (int i = 0; i < resourses->size(); i++) {
        ui->cbResourse->addItem(resourses->at(i)->name());
    }

    int ind = resourses->indexOf(dynamic_cast<ProjectResourse*>(work->resourse()));
    if (ind != -1) ui->cbResourse->setCurrentIndex(ind+1);
}
