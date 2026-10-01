#ifndef GARBAGE_TRUCK_H
#define GARBAGE_TRUCK_H

#include "PoweredVehicle.h"
#include "Ledger.h"
#include "Measurement.h"

class GarbageTruck : public PoweredVehicle {
private:
    double tonnesCapacity_;
    Ledger<Measurement<bool>, 4> ledger_; // חלק ד.2

public:
    GarbageTruck(int id, const std::string& name, const std::string& depot, 
                 double batteryCapacityKWh, double tonnesCapacity, bool inService = true);

    std::string kind() const override { return "GarbageTruck"; }
    double dailyCostILS() const override;

    void measure(int day) override;
    std::string lastMeasurementText() const override;

    double getTonnesCapacity() const { return tonnesCapacity_; }
};

#endif // GARBAGE_TRUCK_H