#include "debug.h"

Debug::Debug()
{
}

void Debug::printEdgesInfo(QVector<IWork *> edges) {
    for (int j = 0; j < edges.size(); j++) {
        qDebug("Edge: %d v1: %d v2: %d",
               edges[j]->getID(),
               edges[j]->vertex1()->getID(),
               edges[j]->vertex2()->getID());
    }
}

void Debug::printGraph(NetworkGraph &graph) {
    if (graph.isNull()) {
        qDebug("printGraphInfo(): Graph is null!");
        return;
    }

    QVector<IEvent*> events = graph.vertices();

    for (int i = 0; i < events.size(); i++) {

        qDebug("\nVertex: %d first: %d last: %d",
               events[i]->getID(),
               events[i]->isFirst(),
               events[i]->isLast());

        qDebug("-------NextEdges-------");
        printEdgesInfo(events[i]->nextEdges());

        qDebug("-------PriveousEdges-------");
        printEdgesInfo(events[i]->previousEdges());
    }
}

void Debug::printEvents(NetworkGraph &graph) {
    if (graph.isNull()) {
        qDebug("printEventsInfo(): Graph is null!");
        return;
    }

    QVector<IEvent*> events = graph.vertices();

    for (int i = 0; i < events.size(); i++) {
        IEvent *event = events[i];
        qDebug("Event: %d earlyTime: %d lateTime: %d reserve: %d",
               event->getID(),
               event->getEarlyTime(),
               event->getLateTime(),
               event->getReserve());
    }
}

void Debug::printCriticalPath(const QVector<IWork *> &path) {
    QString sPath = "Critical path: ";
    for (int i = 0; i < path.size(); i++) {
        sPath += QString("%1-").arg(path[i]->getID());
    }
    qDebug(sPath.toStdString().c_str());
}

