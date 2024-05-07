#include "../include/Action.h"

// BaseAction implementation
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}

const string BaseAction::getStatusString(enum ActionStatus sta) const
{
    switch (sta)
    {
    case ActionStatus::ERROR:
        return "ERROR";
    case ActionStatus::COMPLETED:
        return "COMPLETED";
    }
    return "";
}

const string BaseAction::getCustomerTypeString(enum CustomerType ct) const
{
    switch (ct)
    {
    case CustomerType::Soldier:
        return "Soldier";
    case CustomerType::Civilian:
        return "Civilian";
    }
    return "";
}

void BaseAction::print() const
{
    std::cout << this->toString() << " " << this->getStatusString(this->getStatus()) << std::endl;
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " << errorMsg << std::endl; // Print error message to the screen
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

// simulateStep

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    wareHouse.simulateStep(numOfSteps);
    complete();
    wareHouse.addAction(this->clone());
}

std::string SimulateStep::toString() const
{
    return "simulateStep " + std::to_string(numOfSteps);
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

// Close

Close::Close() {}

void Close::act(WareHouse &wareHouse)
{
    // Pending Orders
    printOrders(wareHouse.getPendingOrders());

    // Process Orders
    printOrders(wareHouse.getInProcessOrders());

    // Completed Orders
    printOrders(wareHouse.getCompletedOrders());

    wareHouse.close();

    complete();
    wareHouse.addAction(this->clone());
}

void Close::printOrders(const vector<Order *> &orders) const
{
    for (const auto &order : orders)
    {
        std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId() << " , Status: " << order->getStatusString(order->getStatus()) << std::endl;
    }
}

string Close::toString() const
{
    return "close ";
}

Close *Close::clone() const
{
    return new Close(*this);
}

// BackUp
BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    // Delete previous backup if exists
    if (backup != nullptr)
    {
        delete backup;
    }
    // Create a new backup by cloning the current warehouse
    wareHouse.addAction(this->clone());
    backup = new WareHouse(wareHouse);
    complete();
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const
{
    return "backup";
}

// Restore
RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    // Check if backup is available
    if (backup == nullptr)
    {
        error("No backup available");
    }
    else
    {
        // Restore the warehouse from the backup
        wareHouse = *backup;
        complete();
    }
    wareHouse.addAction(this->clone());
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    return "restore";
}

// AddOrder
AddOrder::AddOrder(int id) : customerId(id) {}

void AddOrder::act(WareHouse &wareHouse)
{
    // Check if there is a customer with the given ID
    bool existId = false;
    int isSucceeded = -1;
    for (const auto &customer : wareHouse.getCustomers())
    {
        if (customer->getId() == customerId)
        {
            existId = true;
            isSucceeded = customer->addOrder(wareHouse.getOrderCounter());
            if (isSucceeded > -1)
            {
                Order *orderToAdd = new Order(wareHouse.getOrderCounter(), customerId, customer->getCustomerDistance());
                wareHouse.setOrderCounter();
                wareHouse.addOrder(orderToAdd);
                complete();
            }
            break;
        }
    }
    // If customer ID doesn't exist or adding order failed
    if (existId == false || isSucceeded == -1)
    {
        error("Cannot place this order");
    }
    wareHouse.addAction(this->clone());
}

string AddOrder::toString() const
{
    return "order " + std::to_string(customerId);
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders)
    : customerName(customerName), customerType(static_cast<CustomerType>(customerTypeStringToInt(customerType))), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse)
{
    wareHouse.addCustomer(customerName, getCustomerTypeString(customerType), distance, maxOrders);
    complete();
    wareHouse.addAction(this->clone());
}

int AddCustomer::customerTypeStringToInt(const string &cType)
{
    if (cType == "soldier")
        return 1;
    else
        return 2;
}

string AddCustomer::toString() const
{
    return "customer " + customerName + getCustomerTypeString(customerType) + std::to_string(distance) + std::to_string(maxOrders);
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

// PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    int isSucceeded = wareHouse.printOrderStatus(orderId);
    if (isSucceeded != -1)
        complete();
    else
        error("Order doesnt exist");
    wareHouse.addAction(this->clone());
}

string PrintOrderStatus::toString() const
{
    return "orderStatus " + std::to_string(orderId);
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

// PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    int isSucceeded = wareHouse.printCustomerStatus(customerId);
    if (isSucceeded != -1)
        complete();
    else
        error("Customer doesnt exist");
    wareHouse.addAction(this->clone());
}

string PrintCustomerStatus::toString() const
{
    return "customerStatus " + std::to_string(this->customerId);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

// PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    int isSucceeded = wareHouse.printVolunteerStatus(volunteerId);
    if (isSucceeded != -1)
        complete();
    else
        error("Volunteer doesnt exist");
    wareHouse.addAction(this->clone());
}

string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus " + std::to_string(volunteerId);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

// PrintActionsLog

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    const vector<BaseAction *> &actions = wareHouse.getActions();
    for (BaseAction *act : actions)
    {
        act->print();
    }

    complete();
    wareHouse.addAction(this->clone());
}

string PrintActionsLog::toString() const
{
    return "log";
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
