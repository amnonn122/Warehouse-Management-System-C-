#pragma once
#include <string>
#include <vector>
#include <algorithm>

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.

class WareHouse
{

public:
    WareHouse(const string &configFilePath);
    void start();
    void addOrder(Order *order);
    void addAction(BaseAction *action);
    Customer &getCustomer(int customerId) const;
    Volunteer &getVolunteer(int volunteerId) const;
    Order &getOrder(int orderId) const;
    const vector<BaseAction *> &getActions() const;
    void close();
    void open();
    
    //rule of five
    ~WareHouse();
    WareHouse(const WareHouse& other);
    WareHouse& operator=(const WareHouse& other);
    WareHouse(WareHouse&& other) noexcept;
    WareHouse& operator=(WareHouse&& other) noexcept;

    int getCustomerCounter() const;
    int getVolunteerCounter() const;
    int getOrderCounter() const;
    void setOrderCounter(); // Add 1 to orderCounter
    void readConfigAndSetup(const string &configFilePath);
    void addCustomer(Customer *customer);
    void addCustomer(const string &customerName, const string &customerType, int distance, int maxOrders);
    void addVolunteer(Volunteer *volunteer);
    const vector<Order *> &getPendingOrders() const;
    const vector<Order *> &getInProcessOrders() const;
    const vector<Order *> &getCompletedOrders() const;
    const vector<Customer *> &getCustomers() const;

    int getInstanceOfVolunteer(Volunteer *volunteer) const;
    int printOrderStatus(int orderId);
    int printCustomerStatus(int customerId);
    int printVolunteerStatus(int volunteerId);

    void simulateStep(int numberOfSteps);
    void assignOrdersToVolunteers();
    void performSimulationStep();
    void checkVolunteerFinishedOrders();
    void deleteMaxOrdersVolunteers();

private:
    bool isOpen;
    vector<BaseAction *> actionsLog;
    vector<Volunteer *> volunteers;
    vector<Order *> pendingOrders;
    vector<Order *> inProcessOrders;
    vector<Order *> completedOrders;
    vector<Customer *> customers;
    int customerCounter;  // For assigning unique customer IDs
    int volunteerCounter; // For assigning unique volunteer IDs

    int orderCounter; // For assigning unique order IDs
};
