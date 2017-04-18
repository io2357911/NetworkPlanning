#ifndef RESOURSESWIDGET_H
#define RESOURSESWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "../math/networkgraph.h"

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

    void setResourses(QVector<Resourse*>* resourses);

signals:
    void newResourse();
    void resourseChanged();
    void deleteResourse(Resourse*);

private slots:
    void on_pbAdd_clicked();
    void on_pbDelete_clicked();
    void onItemChanged(QTableWidgetItem *item);

    void onResoursesChanged();

private:
    Ui::ResoursesWidget *ui;

    QVector<Resourse*>* resourses;
};

#endif // RESOURSESWIDGET_H
