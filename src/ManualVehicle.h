#ifndef MANUAL_VEHICLE_H
#define MANUAL_VEHICLE_H

#include "Vehicle.h"

// Level 2: Abstract Base Class
class ManualVehicle : public Vehicle {
public:
    ManualVehicle(int id, const std::string& name, const std::string& depot, bool inService = true);

    double dailyCostILS() const override;
    std::string statusLine() const override;

    virtual int ridesPerDay() const = 0;
};

#endif // MANUAL_VEHICLE_H