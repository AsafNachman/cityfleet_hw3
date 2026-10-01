#ifndef ELECTRIC_BUS_H
#define ELECTRIC_BUS_H

#include "PoweredVehicle.h"
#include "Ledger.h"
#include "Measurement.h"

class ElectricBus : public PoweredVehicle {
private:
    int seats_;
    Ledger<Measurement<double>, 6> ledger_; // חלק ד.2

public:
    ElectricBus(int id, const std::string& name, const std::string& depot, 
                double batteryCapacityKWh, int seats, bool inService = true);

    std::string kind() const override { return "ElectricBus"; }
    double dailyCostILS() const override;

    void measure(int day) override;
    std::string lastMeasurementText() const override;

    int getSeats() const { return seats_; }
};

#endif // ELECTRIC_BUS_H