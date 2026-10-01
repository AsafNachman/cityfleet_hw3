#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <stdexcept>

// Level 1: Abstract Base Class
class Vehicle {
private:
    int id_;
    std::string name_;
    std::string depot_;
    bool inService_;

public:
    // Constructor with validation
    Vehicle(int id, const std::string& name, const std::string& depot, bool inService = true);

    // Virtual Destructor
    virtual ~Vehicle() = default;

    // Pure Virtual Functions
    virtual std::string kind() const = 0;
    virtual double dailyCostILS() const = 0;

    // Virtual function with default implementation
    virtual std::string statusLine() const;

    // Default empty text so Fleet can print measurements without knowing the concrete type.
    virtual std::string lastMeasurementText() const { return ""; }

    // Const Accessors
    int getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getDepot() const { return depot_; }
    bool isInService() const { return inService_; }
    void setInService(bool status) { inService_ = status; }
};

#endif // VEHICLE_H