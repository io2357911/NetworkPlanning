#include "algorithms.h"

void PlanningAlgoritms::NetworkGraphAlgoritm::compute(NetworkGraph *graph) {
    if (graph->isNull()) return;
    if (!graph->firstVertex()) return;

    NetworkGraph::Vertex *vertex;

    // проход в прямом направлении
    reset(graph);
    vertex = nextForward(graph);
    while (vertex != NULL) {
        Event *event = vertex->value();
        event->earlyTime = maxEarlyTime(vertex);
        event->isCalculated = true;

        vertex = nextForward(graph);
    }

    // проход в обратном направлении
    reset(graph);
    vertex = nextBackward(graph);
    while (vertex != NULL) {
        Event *event = vertex->value();
        event->lateTime = minLateTime(vertex);
        event->reserve = event->lateTime - event->earlyTime;
        event->isCalculated = true;

        vertex = nextBackward(graph);
    }
}

bool PlanningAlgoritms::NetworkGraphAlgoritm::reset(NetworkGraph *graph) {
    QVector<NetworkGraph::Vertex*> vertices = graph->vertices();
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i]->value()->isCalculated = false;
    }
}

NetworkGraph::Vertex *PlanningAlgoritms::NetworkGraphAlgoritm::nextForward(NetworkGraph *graph) {
    QVector<NetworkGraph::Vertex*> vertices = graph->vertices();

    for (int i = 0; i < vertices.size(); i++) {
        NetworkGraph::Vertex *vertex = vertices[i];

        if (vertex->value()->isCalculated)
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

        if (vertex->value()->isCalculated)
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
        Work *prevWork = edges[i]->value();
        Event *prevEvent = edges[i]->vertex1()->value();

        int time = prevEvent->earlyTime + prevWork->time;
        if (time > maxTime)
            maxTime = time;
    }
    return maxTime;
}

int PlanningAlgoritms::NetworkGraphAlgoritm::minLateTime(NetworkGraph::Vertex *vertex) {
    if (vertex->isLast())
        return vertex->value()->earlyTime;

    int minTime = pow(2, 8 * sizeof(int)) / 2 - 1;
    QVector<NetworkGraph::Edge*> edges = vertex->nextEdges();
    for (int i = 0; i < edges.size(); i++) {
        Work *nextWork = edges[i]->value();
        Event *nextEvent = edges[i]->vertex2()->value();

        int time = nextEvent->lateTime - nextWork->time;
        if (time < minTime)
            minTime = time;
    }
    return minTime;
}

bool PlanningAlgoritms::NetworkGraphAlgoritm::allAreCalculated(const QVector<NetworkGraph::Vertex *> &vertices) {
    for (int i = 0; i < vertices.size(); i++) {
        if (!vertices[i]->value()->isCalculated)
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

    while (vertex != lastVertex) {
        QVector<NetworkGraph::Edge*> nextEdges = vertex->nextEdges();
        for (int i = 0; i < nextEdges.size(); i++) {

            Event *firstEvent = nextEdges[i]->vertex1()->value();
            Event *secondEvent = nextEdges[i]->vertex2()->value();
            Work *work = nextEdges[i]->value();

            int fullReserve = secondEvent->lateTime - firstEvent->earlyTime - work->time;

            if (firstEvent->reserve == 0 && secondEvent->reserve == 0 && fullReserve == 0) {
                critPath.append(nextEdges[i]);
                vertex = nextEdges[i]->vertex2();
                break;
            }
        }
    }

    return critPath;
}
