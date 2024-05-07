#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Order.h"
#include "../include/Volunteer.h"
#include "../include/Action.h"

#include <fstream>
#include <iostream>
#include <sstream>

WareHouse::WareHouse(const string &configFilePath) : isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    // Read configuration file & set up
    readConfigAndSetup(configFilePath);
}

void WareHouse::start()
{
    open();
    std::cout << "Warehouse is open!" << std::endl;

    std::string userInput;
    while (isOpen)
    {
        // Wait for user input
        std::cout << "Enter an action: ";
        std::getline(std::cin, userInput);

        if (userInput == "log")
        {
            // Execute PrintActionsLog action
            PrintActionsLog action;
            action.act(*this);
        }
        else if (userInput == "close")
        {
            // Execute Close action
            Close action;
            action.act(*this);
        }
        else if (userInput == "backup")
        {
            // Execute BackupWarehouse action
            BackupWareHouse action;
            action.act(*this);
        }
        else if (userInput == "restore")
        {
            // Execute RestoreWarehouse action
            RestoreWareHouse action;
            action.act(*this);
        }
        else if (userInput.substr(0, 4) == "step")
        {
            // Extract order ID from input
            int numOfSteps;
            if (sscanf(userInput.substr(5).c_str(), "%d", &numOfSteps) != 1)
            {
                std::cerr << "Invalid order ID!" << std::endl;
                return; 
            }

            SimulateStep action(numOfSteps);
            action.act(*this);
        }
        else if (userInput.substr(0, 11) == "orderStatus")
        {
            // Extract order ID from input
            int orderId;
            if (sscanf(userInput.substr(12).c_str(), "%d", &orderId) != 1)
            {
                std::cerr << "Invalid order ID!" << std::endl;
                return;
            }

            // Execute PrintOrderStatus action
            PrintOrderStatus action(orderId);
            action.act(*this);
        }
        else if (userInput.substr(0, 14) == "customerStatus")
        {
            // Extract customer ID from input
            int customerId;
            if (sscanf(userInput.substr(15).c_str(), "%d", &customerId) != 1)
            {
                std::cerr << "Invalid customer ID!" << std::endl;
                return;
            }

            // Execute PrintCustomerStatus action
            PrintCustomerStatus action(customerId);
            action.act(*this);
        }
        else if (userInput.substr(0, 15) == "volunteerStatus")
        {
            // Extract volunteer ID from input
            int volunteerId;
            if (sscanf(userInput.substr(16).c_str(), "%d", &volunteerId) != 1)
            {
                std::cerr << "Invalid volunteer ID!" << std::endl;
                return;
            }

            // Execute PrintVolunteerStatus action
            PrintVolunteerStatus action(volunteerId);
            action.act(*this);
        }
        else if (userInput.substr(0, 5) == "order")
        {
            // Extract the customer ID from the input
            std::string customerIDString = userInput.substr(6);
            int customerID = std::stoi(customerIDString);

            // Execute AddOrder action
            AddOrder action(customerID);
            action.act(*this);
        }
        else if (userInput.substr(0, 8) == "customer")
        {
            // Extract customer details from input
            std::istringstream iss(userInput.substr(9));
            std::string name, typeString;
            int distance, maxOrders;
            iss >> name >> typeString >> distance >> maxOrders;

            // Execute AddCustomer action
            AddCustomer action(name, typeString, distance, maxOrders);
            action.act(*this);
        }
        else
        {
            std::cout << "Invalid action!" << std::endl;
        }
    }
}

int WareHouse::getCustomerCounter() const
{
    return customerCounter;
}

int WareHouse::getOrderCounter() const
{
    return orderCounter;
}

int WareHouse::getVolunteerCounter() const
{
    return volunteerCounter;
}

void WareHouse::setOrderCounter()
{
    orderCounter++;
}

void WareHouse::addOrder(Order *order)
{
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (const auto &customer : customers)
    {
        if (customer->getId() == customerId)
        {
            return *customer;
        }
    }
    throw std::runtime_error("Customer not found with ID: " + std::to_string(customerId));
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (const auto &volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return *volunteer;
        }
    }
    throw std::runtime_error("Volunteer not found with ID: " + std::to_string(volunteerId));
}

Order &WareHouse::getOrder(int orderId) const
{
    for (const auto &order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (const auto &order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (const auto &order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    throw std::runtime_error("Order not found with ID: " + std::to_string(orderId));
}

const vector<BaseAction *> &WareHouse::getActions() const
{
    return actionsLog;
}

const vector<Order *> &WareHouse::getPendingOrders() const
{
    return pendingOrders;
}

const vector<Order *> &WareHouse::getInProcessOrders() const
{
    return inProcessOrders;
}

const vector<Order *> &WareHouse::getCompletedOrders() const
{
    return completedOrders;
}

const vector<Customer *> &WareHouse::getCustomers() const
{
    return customers;
}

WareHouse::~WareHouse()
{
    // Free memory for actionsLog
    for (BaseAction *action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();

    // Free memory for volunteers
    for (Volunteer *volunteer : volunteers)
    {
        delete volunteer;
    }
    volunteers.clear();

    // Free memory for pendingOrders
    for (Order *order : pendingOrders)
    {
        delete order;
    }
    pendingOrders.clear();

    // Free memory for inProcessOrders
    for (Order *order : inProcessOrders)
    {
        delete order;
    }
    inProcessOrders.clear();

    // Free memory for completedOrders
    for (Order *order : completedOrders)
    {
        delete order;
    }
    completedOrders.clear();

    // Free memory for customers
    for (Customer *customer : customers)
    {
        delete customer;
    }
    customers.clear();
}

WareHouse::WareHouse(const WareHouse &other) : isOpen(other.isOpen),
                                               actionsLog(),
                                               volunteers(),
                                               pendingOrders(),
                                               inProcessOrders(),
                                               completedOrders(),
                                               customers(),
                                               customerCounter(other.customerCounter),
                                               volunteerCounter(other.volunteerCounter),
                                               orderCounter(other.orderCounter)
{
    // Deep copy actionsLog
    for (const auto &action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }

    // Deep copy volunteers
    for (const auto &volunteer : other.volunteers)
    {
        if (dynamic_cast<LimitedCollectorVolunteer *>(volunteer))
            volunteers.push_back(new LimitedCollectorVolunteer(std::move(*dynamic_cast<LimitedCollectorVolunteer *>(volunteer))));
        else if (dynamic_cast<CollectorVolunteer *>(volunteer))
            volunteers.push_back(new CollectorVolunteer(std::move(*dynamic_cast<CollectorVolunteer *>(volunteer))));
        if (dynamic_cast<LimitedDriverVolunteer *>(volunteer))
            volunteers.push_back(new LimitedDriverVolunteer(std::move(*dynamic_cast<LimitedDriverVolunteer *>(volunteer))));
        else if (dynamic_cast<DriverVolunteer *>(volunteer))
            volunteers.push_back(new DriverVolunteer(std::move(*dynamic_cast<DriverVolunteer *>(volunteer))));
    }

    // Deep copy pendingOrders
    for (const auto &order : other.pendingOrders)
    {
        pendingOrders.push_back(new Order(*order));
    }

    // Deep copy inProcessOrders
    for (const auto &order : other.inProcessOrders)
    {
        inProcessOrders.push_back(new Order(*order));
    }

    // Deep copy completedOrders
    for (const auto &order : other.completedOrders)
    {
        completedOrders.push_back(new Order(*order));
    }

    // Deep copy customers
    for (auto &customer : other.customers)
    {
        if (dynamic_cast<SoldierCustomer *>(customer))
            customers.push_back(new SoldierCustomer(std::move(*dynamic_cast<SoldierCustomer *>(customer))));
        else if (dynamic_cast<CivilianCustomer *>(customer))
            customers.push_back(new CivilianCustomer(std::move(*dynamic_cast<CivilianCustomer *>(customer))));
    }
}

WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (this != &other)
    { // Check for self-assignment
        // Clear current data to avoid memory leaks
        for (auto &action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();

        for (auto &volunteer : volunteers)
        {
            delete volunteer;
        }
        volunteers.clear();

        for (auto &order : pendingOrders)
        {
            delete order;
        }
        pendingOrders.clear();

        for (auto &order : inProcessOrders)
        {
            delete order;
        }
        inProcessOrders.clear();

        for (auto &order : completedOrders)
        {
            delete order;
        }
        completedOrders.clear();

        for (auto &customer : customers)
        {
            delete customer;
        }
        customers.clear();

        // Deep copy volunteers
        for (const auto &volunteer : other.volunteers)
        {
            if (dynamic_cast<LimitedCollectorVolunteer *>(volunteer))
                volunteers.push_back(new LimitedCollectorVolunteer(std::move(*dynamic_cast<LimitedCollectorVolunteer *>(volunteer))));
            else if (dynamic_cast<CollectorVolunteer *>(volunteer))
                volunteers.push_back(new CollectorVolunteer(std::move(*dynamic_cast<CollectorVolunteer *>(volunteer))));
            if (dynamic_cast<LimitedDriverVolunteer *>(volunteer))
                volunteers.push_back(new LimitedDriverVolunteer(std::move(*dynamic_cast<LimitedDriverVolunteer *>(volunteer))));
            else if (dynamic_cast<DriverVolunteer *>(volunteer))
                volunteers.push_back(new DriverVolunteer(std::move(*dynamic_cast<DriverVolunteer *>(volunteer))));
        }

        // Deep copy actionsLog
        for (const auto &action : other.actionsLog)
        {
            actionsLog.push_back(action->clone());
        }

        // Deep copy pendingOrders
        for (const auto &order : other.pendingOrders)
        {
            pendingOrders.push_back(new Order(*order));
        }

        // Deep copy inProcessOrders
        for (const auto &order : other.inProcessOrders)
        {
            inProcessOrders.push_back(new Order(*order));
        }

        // Deep copy completedOrders
        for (const auto &order : other.completedOrders)
        {
            completedOrders.push_back(new Order(*order));
        }

        // Deep copy customers
        for (auto &customer : other.customers)
        {
            if (dynamic_cast<SoldierCustomer *>(customer))
                customers.push_back(new SoldierCustomer(std::move(*dynamic_cast<SoldierCustomer *>(customer))));
            else if (dynamic_cast<CivilianCustomer *>(customer))
                customers.push_back(new CivilianCustomer(std::move(*dynamic_cast<CivilianCustomer *>(customer))));
        }

        // Copy other counters
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
    }
    return *this;
}

// Move constructor
WareHouse::WareHouse(WareHouse &&other) noexcept
    : isOpen(std::move(other.isOpen)),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      customerCounter(std::move(other.customerCounter)),
      volunteerCounter(std::move(other.volunteerCounter)),
      orderCounter(std::move(other.orderCounter))
{
}

WareHouse &WareHouse::operator=(WareHouse &&other) noexcept
{
    if (this != &other)
    {
        // Move data from 'other' to this object
        isOpen = std::move(other.isOpen);
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
        customerCounter = std::move(other.customerCounter);
        volunteerCounter = std::move(other.volunteerCounter);
        orderCounter = std::move(other.orderCounter);

        // Reset 'other' to a valid state
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
    }
    return *this;
}

void WareHouse::close()
{
    isOpen = false;
}

void WareHouse::open()
{
    isOpen = true;
}

void WareHouse::addCustomer(Customer *customer)
{
    customers.push_back(customer);
}

void WareHouse::addCustomer(const string &customerName, const string &customerType, int distance, int maxOrders)
{
    if (customerType == "Soldier")
    {
        SoldierCustomer *soldierCustomer = new SoldierCustomer(customerCounter, customerName, distance, maxOrders);
        addCustomer(soldierCustomer);
    }
    else
    {
        CivilianCustomer *civilianCustomer = new CivilianCustomer(customerCounter, customerName, distance, maxOrders);
        addCustomer(civilianCustomer);
    }
    customerCounter++;
}

void WareHouse::addVolunteer(Volunteer *volunteer)
{
    volunteers.push_back(volunteer);
}

int WareHouse::printOrderStatus(int orderId)
{
    for (const auto &order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            std::cout << "OrderId: " << orderId << std::endl;
            std::cout << "OrderStatus: " << order->getStatusString(order->getStatus()) << std::endl;
            std::cout << "CustomerID: " << order->getCustomerId() << std::endl;
            if (order->getCollectorId() == NO_VOLUNTEER)
                std::cout << "Collector: None" << std::endl;
            else
                std::cout << "Collector: " << order->getCollectorId() << std::endl;
            if (order->getDriverId() == NO_VOLUNTEER)
                std::cout << "Driver: None" << std::endl;
            else
                std::cout << "Driver: " << order->getDriverId() << std::endl;
            return 1;
        }
    }
    for (const auto &order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            std::cout << "OrderId: " << orderId << std::endl;
            std::cout << "OrderStatus: " << order->getStatusString(order->getStatus()) << std::endl;
            std::cout << "CustomerID: " << order->getCustomerId() << std::endl;
            std::cout << "Collector: " << order->getCollectorId() << std::endl;
            if (order->getDriverId() == NO_VOLUNTEER)
                std::cout << "Driver: None" << std::endl;
            else
                std::cout << "Driver: " << order->getDriverId() << std::endl;
            return 1;
        }
    }
    for (const auto &order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            std::cout << "OrderId: " << orderId << std::endl;
            std::cout << "OrderStatus: " << order->getStatusString(order->getStatus()) << std::endl;
            std::cout << "CustomerID: " << order->getCustomerId() << std::endl;
            std::cout << "Collector: " << order->getCollectorId() << std::endl;
            std::cout << "Driver: " << order->getDriverId() << std::endl;
            return 1;
        }
    }
    // If order is not found in any of the lists
    return -1;
}

int WareHouse::printCustomerStatus(int customerId)
{
    // Search for the customer with the given ID
    Customer *customer = nullptr;
    for (const auto &c : customers)
    {
        if (c->getId() == customerId)
        {
            customer = c;
            break;
        }
    }

    // If customer not found
    if (customer == nullptr)
    {
        return -1;
    }

    // Print customer ID
    std::cout << "CustomerID: " << customerId << std::endl;

    // Print orders and their statuses
    const vector<int> &orders = customer->getOrdersIds();
    for (int orderId : orders)
    {
        Order *order = &getOrder(orderId);
        if (order != nullptr)
        {
            std::cout << "OrderID: " << order->getId() << std::endl;
            std::cout << "OrderStatus: " << order->getStatusString(order->getStatus()) << std::endl;
        }
    }

    // Print number of orders left
    int numOrdersLeft = customer->getMaxOrders() - customer->getNumOrders();
    std::cout << "numOrdersLeft: " << numOrdersLeft << std::endl;

    return 1;
}

int WareHouse::getInstanceOfVolunteer(Volunteer *volunteer) const
{
    // Checking the instance of
    int instanceOfVolunteer = 0; // 1 = CollectorVolunteer, 2 = LimitedCollectorVolunteer, 3 = DriverVolunteer, 4 = LimitedDriverVolunteer
    CollectorVolunteer *collectorVolunteer = dynamic_cast<CollectorVolunteer *>(volunteer);
    LimitedCollectorVolunteer *limitedCollectorVolunteer = dynamic_cast<LimitedCollectorVolunteer *>(volunteer);
    DriverVolunteer *driverVolunteer = dynamic_cast<DriverVolunteer *>(volunteer);
    LimitedDriverVolunteer *limitedDriverVolunteer = dynamic_cast<LimitedDriverVolunteer *>(volunteer);

    if (limitedCollectorVolunteer != nullptr)
        instanceOfVolunteer = 2;
    else if (collectorVolunteer != nullptr)
        instanceOfVolunteer = 1;

    if (limitedDriverVolunteer != nullptr)
        instanceOfVolunteer = 4;
    else if (driverVolunteer != nullptr)
        instanceOfVolunteer = 3;

    return instanceOfVolunteer;
}

int WareHouse::printVolunteerStatus(int volunteerId)
{
    // Search for the volunteer with the given ID
    Volunteer *volunteer = nullptr;

    for (const auto &v : volunteers)
    {
        if (v->getId() == volunteerId)
        {
            volunteer = v;
        }
    }

    // If volunteer not found, return -1
    if (volunteer == nullptr)
    {
        return -1;
    }

    std::cout << "VolunteerID: " << volunteerId << std::endl;
    std::cout << "isBusy: " << (volunteer->isBusy() ? "True" : "False") << std::endl;

    int instanceOfVolunteer = getInstanceOfVolunteer(volunteer);

    // If volunteer is busy, print the order ID he is currently processing
    if (volunteer->isBusy())
    {
        std::cout << "OrderID: " << volunteer->getActiveOrderId() << std::endl;
        // Check if the volunteer is a Collector or a Driver based on instanceOfVolunteer
        if (instanceOfVolunteer == 1 || instanceOfVolunteer == 2)
        {
            // Assuming Collector role
            std::cout << "TimeLeft: " << dynamic_cast<CollectorVolunteer *>(volunteer)->getTimeLeft() << std::endl;
        }
        else
        {
            // Assuming Driver role
            std::cout << "TimeLeft: " << dynamic_cast<DriverVolunteer *>(volunteer)->getDistanceLeft() << std::endl;
        }
    }

    else
    {
        std::cout << "OrderID: None" << std::endl;
        std::cout << "TimeLeft: None" << std::endl;
    }

    // Checking if this is a limited volunteer
    int ordersLeft = 0;
    if (instanceOfVolunteer == 2)
    {
        ordersLeft = dynamic_cast<LimitedCollectorVolunteer *>(volunteer)->getNumOrdersLeft();
        std::cout << "OrdersLeft: " << ordersLeft << std::endl;
    }
    else if (instanceOfVolunteer == 4)
    {
        ordersLeft = dynamic_cast<LimitedDriverVolunteer *>(volunteer)->getNumOrdersLeft();
        std::cout << "OrdersLeft: " << ordersLeft << std::endl;
    }
    else
    {
        std::cout << "OrdersLeft: No limit" << std::endl;
    }

    return 1;
}

// Helper function to assign orders to volunteers based on their status
void WareHouse::assignOrdersToVolunteers()
{
    auto it = pendingOrders.begin();
    while (it != pendingOrders.end())
    {
        Order *order = *it;
        OrderStatus currentOrderStatus = order->getStatus();
        if (currentOrderStatus == OrderStatus::PENDING)
        {
            for (auto &volunteer : volunteers)
            {
                int instanceOfVolunteer = getInstanceOfVolunteer(volunteer);
                // Check if the volunteer is a Collector / limitedCollector
                if (instanceOfVolunteer == 1 || instanceOfVolunteer == 2)
                {
                    if (volunteer->canTakeOrder(*order))
                    {
                        volunteer->acceptOrder(*order);
                        order->setCollectorId(volunteer->getId());
                        order->setStatus(OrderStatus::COLLECTING);

                        inProcessOrders.push_back(std::move(order));
                        it = pendingOrders.erase(it); // Remove the order from pendingOrders
                        break;                        // Exit the loop after assigning the order
                    }
                }
            }
            if (order->getCollectorId() == NO_VOLUNTEER)
                ++it;
        }
        else if (currentOrderStatus == OrderStatus::COLLECTING)
        {
            for (auto &volunteer : volunteers)
            {
                int instanceOfVolunteer = getInstanceOfVolunteer(volunteer);
                // Check if the volunteer is a Driverr / limitedDriver
                if (instanceOfVolunteer == 3 || instanceOfVolunteer == 4)
                {
                    if (volunteer->canTakeOrder(*order))
                    {
                        volunteer->acceptOrder(*order);
                        order->setDriverId(volunteer->getId());
                        order->setStatus(OrderStatus::DELIVERING);

                        inProcessOrders.push_back(std::move(order));
                        it = pendingOrders.erase(it); // Remove the order from pendingOrders
                        break;                        // Exit the loop after assigning the order
                    }
                }
            }
            if (order->getDriverId() == NO_VOLUNTEER)
                ++it;
        }
        else
        {
            ++it; // Move to the next order if it's not pending or collecting
        }
    }
}

// Helper function to perform a step in the simulation
void WareHouse::performSimulationStep()
{
    for (auto &volunteer : volunteers)
    {
        volunteer->step();
    }
}

// Helper function to check if volunteers have finished their orders
void WareHouse::checkVolunteerFinishedOrders()
{
    auto it = inProcessOrders.begin();
    while (it != inProcessOrders.end())
    {
        Order *order = *it;
        int collectorId = order->getCollectorId();
        int driverId = order->getDriverId();
        if (collectorId != NO_VOLUNTEER && volunteers[collectorId]->getCompletedOrderId() == order->getId())
        {
            pendingOrders.push_back(std::move(order));
            it = inProcessOrders.erase(it);
        }
        else if (driverId != NO_VOLUNTEER && volunteers[driverId]->getCompletedOrderId() == order->getId())
        {
            order->setStatus(OrderStatus::COMPLETED);
            completedOrders.push_back(std::move(order));
            it = inProcessOrders.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// Helper function to delete volunteers who have reached maxOrders limit
void WareHouse::deleteMaxOrdersVolunteers()
{
    for (auto it = volunteers.begin(); it != volunteers.end();)
    {
        if ((*it)->hasOrdersLeft())
        {
            ++it;
        }
        else if ((*it)->getActiveOrderId() != NO_ORDER)
        {
            ++it;
        }
        else
        {
            delete *it;
            it = volunteers.erase(it);
        }
    }
}

void WareHouse::simulateStep(int numberOfSteps)
{
    for (int step = 0; step < numberOfSteps; ++step)
    {
        // Assign orders to volunteers based on their status
        assignOrdersToVolunteers();

        // Perform a step in the simulation
        performSimulationStep();

        // Check if volunteers have finished their orders
        checkVolunteerFinishedOrders();

        // Delete volunteers who have reached maxOrders limit
        deleteMaxOrdersVolunteers();
    }
}

void WareHouse::readConfigAndSetup(const string &configFilePath)
{
    std::ifstream inputFile(configFilePath);
    if (!inputFile.is_open())
    {
        throw std::invalid_argument("Could not open configuration file");
    }

    while (true)
    {
        std::string line;
        if (!std::getline(inputFile, line))
        {
            break;
        }

        if (line.empty())
        {
            continue;
        }

        std::vector<std::string> tokens;
        std::stringstream lineStream(line);
        std::string token;

        while (std::getline(lineStream, token, ' '))
        {
            tokens.push_back(token);
        }

        // Create a new customer
        if (tokens[0] == "customer")
        {
            if (tokens[2] == "soldier")
            {
                SoldierCustomer *soldierCustomer = new SoldierCustomer(customerCounter, tokens[1], stoi(tokens[3]), stoi(tokens[4]));
                addCustomer(soldierCustomer);
            }
            else
            {
                CivilianCustomer *civilianCustomer = new CivilianCustomer(customerCounter, tokens[1], stoi(tokens[3]), stoi(tokens[4]));
                addCustomer(civilianCustomer);
            }
            customerCounter++;
        }

        // Create a new volunteer
        if (tokens[0] == "volunteer")
        {
            if (tokens[2] == "collector")
            {
                CollectorVolunteer *collectorVolunteer = new CollectorVolunteer(volunteerCounter, tokens[1], stoi(tokens[3]));
                addVolunteer(collectorVolunteer);
            }
            else if (tokens[2] == "limited_collector")
            {
                LimitedCollectorVolunteer *limitedCollectorVolunteer = new LimitedCollectorVolunteer(volunteerCounter, tokens[1], stoi(tokens[3]), stoi(tokens[4]));
                addVolunteer(limitedCollectorVolunteer);
            }
            else if (tokens[2] == "driver")
            {
                DriverVolunteer *driverVolunteer = new DriverVolunteer(volunteerCounter, tokens[1], stoi(tokens[3]), stoi(tokens[4]));
                addVolunteer(driverVolunteer);
            }
            else
            {
                LimitedDriverVolunteer *limitedDriverVolunteer = new LimitedDriverVolunteer(volunteerCounter, tokens[1], stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5]));
                addVolunteer(limitedDriverVolunteer);
            }
            volunteerCounter++;
        }
    }
    /* Prints all the Volunteers and the Customers fron the config file
    std::cout << "Volunteers:" << std::endl;
    for (const auto &volunteer : volunteers)
    {
        std::cout << (*volunteer).toString() << std::endl;
    }

    std::cout << "Customers:" << std::endl;
    for (const auto &customer : customers)
    {
        std::cout << (*customer).toString() << std::endl;
    } */

    // Close the configuration file
    
    inputFile.close();
}