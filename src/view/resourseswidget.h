#ifndef RESOURSESWIDGET_H
#define RESOURSESWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "../math/networkgraph.h"
#include "../project.h"

using namespace Math::Planning;

namespace Ui {
class ResoursesWidget;
}

class ResoursesWidget :
        public QWidget,
        public ProjectResourseFactory
{
    Q_OBJECT
public:
    explicit ResoursesWidget(QWidget *parent = 0);
    ~ResoursesWidget();

    // ProjectResourseFactory interface
    ProjectResourse *createResourse();

signals:
    void distributeResourses();

public slots:
    void on_pbAdd_clicked();
    void on_pbDelete_clicked();
    void onItemChanged(QTableWidgetItem *item);

    void onResoursesChanged();

private:
    Ui::ResoursesWidget *ui;
};

#endif // RESOURSESWIDGET_H
