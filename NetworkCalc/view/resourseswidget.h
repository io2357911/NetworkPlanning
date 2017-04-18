#ifndef RESOURSESWIDGET_H
#define RESOURSESWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "../math/networkgraph.h"
#include "workresourse.h"

using namespace Math::Planning;

namespace Ui {
class ResoursesWidget;
}

class ResoursesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResoursesWidget(QWidget *parent = 0);
    ~ResoursesWidget();

    void setResourses(QVector<WorkResourse*>* resourses);

signals:
    void newResourse();
    void resourseChanged();
    void deleteResourse(WorkResourse*);

private slots:
    void on_pbAdd_clicked();
    void on_pbDelete_clicked();
    void onItemChanged(QTableWidgetItem *item);

    void onResoursesChanged();

private:
    Ui::ResoursesWidget *ui;

    QVector<WorkResourse*>* resourses;
};

#endif // RESOURSESWIDGET_H
