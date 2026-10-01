#include "ElectricBus.h"
#include "Formatter.h"

ElectricBus::ElectricBus(int id, const std::string& name, const std::string& depot, 
                         double batteryCapacityKWh, int seats, bool inService)
    : PoweredVehicle(id, name, depot, batteryCapacityKWh, inService), seats_(seats) {
    if (seats < 0) {
        throw std::invalid_argument("Seats count cannot be negative.");
    }
}

double ElectricBus::dailyCostILS() const {
    return PoweredVehicle::dailyCostILS() + seats_ * 1.4;
}

void ElectricBus::measure(int day) {
    ledger_.push(makeMeasurement(87.5, day, "%"));
}

std::string ElectricBus::lastMeasurementText() const {
    if (ledger_.empty()) return "No measurements";
    return fmt(ledger_.latest()); // שימוש ב-fmt (פולימורפיזם סטטי)
}