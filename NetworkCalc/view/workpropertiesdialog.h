#ifndef WORKPROPERTIESDIALOG_H
#define WORKPROPERTIESDIALOG_H

#include <QDialog>
#include "../math/networkgraph.h"

using namespace Math::Planning;

namespace Ui {
class WorkPropertiesDialog;
}

class WorkPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkPropertiesDialog(QWidget *parent = 0);
    ~WorkPropertiesDialog();

    void setWork(Work *value);
    Work* getWork() const;

    void updateWorkers();

public slots:
    void updateWork();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::WorkPropertiesDialog *ui;

    Work *work;
};

#endif // WORKPROPERTIESDIALOG_H
