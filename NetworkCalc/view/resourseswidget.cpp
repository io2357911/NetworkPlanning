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

ProjectResourse *ResoursesWidget::createResourse() {
    return new ProjectResourse();
}

void ResoursesWidget::on_pbAdd_clicked() {
    Project::instance()->createResourse();
}

void ResoursesWidget::on_pbDelete_clicked() {
    QVector<ProjectResourse*> *resourses = Project::instance()->resourses();
    if (!resourses) return;
    if (resourses->isEmpty()) return;

    QList<QTableWidgetItem*> items = ui->tlwResourses->selectedItems();
    if (items.isEmpty()) return;

    int row = items[0]->row();
    if (row >= resourses->size()) return;

    Project::instance()->deleteResourse(resourses->at(row));
}

void ResoursesWidget::onResoursesChanged() {
    QVector<ProjectResourse*> *resourses = Project::instance()->resourses();
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

    QVector<ProjectResourse*> *resourses = Project::instance()->resourses();
    if (!resourses) return;
    if (resourses->isEmpty()) return;
    if (row >= resourses->size()) return;

    Resourse* res = resourses->at(row);

    switch (col) {
    case 0:
        res->setName(val);
//        Project::instance()->resoursesChanged();
        break;

    case 1:
        res->setQuantity(val.toDouble());
//        Project::instance()->resoursesChanged();
        break;

    case 2:
        res->setCost(val.toDouble());
//        Project::instance()->resoursesChanged();
        break;
    }
}
