#include "ManualVehicle.h"

ManualVehicle::ManualVehicle(int id, const std::string& name, const std::string& depot, bool inService)
    : Vehicle(id, name, depot, inService) {}

double ManualVehicle::dailyCostILS() const {
    return 12.0;
}

std::string ManualVehicle::statusLine() const {
    // אותה סיבה כמו ב-PoweredVehicle: בלי Vehicle:: זה קורא לעצמו בלופ
    return Vehicle::statusLine() + " rides=" + std::to_string(ridesPerDay());
}