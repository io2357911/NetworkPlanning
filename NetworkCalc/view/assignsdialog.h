#ifndef ASSIGNSDIALOG_H
#define ASSIGNSDIALOG_H

#include <QDialog>

#include "../math/networkgraph.h"
#include "workwidget.h"

namespace Ui {
class AssignsDialog;
}

class AssignsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AssignsDialog(QWidget *parent = 0);
    ~AssignsDialog();

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
    Ui::AssignsDialog *ui;

    QVector<WorkWidget*> *works;
    QVector<IWorker*> *workers;
};

#endif // ASSIGNSDIALOG_H
