#ifndef WORKWIDGET_H
#define WORKWIDGET_H

#include <QWidget>
#include <QPainter>
#include "eventwidget.h"
#include "../math/networkgraph.h"

namespace Ui {
class WorkWidget;
}

class WorkWidget : public QWidget, public NetworkGraph::Edge, public IWork
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

signals:
    void properties(WorkWidget *widget);
    void deleteMe(WorkWidget *widget);

private slots:
    void onProperties() { emit properties(this); }
    void onDelete() { emit deleteMe(this); }
    void onDeleteEvent(EventWidget *) { emit deleteMe(this); }

private:
    Ui::WorkWidget *ui;

    EventWidget *firstEvent;
    EventWidget *secondEvent;

    QAction *aProperties;
    QAction *aDelete;

    int id;
    int cost;
    int fullReserve;
    bool m_isCritical;
    bool m_isVirtual;
    //int time;

    // IWork interface
public:
    int getID() const;
    void setID(int value);

    int getCost() const;
    void setCost(int value);

    int getTime() const;
    void setTime(int value);

    int getFullReserve() const { return fullReserve; }
    void setFullReserve(int value) { fullReserve = value; }

    bool isCritical() const;
    void setIsCritical(bool value);

    bool isVirtual() const;
    void setIsVirtual(bool value);
};

#endif // WORKWIDGET_H
