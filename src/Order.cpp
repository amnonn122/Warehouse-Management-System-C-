#include "../include/Order.h"

// Constructor for the Order class
Order::Order(int id, int customerId, int distance)
    : id(id), customerId(customerId), distance(distance),
      status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

// Getter methods
int Order::getId() const
{
    return id;
}

int Order::getCustomerId() const
{
    return customerId;
}

int Order::getCollectorId() const
{
    return collectorId;
}

int Order::getDriverId() const
{
    return driverId;
}

OrderStatus Order::getStatus() const
{
    return status;
}

int Order::getDistance() const
{
    return distance;
}

// Setters
void Order::setStatus(OrderStatus newStatus)
{
    status = newStatus;
}

void Order::setCollectorId(int newCollectorId)
{
    collectorId = newCollectorId;
}

void Order::setDriverId(int newDriverId)
{
    driverId = newDriverId;
}

// String representation of the Order
const string Order::toString() const
{
    string statusString = getStatusString(status);
    return "Order ID: " + std::to_string(id) +
           "\nCustomer ID: " + std::to_string(customerId) +
           "\nDistance: " + std::to_string(distance) +
           "\nStatus: " + statusString +
           "\nCollector ID: " + std::to_string(collectorId) +
           "\nDriver ID: " + std::to_string(driverId);
}

const string Order::getStatusString(enum OrderStatus sta) const
{
    switch (sta)
    {
    case OrderStatus::PENDING:
        return "PENDING";
    case OrderStatus::COLLECTING:
        return "COLLECTING";
    case OrderStatus::DELIVERING:
        return "DELIVERING";
    case OrderStatus::COMPLETED:
        return "COMPLETED";
    }
    return "";
}
