#include <iostream>
#include <string>

// using the abstraction as vechile class has abstarct method
class Vehicle {
// data encapsulation
protected:
    std::string brand;
    std::string model;

//constructor
public:
    Vehicle(const std::string& brand, const std::string& model)
        : brand(brand), model(model) {}

//abstract method to display vehile information
    virtual void displayInfo() const = 0;  
};

//car class inherit from "Vehicle" using inheritance
class Car : public Vehicle {
private:
    int numSeats;

//constructor
public:
    Car(const std::string& brand, const std::string& model, int numSeats)
        : Vehicle(brand, model), numSeats(numSeats) {}

    // polymorphism 
    void displayInfo() const override {
        std::cout << "Car: " << brand << " " << model << " (" << numSeats << " seats)\n";
    }
};

//class inherited from Vehicle class
class SUV : public Vehicle {
private:
    bool fourWheelDrive;

//constructor
public:
    SUV(const std::string& brand, const std::string& model, bool fourWheelDrive)
        : Vehicle(brand, model), fourWheelDrive(fourWheelDrive) {}

    void displayInfo() const override {
        std::cout << "SUV: " << brand << " " << model << " (4WD: " << (fourWheelDrive ? "Yes" : "No") << ")\n";
    }
};

class RentalManager {
//function overloading
public:
    void rent(const Vehicle& vehicle) {
        std::cout << "Renting: ";
        vehicle.displayInfo();
    }

    void rent(const Vehicle& vehicle, int days) {
        std::cout << "Renting for " << days << " days: ";
        vehicle.displayInfo();
    }
};

//implamening friendship
class Employee {
private:
    std::string name;

public:
    Employee(const std::string& name) : name(name) {}

    friend class RentalManager;

    void displayEmpInfo() const {
        std::cout << "Employee: " << name << "\n";
    }
};

class RentalStatistics {
public:
    //static member
    static int rentedCount;

//static menthod
    static void incrementRentedCount() {
        rentedCount++;
    }
};

int RentalStatistics::rentedCount = 0;

class Customer {
private:
    std::string name;

//conctructor
public:
    Customer(const std::string& name) : name(name) {}

    void displayCusInfo() const {
        std::cout << "Customer: " << name << "\n";
    }
};

int main() {
    //adding demo cars
    Car car("Audi", "A6", 5);
    SUV suv("KIA", "Sportage", true);

    RentalManager manager;

    // class objects pointers
    Vehicle* vehiclePtr = &car;
    Vehicle* suvPtr = &suv;

    // functional Pointers lambda function
    auto displayFunc = [](const Vehicle& vehicle) { vehicle.displayInfo(); };

    //doing demo rental to show system function
    manager.rent(*vehiclePtr);
    manager.rent(*suvPtr, 3);

    Employee emp("Ali");
    emp.displayEmpInfo();

    RentalStatistics::incrementRentedCount();
    std::cout << "Total rentals: " << RentalStatistics::rentedCount << "\n";

    Customer customer("Amir");
    customer.displayCusInfo();

    return 0;
}
