#include "resourseswidget.h"
#include "ui_resourseswidget.h"

ResoursesWidget::ResoursesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResoursesWidget) {

    ui->setupUi(this);

    connect(ui->tlwResourses, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(onItemChanged(QTableWidgetItem*)));
}

ResoursesWidget::~ResoursesWidget() {
    delete ui;
}

void ResoursesWidget::setResourses(QVector<WorkResourse*> *resourses) {
    this->resourses = resourses;
}

void ResoursesWidget::on_pbAdd_clicked() {
    if (!resourses) return;

    emit newResourse();

    //    resourses->append(new Resourse("Ресурс"));
//    onResoursesChanged();
}

void ResoursesWidget::on_pbDelete_clicked() {
    if (!resourses) return;
    if (resourses->isEmpty()) return;

    QList<QTableWidgetItem*> items = ui->tlwResourses->selectedItems();
    if (items.isEmpty()) return;

    int row = items[0]->row();
    if (row >= resourses->size()) return;

    emit deleteResourse(resourses->at(row));

//    resourses->remove(row);
//    onResoursesChanged();
}

void ResoursesWidget::onResoursesChanged() {
    if (!resourses) return;

    ui->tlwResourses->setRowCount(0);
    for (int i = 0; i < resourses->size(); i++) {
        Resourse* res = resourses->at(i);

        ui->tlwResourses->setRowCount(i+1);

        QTableWidgetItem* item;
        item = new QTableWidgetItem(res->name());
        ui->tlwResourses->setItem(i, 0, item);

        item = new QTableWidgetItem(QString::number(res->quantity()));
        ui->tlwResourses->setItem(i, 1, item);

        item = new QTableWidgetItem(QString::number(res->cost()));
        ui->tlwResourses->setItem(i, 2, item);
    }
}

void ResoursesWidget::onItemChanged(QTableWidgetItem *item) {
    int row = item->row();
    int col = item->column();
    QString val = item->text();

    if (!resourses) return;
    if (resourses->isEmpty()) return;
    if (row >= resourses->size()) return;

    Resourse* res = resourses->at(row);

    switch (col) {
    case 0:
        res->setName(val);
        emit resourseChanged();
        break;

    case 1:
        res->setQuantity(val.toDouble());
        emit resourseChanged();
        break;

    case 2:
        res->setCost(val.toDouble());
        emit resourseChanged();
        break;
    }
}
