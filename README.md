# Warehouse Management System - C++ OOP
Warehouse management system written in c++ using OPP
This project implements a Food Warehouse Management System in C++. The system simulates the operations of a warehouse where volunteers handle orders made by customers. It is designed as a command-line program and utilizes object-oriented programming principles, including classes, standard data structures, and the "Rule of 5" in C++.

# Features
Simulates the operations of a warehouse, including handling orders, assigning volunteers, processing orders, and managing customer interactions.
Supports different types of customers (Soldier Customer, Civilian Customer) and volunteers (Collector, Limited Collector, Driver, Limited Driver).
Provides actions for placing orders, creating new customers, checking order status, performing simulation steps, and more.
# How to Run
To run the program, follow these steps:

1. Clone the repository to your local machine. 
2. Navigate to the project directory in your terminal.
3. Compile the source files using the provided makefile:

   ```
    make
   ```
6. Run the compiled executable with the path to the configuration file as the first command-line argument:
```
./bin/warehouse <path_to_configuration_file>
```
Replace `<path_to_configuration_file>` with the path to the configuration file containing the initial state of the warehouse.

# Example Configuration File
The configuration file should contain the initial setup of the warehouse, including customers and volunteers. Each line in the file represents either a customer or a volunteer, following the specified format. Here's an example:

```customer Maya soldier 4 2
customer David civilian 3 1
volunteer Noya collector 2
volunteer Ibrahim limited_collector 3 2
volunteer Din limited_driver 13 4 2
volunteer Limor driver 8 3
```
Ensure that the configuration file adheres to the provided guidelines for customer and volunteer descriptions.

# Usage
Once the program is running, it will initialize the warehouse according to the configuration file provided. It will then start the simulation and prompt the user to enter actions to execute. The available actions are described in the assignment guidelines and include placing orders, checking order status, performing simulation steps, printing status of customers and volunteers, and more.

# Authors
Amnon Abaev
