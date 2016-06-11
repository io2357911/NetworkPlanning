#ifndef WORKPROPERTIESDIALOG_H
#define WORKPROPERTIESDIALOG_H

#include <QDialog>
#include "../math/networkgraph.h"

namespace Ui {
class WorkPropertiesDialog;
}

class WorkPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkPropertiesDialog(QWidget *parent = 0);
    ~WorkPropertiesDialog();

    void setWork(IWork *value);
    void setWorkers(QVector<IWorker*> *workers);

    void updateWorkers();

private slots:
    void on_buttonBox_accepted();
    void on_cbWorker_currentIndexChanged(int index);

private:
    void updateWork();
    IWorker *currentWorker();

private:
    Ui::WorkPropertiesDialog *ui;

    IWork *work;
    QVector<IWorker*> *workers;
};

#endif // WORKPROPERTIESDIALOG_H
