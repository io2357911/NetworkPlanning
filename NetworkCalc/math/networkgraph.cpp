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
      m_costEstimation(this),
      m_cost(timeRandom == 0 ? new PertBeta(&m_costEstimation) : timeRandom->create(&m_costEstimation)),
      m_timeEstimation(timeMin, timeMax, timeAvg),
      m_timeResEstimation(this),
      m_time(timeRandom == 0 ? new PertBeta(&m_timeResEstimation) : timeRandom->create(&m_timeResEstimation)),
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

Random *Work::cost() {
    return m_cost;
}

void Work::setCost(IRandomFactory *factory) {
    if (!factory) return;
    if (m_cost) delete m_cost;

    m_cost = factory->create(&m_costEstimation);
}

IEstimation *Work::costEstimation() {
    return &m_costEstimation;
}

void Work::setTime(IRandomFactory* factory) {
    if (!factory) return;
    if (m_time) delete m_time;

    m_time = factory->create(&m_timeResEstimation);
}

Math::Random *Work::time() {
    return m_time;
}

IEstimation *Work::timeEstimation() {
    return &m_timeEstimation;
}

IFunction *Work::timeSpeed() {
    return m_timeSpeed;
}

void Work::setTimeSpeed(IFunction *speed) {
    if (m_timeSpeed) delete m_timeSpeed;

    m_timeSpeed = speed;
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


NetworkGraph::NetworkGraph(QString name) : m_name(name), m_time(0), m_cost(0) {}

NetworkGraph::NetworkGraph(QString name, QVector<Event *> vertices, QVector<Work *> edges)
    : Graph(vertices, edges), m_name(name), m_time(0), m_cost(0)
{}

QString NetworkGraph::name() const {
    return m_name;
}

void NetworkGraph::setName(const QString &name) {
    m_name = name;
}

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

ResourseDistribution NetworkGraph::resourseDistribution() {
    QVector<Work*> works = edges();
    ResourseDistribution res(works.size());

    for (int i = 0; i < works.size(); i++) {
        res[i] = works[i]->resourseCount();
    }

    return res;
}

void NetworkGraph::setResourseDistribution(const ResourseDistribution &res) {
    QVector<Work*> works = edges();
    if (res.size() != works.size()) return;

    for (int i = 0; i < works.size(); i++) {
        works[i]->setResourseCount(res[i]);
    }
}




TimeEstimation::TimeEstimation(Work *work) : m_work(work)
{}

double TimeEstimation::min() {
    if (!m_work) return 0;

    double time = m_work->timeEstimation()->min();
    double resCount = m_work->resourseCount();
    IFunction *timeSpeed = m_work->timeSpeed();

    if (resCount <= 0) return 0;
    if (!timeSpeed) return 0;
    // t = W/w(r);
    // w(r) - скорость, в простейшем случае число - resourse
    // W - общая трудоемкость
    // t - время выполнения
    return time / timeSpeed->value(resCount);
}

void TimeEstimation::setMin(double) {}

double TimeEstimation::max() {
    if (!m_work) return 0;

    double time = m_work->timeEstimation()->max();
    double resCount = m_work->resourseCount();
    IFunction *timeSpeed = m_work->timeSpeed();

    if (resCount <= 0) return 0;
    if (!timeSpeed) return 0;
    // t = W/w(r);
    // w(r) - скорость, в простейшем случае число - resourse
    // W - общая трудоемкость
    // t - время выполнения
    return time / timeSpeed->value(resCount);
}

void TimeEstimation::setMax(double) {}

double TimeEstimation::avg() {
    if (!m_work) return 0;

    double time = m_work->timeEstimation()->avg();
    double resCount = m_work->resourseCount();
    IFunction *timeSpeed = m_work->timeSpeed();

    if (resCount <= 0) return 0;
    if (!timeSpeed) return 0;
    // t = W/w(r);
    // w(r) - скорость, в простейшем случае число - resourse
    // W - общая трудоемкость
    // t - время выполнения
    return time / timeSpeed->value(resCount);
}

void TimeEstimation::setAvg(double) {}

CostEstimation::CostEstimation(Work *work)
    : m_work(work)
{}

double CostEstimation::min() {
    if (!m_work) return 0;

    double time = m_work->timeEstimation()->min();
    Resourse *resourse = m_work->resourse();
    double resourseCount = m_work->resourseCount();

    if (!resourse) return 0;

    return time * resourse->cost() * resourseCount;
}

void CostEstimation::setMin(double /*value*/)
{}

double CostEstimation::max() {
    if (!m_work) return 0;

    double time = m_work->timeEstimation()->max();
    Resourse *resourse = m_work->resourse();
    double resourseCount = m_work->resourseCount();

    if (!resourse) return 0;

    return time * resourse->cost() * resourseCount;
}

void CostEstimation::setMax(double /*value*/)
{}

double CostEstimation::avg() {
    if (!m_work) return 0;

    double time = m_work->timeEstimation()->avg();
    Resourse *resourse = m_work->resourse();
    double resourseCount = m_work->resourseCount();

    if (!resourse) return 0;

    return time * resourse->cost() * resourseCount;
}

void CostEstimation::setAvg(double /*value*/)
{}

// namespace Random

} // namespace Planning
} // namespace Math
