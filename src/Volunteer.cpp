#include "../include/Volunteer.h"
#include "../include/Order.h"

// Volunteer class implementation
Volunteer::Volunteer(int id, const string &name) : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

int Volunteer::getId() const
{
    return id;
}

const string &Volunteer::getName() const
{
    return name;
}

int Volunteer::getActiveOrderId() const
{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const
{
    return completedOrderId;
}

bool Volunteer::isBusy() const
{
    return activeOrderId != NO_ORDER;
}

// CollectorVolunteer implementation

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}

CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step()
{
    if (decreaseCoolDown())
    {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const
{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown()
{
    if (timeLeft > 0)
    {
        timeLeft--;
    }
    return timeLeft == 0;
}

bool CollectorVolunteer::hasOrdersLeft() const
{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return (!isBusy() && order.getId() != activeOrderId);
}

void CollectorVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        completedOrderId = activeOrderId;
        activeOrderId = order.getId();
        timeLeft = coolDown;
    }
}

// LimitedCollectorVolunteer class implementation

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders) : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
    return (hasOrdersLeft() && !isBusy() && order.getId() != activeOrderId);
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        CollectorVolunteer::acceptOrder(order);
        ordersLeft--;
    }
}

int LimitedCollectorVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

// DriverVolunteer implementation

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const
{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const
{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const
{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft()
{
    distanceLeft = distanceLeft - distancePerStep;
    return distanceLeft <= 0;
}

bool DriverVolunteer::hasOrdersLeft() const
{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const
{
    return (!isBusy() && order.getDistance() <= maxDistance && activeOrderId != order.getId());
}

void DriverVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        activeOrderId = order.getId();
        distanceLeft = order.getDistance();
    }
}

void DriverVolunteer::step()
{
    if (isBusy())
        if (decreaseDistanceLeft())
        {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
}

// LimitedDriverVolunteer class implementation

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const
{
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
    return (!isBusy() && order.getDistance() <= getMaxDistance() && ordersLeft > 0 && activeOrderId != order.getId());
}

void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        DriverVolunteer::acceptOrder(order);
        ordersLeft--;
    }
}

std::string Volunteer::toString() const
{
    std::stringstream ss;
    ss << "Volunteer ID: " << getId() << ", Name: " << getName();
    return ss.str();
}

std::string CollectorVolunteer::toString() const
{
    std::stringstream ss;
    ss << "Collector Volunteer - " << Volunteer::toString() << ", Cool Down: " << getCoolDown() << ", Time Left: " << getTimeLeft();
    return ss.str();
}

std::string LimitedCollectorVolunteer::toString() const
{
    std::stringstream ss;
    ss << "Limited Collector Volunteer - " << CollectorVolunteer::toString() << ", Max Orders: " << getMaxOrders() << ", Orders Left: " << getNumOrdersLeft();
    return ss.str();
}

std::string DriverVolunteer::toString() const
{
    std::stringstream ss;
    ss << "Driver Volunteer - " << Volunteer::toString() << ", Max Distance: " << getMaxDistance() << ", Distance Per Step: " << getDistancePerStep() << ", Distance Left: " << getDistanceLeft();
    return ss.str();
}

std::string LimitedDriverVolunteer::toString() const
{
    std::stringstream ss;
    ss << "Limited Driver Volunteer - " << DriverVolunteer::toString() << ", Max Orders: " << getMaxOrders() << ", Orders Left: " << getNumOrdersLeft();
    return ss.str();
}