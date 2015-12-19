#include "algorithms.h"

void PlanningAlgoritms::NetworkGraphAlgoritm::compute(NetworkGraph *graph) {
    if (graph->isNull()) return;
    if (!graph->firstVertex()) return;

    NetworkGraph::Vertex *vertex;

    // проход в прямом направлении
    reset(graph);
    vertex = nextForward(graph);
    while (vertex != NULL) {
        IEvent *event = vertex->value();
        event->setEarlyTime(maxEarlyTime(vertex));
        event->setIsCalculated(true);

        vertex = nextForward(graph);
    }

    // проход в обратном направлении
    reset(graph);
    vertex = nextBackward(graph);
    while (vertex != NULL) {
        IEvent *event = vertex->value();
        event->setLateTime(minLateTime(vertex));
        event->setReserve(event->getLateTime() - event->getEarlyTime());
        event->setIsCalculated(true);

        vertex = nextBackward(graph);
    }
}

bool PlanningAlgoritms::NetworkGraphAlgoritm::reset(NetworkGraph *graph) {
    QVector<NetworkGraph::Vertex*> vertices = graph->vertices();
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i]->value()->setIsCalculated(false);
    }
}

NetworkGraph::Vertex *PlanningAlgoritms::NetworkGraphAlgoritm::nextForward(NetworkGraph *graph) {
    QVector<NetworkGraph::Vertex*> vertices = graph->vertices();

    for (int i = 0; i < vertices.size(); i++) {
        NetworkGraph::Vertex *vertex = vertices[i];

        if (vertex->value()->getIsCalculated())
            continue;

        if (vertex->isFirst())
            return vertex;

        if (allAreCalculated(vertex->previousVertices())) {
            return vertex;
        }
    }
    return NULL;
}

NetworkGraph::Vertex *PlanningAlgoritms::NetworkGraphAlgoritm::nextBackward(NetworkGraph *graph) {
    QVector<NetworkGraph::Vertex*> vertices = graph->vertices();

    for (int i = 0; i < vertices.size(); i++) {
        NetworkGraph::Vertex *vertex = vertices[i];

        if (vertex->value()->getIsCalculated())
            continue;

        if (vertex->isLast())
            return vertex;

        if (allAreCalculated(vertex->nextVertices())) {
            return vertex;
        }
    }
    return NULL;
}

int PlanningAlgoritms::NetworkGraphAlgoritm::maxEarlyTime(NetworkGraph::Vertex *vertex) {
    if (vertex->isFirst())
        return 0;

    int maxTime = 0;
    QVector<NetworkGraph::Edge*> edges = vertex->previousEdges();
    for (int i = 0; i < edges.size(); i++) {
        IWork *prevWork = edges[i]->value();
        IEvent *prevEvent = edges[i]->vertex1()->value();

        int time = prevEvent->getEarlyTime() + prevWork->getTime();
        if (time > maxTime)
            maxTime = time;
    }
    return maxTime;
}

int PlanningAlgoritms::NetworkGraphAlgoritm::minLateTime(NetworkGraph::Vertex *vertex) {
    if (vertex->isLast())
        return vertex->value()->getEarlyTime();

    int minTime = pow(2, 8 * sizeof(int)) / 2 - 1;
    QVector<NetworkGraph::Edge*> edges = vertex->nextEdges();
    for (int i = 0; i < edges.size(); i++) {
        IWork *nextWork = edges[i]->value();
        IEvent *nextEvent = edges[i]->vertex2()->value();

        int time = nextEvent->getLateTime() - nextWork->getTime();
        if (time < minTime)
            minTime = time;
    }
    return minTime;
}

bool PlanningAlgoritms::NetworkGraphAlgoritm::allAreCalculated(const QVector<NetworkGraph::Vertex *> &vertices) {
    for (int i = 0; i < vertices.size(); i++) {
        if (!vertices[i]->value()->getIsCalculated())
            return false;
    }
    return true;
}

QVector<NetworkGraph::Edge*> PlanningAlgoritms::CriticalPathAlgorithm::compute(NetworkGraph *graph)
{
    QVector<NetworkGraph::Edge*> critPath;
    if (graph->isNull()) return critPath;

    NetworkGraph::Vertex *vertex = graph->firstVertex();
    if (!vertex) return critPath;

    NetworkGraph::Vertex *lastVertex = graph->lastVertex();
    if (!lastVertex) return critPath;

    QVector<NetworkGraph::Edge*> edges = graph->edges();
    for (int i = 0; i < edges.size(); i++) {
        IEvent *firstEvent = edges[i]->vertex1()->value();
        IEvent *secondEvent = edges[i]->vertex2()->value();
        IWork *work = edges[i]->value();

        int fullReserve = secondEvent->getLateTime() - firstEvent->getEarlyTime() - work->getTime();

        work->setFullReserve(fullReserve);
        work->setIsCritical(firstEvent->getReserve() == 0 &&
                            secondEvent->getReserve() == 0 &&
                            work->getFullReserve() == 0);
    }

    while (vertex != lastVertex) {
        QVector<NetworkGraph::Edge*> nextEdges = vertex->nextEdges();
        for (int i = 0; i < nextEdges.size(); i++) {
            IWork *work = nextEdges[i]->value();

            if (work->isCritical()) {
                critPath.append(nextEdges[i]);
                vertex = nextEdges[i]->vertex2();
                break;
            }
        }
    }

    return critPath;
}
