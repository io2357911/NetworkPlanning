#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "../tools/ini.h"
#include "../math/networkgraph.h"

namespace Ui {
class EventWidget;
}

class EventWidget : public QWidget, public NetworkGraph::Vertex, public IEvent
{
    Q_OBJECT

public:
    explicit EventWidget(QWidget *parent = 0);
    ~EventWidget();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);

    void paintEvent(QPaintEvent *evt);

    void store(QString fileName);
    void restore(QString fileName);

    void contextMenuEvent(QContextMenuEvent *event);

    QPoint center();

signals:
    void clicked(EventWidget *widget);
    void moved(EventWidget *widget);

    void connectMe(EventWidget *widget);
    void properties(EventWidget *widget);
    void deleteMe(EventWidget *widget);

private slots:
    void onConnection() { emit connectMe(this); }
    void onProperties() { emit properties(this); }
    void onDelete() { emit deleteMe(this); }

private:
    Ui::EventWidget *ui;

    //Event m_event;

    QAction *aConnection;
    QAction *aProperties;
    QAction *aDelete;

    QPoint offset;

    bool isCalculated;

    // IEvent interface
public:
    int getID() const;
    void setID(int value);
    int getEarlyTime() const;
    void setEarlyTime(int value);
    int getLateTime() const;
    void setLateTime(int value);
    int getReserve() const;
    void setReserve(int value);
    bool getIsCalculated() const;
    void setIsCalculated(bool value);
};

#endif // EVENTWIDGET_H
