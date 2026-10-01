#include "GarbageTruck.h"
#include "Formatter.h"

GarbageTruck::GarbageTruck(int id, const std::string& name, const std::string& depot, 
                           double batteryCapacityKWh, double tonnesCapacity, bool inService)
    : PoweredVehicle(id, name, depot, batteryCapacityKWh, inService), tonnesCapacity_(tonnesCapacity) {
    if (tonnesCapacity < 0.0) {
        throw std::invalid_argument("Capacity cannot be negative.");
    }
}

double GarbageTruck::dailyCostILS() const {
    return PoweredVehicle::dailyCostILS() + tonnesCapacity_ * 30.0;
}

void GarbageTruck::measure(int day) {
    bool full = true;
    ledger_.push(makeMeasurement(full, day, ""));
}

std::string GarbageTruck::lastMeasurementText() const {
    if (ledger_.empty()) return "No measurements";
    return fmt(ledger_.latest());
}