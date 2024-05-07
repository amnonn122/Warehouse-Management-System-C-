all: clean compile link

link:
	g++ -o bin/warehouse bin/Order.o bin/main.o bin/WareHouse.o bin/Customer.o bin/Volunteer.o bin/Action.o
compile:	
	g++ -g -Wall -Weffc++ -c -o bin/Order.o src/Order.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -c -o bin/WareHouse.o src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -c -o bin/main.o src/main.cpp
clean:
	rm -f bin/*.o
