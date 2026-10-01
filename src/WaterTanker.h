#ifndef WATER_TANKER_H
#define WATER_TANKER_H

#include "PoweredVehicle.h"
#include "Ledger.h"
#include "Measurement.h"

class WaterTanker : public PoweredVehicle {
private:
    double capacityLiters_;
    Ledger<Measurement<double>, 5> ledger_;

public:
    WaterTanker(int id, const std::string& name, const std::string& depot, 
                double batteryCapacityKWh, double capacityLiters, bool inService = true)
        : PoweredVehicle(id, name, depot, batteryCapacityKWh, inService), capacityLiters_(capacityLiters) {
        if (capacityLiters < 0.0) {
            throw std::invalid_argument("Water capacity cannot be negative.");
        }
    }

    std::string kind() const override { return "WaterTanker"; }
    
    double dailyCostILS() const override {
        return PoweredVehicle::dailyCostILS() + capacityLiters_ * 0.05;
    }

    void measure(int day) override {
        ledger_.push(makeMeasurement(500.0, day, "L"));
    }

    std::string lastMeasurementText() const override {
        if (ledger_.empty()) return "No measurements";
        return fmt(ledger_.latest());
    }
};

#endif // WATER_TANKER_H