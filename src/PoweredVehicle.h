#ifndef POWERED_VEHICLE_H
#define POWERED_VEHICLE_H

#include "Vehicle.h"

// Level 2: Abstract Base Class
class PoweredVehicle : public Vehicle {
protected:
    double batteryCapacityKWh_;

public:
    PoweredVehicle(int id, const std::string& name, const std::string& depot, 
                   double batteryCapacityKWh, bool inService = true);

    double dailyCostILS() const override;
    std::string statusLine() const override;

    virtual void measure(int day) = 0;
    virtual std::string lastMeasurementText() const = 0;

    double getBatteryCapacityKWh() const { return batteryCapacityKWh_; }
};

#endif // POWERED_VEHICLE_H