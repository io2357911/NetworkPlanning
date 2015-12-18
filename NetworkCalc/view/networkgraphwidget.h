#ifndef NETWORKGRAPHWIDGET_H
#define NETWORKGRAPHWIDGET_H

#include <QWidget>
#include <QVector>
#include "eventwidget.h"
#include "workwidget.h"

namespace Ui {
class NetworkGraphWidget;
}

class NetworkGraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkGraphWidget(QWidget *parent = 0);
    ~NetworkGraphWidget();

    void contextMenuEvent(QContextMenuEvent *event);

    void paintEvent(QPaintEvent *evt);

    void keyPressEvent(QKeyEvent *event);

    void wheelEvent(QWheelEvent *event);


private slots:
    void onNewEvent();

    void onEventClicked(EventWidget *widget);
    void onEventMoved(EventWidget *widget);
    void onEventStartConnect(EventWidget *widget);
    void onEventProperties(EventWidget *widget);
    void onEventDelete(EventWidget *widget);

    void onWorkProperties(WorkWidget *widget);
    void onWorkDelete(WorkWidget *widget);

private:
    Ui::NetworkGraphWidget *ui;

    QVector<EventWidget*> events;
    QVector<WorkWidget*> works;

    QAction *aNewEvent;

    EventWidget *firstConnectEvent;
};

#endif // NETWORKGRAPHWIDGET_H
