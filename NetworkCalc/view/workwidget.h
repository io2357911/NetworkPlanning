#ifndef WORKWIDGET_H
#define WORKWIDGET_H

#include <QWidget>
#include <QPainter>
#include "eventwidget.h"
#include "../math/networkgraph.h"

using namespace Math::Planning;

namespace Ui {
class WorkWidget;
}

class WorkWidget : public QWidget, public Work
{
    Q_OBJECT

public:
    explicit WorkWidget(QWidget *parent = 0);
    ~WorkWidget();

    void draw(QPainter *painter);

    EventWidget *getFirstEvent() const;
    void setFirstEvent(EventWidget *value);

    EventWidget *getSecondEvent() const;
    void setSecondEvent(EventWidget *value);

    void store(QString fileName);
    void restore(QString fileName);

    void contextMenuEvent(QContextMenuEvent *event);

    // Work interface
    void setResourseCount(double value);
    void setTimeMin(double value);
    void setTimeMax(double value);
    void setTimeAvg(double value);
    void setFullReserve(double value);
    void setIsCritical(bool value);
    void setIsVirtual(bool value);

signals:
    void properties(WorkWidget *widget);
    void deleteMe(WorkWidget *widget);

private slots:
    void onProperties() { emit properties(this); }
    void onDelete() { emit deleteMe(this); }
    void onDeleteEvent(EventWidget *) { emit deleteMe(this); }

    void onWorkChanged();

private:
    Ui::WorkWidget *ui;

    EventWidget *firstEvent;
    EventWidget *secondEvent;

    QAction *aProperties;
    QAction *aDelete;
};

#endif // WORKWIDGET_H
