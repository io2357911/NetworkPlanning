#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include "graph.h"

class Work {
public:
    Work(int number, int cost, int time)
        : number(number),
          cost(cost),
          time(time)
    {}

    int number;
    int cost;
    int time;
};

class Event {
public:
    Event(int number, int earlyTime = 0, int lateTime = 0, int reserve = 0, bool isCalculated = false)
        : number(number),
          earlyTime(earlyTime),
          lateTime(lateTime),
          reserve(reserve),
          isCalculated(isCalculated)
    {}

    int number;
    int earlyTime;
    int lateTime;
    int reserve;

    bool isCalculated;
};

typedef Graph<Event, Work> NetworkGraph;

#endif // NETWORKGRAPH_H
