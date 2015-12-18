#ifndef WORKWIDGET_H
#define WORKWIDGET_H

#include <QWidget>
#include <QPainter>
#include "eventwidget.h"

namespace Ui {
class WorkWidget;
}

class WorkWidget : public QWidget
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
};

#endif // WORKWIDGET_H
