#include "networkgraph.h"

namespace Math {
namespace Planning {

QString Resourse::name() const {
    return m_name;
}

void Resourse::setName(const QString &name) {
    m_name = name;
}

double Resourse::quantity() const {
    return m_quantity;
}

void Resourse::setQuantity(double quantity) {
    m_quantity = quantity;
}

double Resourse::cost() const {
    return m_cost;
}

void Resourse::setCost(double cost) {
    m_cost = cost;
}

Event::Event(QString name, int earlyTime, int lateTime, int reserve, bool isCalculated)
    : m_name(name),
      m_earlyTime(earlyTime),
      m_lateTime(lateTime),
      m_reserve(reserve),
      m_isCalculated(isCalculated)
{}

QString Event::name() const {
    return m_name;
}

void Event::setName(int name) {
    m_name = name;
}

double Event::earlyTime() const {
    return m_earlyTime;
}

void Event::setEarlyTime(double earlyTime) {
    m_earlyTime = earlyTime;
}

double Event::lateTime() const {
    return m_lateTime;
}

void Event::setLateTime(double lateTime) {
    m_lateTime = lateTime;
}

double Event::reserve() const {
    return m_reserve;
}

void Event::setReserve(double reserve) {
    m_reserve = reserve;
}

bool Event::isCalculated() const {
    return m_isCalculated;
}

void Event::setIsCalculated(bool isCalculated) {
    m_isCalculated = isCalculated;
}



Work::Work(Event *event1,
           Event *event2,
           QString name,
           Resourse *resourse,
           double resourseCount,
           double timeMin,
           double timeMax,
           double timeAvg,
           IRandom *timeRandom,
           IFunction *timeSpeed,
           bool isCritical,
           bool isVirtual,
           double fullReserve)
    : GraphEdge<Event, Work>(event1, event2),
      m_name(name),
      m_resourse(resourse),
      m_resourseCount(resourseCount),
      m_timeMin(timeMin),
      m_timeMax(timeMax),
      m_timeAvg(timeAvg),
      m_time(timeRandom == 0 ? new Randoms::PertBeta(this) : timeRandom),
      m_timeSpeed(timeSpeed == 0 ? new Math::Functions::Linear : timeSpeed),
      m_fullReserve(fullReserve),
      m_isCritical(isCritical),
      m_isVirtual(isVirtual)
{}

QString Work::name() const {
    return m_name;
}

void Work::setName(int name) {
    m_name = name;
}

double Work::cost() {
    if (!m_resourse) return 0;

    return m_time.value() * m_resourse->cost();
}

double Work::timeMin() {
    if (m_resourseCount <= 0) return 0;
    if (!m_timeSpeed) return 0;
    // t = W/w(r);
    // w(r) - скорость, в простейшем случае число - resourse
    // W - общая трудоемкость
    // t - время выполнения
    return m_timeMin / m_timeSpeed->value(m_resourseCount);
}

double Work::timeMax() {
    if (m_resourseCount <= 0) return 0;
    if (!m_timeSpeed) return 0;
    // t = W/w(r);
    // w(r) - скорость, в простейшем случае число - resourse
    // W - общая трудоемкость
    // t - время выполнения
    return m_timeMax / m_timeSpeed->value(m_resourseCount);
}

double Work::timeAvg() {
    if (m_resourseCount <= 0) return 0;
    if (!m_timeSpeed) return 0;
    // t = W/w(r);
    // w(r) - скорость, в простейшем случае число - resourse
    // W - общая трудоемкость
    // t - время выполнения
    return m_timeAvg / m_timeSpeed->value(m_resourseCount);
}

Random *Work::time() {
    return &m_time;
}

double Work::fullReserve() const {
    return m_fullReserve;
}

void Work::setFullReserve(double fullReserve) {
    m_fullReserve = fullReserve;
}

bool Work::isCritical() const {
    return m_isCritical;
}

void Work::setIsCritical(bool value) {
    m_isCritical = value;
}

bool Work::isVirtual() const {
    return m_isVirtual;
}


NetworkGraph::NetworkGraph() {}

NetworkGraph::NetworkGraph(QVector<Event *> vertices, QVector<Work *> edges)
    : Graph(vertices, edges)
{}

double NetworkGraph::getTime() const {
    return time.value();
}

void NetworkGraph::setTime(double /*value*/) {
}

double NetworkGraph::getCost() const {
    return cost;
}

void NetworkGraph::setCost(double value) {
    cost = value;
}

QVector<Work*> NetworkGraph::criticalPath() {
    QVector<Work*> critPath;
    if (isNull()) return critPath;

    Event *event = firstVertex();
    if (!event) return critPath;

    Event *lastEvent = lastVertex();
    if (!lastEvent) return critPath;

    // построим критический путь
    while (event != lastEvent) {
        QVector<Work*> nextEdges = event->nextEdges();
        for (int i = 0; i < nextEdges.size(); i++) {
            Work *work = nextEdges[i];

            if (work->isCritical()) {
                critPath.append(nextEdges[i]);
                event = nextEdges[i]->vertex2();
                break;
            }
        }
    }

    return critPath;
}

} // namespace Planning
} // namespace Math
