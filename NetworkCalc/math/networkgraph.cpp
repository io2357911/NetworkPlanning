#include "networkgraph.h"

namespace Math {
namespace Planning {

Resourse::Resourse(QString name, double quantity, double cost)
    : m_name(name), m_quantity(quantity), m_cost(cost)
{}

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

void Event::setName(QString value) {
    m_name = value;
}

double Event::earlyTime() const {
    return m_earlyTime;
}

void Event::setEarlyTime(double value) {
    m_earlyTime = value;
}

double Event::lateTime() const {
    return m_lateTime;
}

void Event::setLateTime(double value) {
    m_lateTime = value;
}

double Event::reserve() const {
    return m_reserve;
}

void Event::setReserve(double value) {
    m_reserve = value;
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
           IRandomFactory* timeRandom,
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
      m_time(timeRandom == 0 ? new Randoms::PertBeta(this) : timeRandom->create(this)),
      m_timeSpeed(timeSpeed == 0 ? new Math::Functions::Linear : timeSpeed),
      m_fullReserve(fullReserve),
      m_isCritical(isCritical),
      m_isVirtual(isVirtual)
{}

QString Work::name() const {
    return m_name;
}

void Work::setName(QString value) {
    m_name = value;
}

void Work::setResourse(Resourse *value) {
    m_resourse = value;
}

Resourse *Work::resourse() {
    return m_resourse;
}

void Work::setResourseCount(double value) {
    m_resourseCount = value;
}

double Work::resourseCount() {
    return m_resourseCount;
}

double Work::cost() {
    if (!m_time) return 0;
    if (!m_resourse) return 0;

    return m_time->value() * m_resourse->cost() * m_resourseCount;
}

void Work::setTimeMin(double value) {
    m_timeMin = value;
}

double Work::timeMin(bool unitResourse) {
    if (unitResourse) return m_timeMin;

    if (m_resourseCount <= 0) return 0;
    if (!m_timeSpeed) return 0;
    // t = W/w(r);
    // w(r) - скорость, в простейшем случае число - resourse
    // W - общая трудоемкость
    // t - время выполнения
    return m_timeMin / m_timeSpeed->value(m_resourseCount);
}

void Work::setTimeMax(double value) {
    m_timeMax = value;
}

double Work::timeMax(bool unitResourse) {
    if (unitResourse) return m_timeMax;

    if (m_resourseCount <= 0) return 0;
    if (!m_timeSpeed) return 0;
    // t = W/w(r);
    // w(r) - скорость, в простейшем случае число - resourse
    // W - общая трудоемкость
    // t - время выполнения
    return m_timeMax / m_timeSpeed->value(m_resourseCount);
}

void Work::setTimeAvg(double value) {
    m_timeAvg = value;
}

double Work::timeAvg(bool unitResourse) {
    if (unitResourse) return m_timeAvg;

    if (m_resourseCount <= 0) return 0;
    if (!m_timeSpeed) return 0;
    // t = W/w(r);
    // w(r) - скорость, в простейшем случае число - resourse
    // W - общая трудоемкость
    // t - время выполнения
    return m_timeAvg / m_timeSpeed->value(m_resourseCount);
}

void Work::setTime(IRandomFactory* factory) {
    if (!factory) return;
    if (m_time) delete m_time;

    m_time = factory->create(this);
}

Math::Random *Work::time() {
    return m_time;
}

double Work::fullReserve() const {
    return m_fullReserve;
}

void Work::setFullReserve(double fullReserve) {
    m_fullReserve = fullReserve;
}

void Work::setIsCritical(bool value) {
    m_isCritical = value;
}

bool Work::isCritical() const {
    return m_isCritical;
}

void Work::setIsVirtual(bool value) {
    m_isVirtual = value;
}

bool Work::isVirtual() const {
    return m_isVirtual;
}


NetworkGraph::NetworkGraph() : m_time(0), m_cost(0) {}

NetworkGraph::NetworkGraph(QVector<Event *> vertices, QVector<Work *> edges)
    : Graph(vertices, edges), m_time(0), m_cost(0)
{}

void NetworkGraph::setTime(Random *time) {
    if (m_time) delete m_time;

    m_time = time;
}

Math::Random *NetworkGraph::time() {
    return m_time;
}

void NetworkGraph::setCost(Random *cost) {
    if (m_cost) delete m_cost;

    m_cost = cost;
}

Math::Random *NetworkGraph::cost() {
    return m_cost;
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

namespace Randoms {


double Beta::f(double val) {
    if (!m_work) return 0;
    return Math::Randoms::Beta::f(val,
                                  m_work->timeMin(false),
                                  m_work->timeMax(false),
                                  m_work->timeAvg(false));
}

double Beta::F(double val) {
    if (!m_work) return 0;
    return Math::Randoms::Beta::F(val,
                                  m_work->timeMin(false),
                                  m_work->timeMax(false),
                                  m_work->timeAvg(false));
}

double Beta::mathExpected() {
    if (!m_work) return 0;
    return Math::Randoms::Beta::mathExpected(m_work->timeMin(false),
                                             m_work->timeMax(false),
                                             m_work->timeAvg(false));
}

double Beta::dispersion() {
    if (!m_work) return 0;
    return Math::Randoms::Beta::dispersion(m_work->timeMin(false),
                                           m_work->timeMax(false),
                                           m_work->timeAvg(false));
}

double Beta::_random() {
    if (!m_work) return 0;
    return Math::Randoms::Beta::random(m_work->timeMin(false),
                                       m_work->timeMax(false),
                                       m_work->timeAvg(false));
}


double Triangle::f(double val) {
    if (!m_work) return 0;
    return Math::Randoms::Triangle::f(val,
                                     m_work->timeMin(false),
                                     m_work->timeMax(false),
                                     m_work->timeAvg(false));
}

double Triangle::F(double val) {
    if (!m_work) return 0;
    return Math::Randoms::Triangle::F(val,
                                     m_work->timeMin(false),
                                     m_work->timeMax(false),
                                     m_work->timeAvg(false));
}

double Triangle::mathExpected() {
    if (!m_work) return 0;
    return Math::Randoms::Triangle::mathExpected(m_work->timeMin(false),
                                                m_work->timeMax(false),
                                                m_work->timeAvg(false));
}

double Triangle::dispersion() {
    if (!m_work) return 0;
    return Math::Randoms::Triangle::dispersion(m_work->timeMin(false),
                                              m_work->timeMax(false),
                                              m_work->timeAvg(false));
}

double Triangle::_random() {
    if (!m_work) return 0;
    return Math::Randoms::Triangle::random(m_work->timeMin(false),
                                          m_work->timeMax(false),
                                          m_work->timeAvg(false));
}


double PertBeta::f(double val) {
    if (!m_work) return 0;
    return Math::Randoms::PertBeta::f(val,
                                     m_work->timeMin(false),
                                     m_work->timeMax(false),
                                     m_work->timeAvg(false));
}

double PertBeta::F(double val) {
    if (!m_work) return 0;
    return Math::Randoms::PertBeta::F(val,
                                     m_work->timeMin(false),
                                     m_work->timeMax(false),
                                     m_work->timeAvg(false));
}

double PertBeta::mathExpected() {
    if (!m_work) return 0;
    return Math::Randoms::PertBeta::mathExpected(m_work->timeMin(false),
                                                m_work->timeMax(false),
                                                m_work->timeAvg(false));
}

double PertBeta::dispersion() {
    if (!m_work) return 0;
    return Math::Randoms::PertBeta::dispersion(m_work->timeMin(false),
                                              m_work->timeMax(false),
                                              m_work->timeAvg(false));
}

double PertBeta::_random() {
    if (!m_work) return 0;
    return Math::Randoms::PertBeta::random(m_work->timeMin(false),
                                          m_work->timeMax(false),
                                          m_work->timeAvg(false));
}


} // namespace Random

} // namespace Planning
} // namespace Math
