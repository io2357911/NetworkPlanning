#ifndef ASSIGNSWIDGET_H
#define ASSIGNSWIDGET_H

#include <QWidget>

#include "../math/networkgraph.h"
#include "workwidget.h"


namespace Ui {
class AssignsWidget;
}

class AssignsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssignsWidget(QWidget *parent = 0);
    ~AssignsWidget();

    void setWorks(QVector<WorkWidget *> *value);
    void setWorkers(QVector<IWorker *> *value);

signals:
    void assignChanged();

public slots:
    void updateWorks();
    void updateWorkers();

private slots:
    void onWorkerChanged(int);

private:
    IWork *getWork(QComboBox *box);
    IWorker *getWorker(QComboBox *box);

private:
    Ui::AssignsWidget *ui;

    QVector<WorkWidget*> *works;
    QVector<IWorker*> *workers;
};

#endif // ASSIGNSWIDGET_H
