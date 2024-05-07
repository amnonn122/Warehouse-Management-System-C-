#include "../include/Customer.h"
#include <sstream>

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
    : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId() {}

const string &Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}

int Customer::getCustomerDistance() const
{
    return locationDistance;
}

int Customer::getMaxOrders() const
{
    return maxOrders;
}

int Customer::getNumOrders() const
{
    return ordersId.size();
}

bool Customer::canMakeOrder() const
{
    return ordersId.size() < static_cast<size_t>(maxOrders);
}

const vector<int> &Customer::getOrdersIds() const
{
    return ordersId;
}

int Customer::addOrder(int orderId)
{
    if (canMakeOrder())
    {
        ordersId.push_back(orderId);
        return orderId;
    }
    else
    {
        return -1;
    }
}

// SoldierCustomer constructor definition
SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

// SoldierCustomer clone function definition
SoldierCustomer *SoldierCustomer::clone() const
{
    SoldierCustomer *clone = new SoldierCustomer(getId(), getName(), getCustomerDistance(), getMaxOrders());
    for (int orderId : getOrdersIds())
    {
        clone->addOrder(orderId);
    }
    return clone;
}

// CivilianCustomer constructor definition
CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

// CivilianCustomer clone function definition
CivilianCustomer *CivilianCustomer::clone() const
{
    CivilianCustomer *clone = new CivilianCustomer(getId(), getName(), getCustomerDistance(), getMaxOrders());
    for (int orderId : getOrdersIds())
    {
        clone->addOrder(orderId);
    }
    return clone;
}

// toStrings
std::string Customer::toString() const
{
    std::stringstream ss;
    ss << "Customer ID: " << id << ", Name: " << name << ", Location Distance: " << locationDistance << ", Max Orders: " << maxOrders;
    return ss.str();
}

std::string SoldierCustomer::toString() const
{
    return "Soldier Customer: " + Customer::toString();
}

std::string CivilianCustomer::toString() const
{
    return "Civilian Customer: " + Customer::toString();
}