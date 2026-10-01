#include "PoweredVehicle.h"

PoweredVehicle::PoweredVehicle(int id, const std::string& name, const std::string& depot, 
                               double batteryCapacityKWh, bool inService)
    : Vehicle(id, name, depot, inService), batteryCapacityKWh_(batteryCapacityKWh) {
    if (batteryCapacityKWh < 0.0) {
        throw std::invalid_argument("Battery capacity cannot be negative.");
    }
}

double PoweredVehicle::dailyCostILS() const {
    return batteryCapacityKWh_ * 0.55;
}

std::string PoweredVehicle::statusLine() const {
    // א.2: חייבים לקרוא Vehicle::statusLine() עם שם המחלקה.
    // בלי זה הקריאה הולכת ל-vtable לפי this, כלומר שוב לפונקציה הזאת,
    // וזה רקורסיה בלי סוף עד שהמחסנית מתפוצצת.
    return Vehicle::statusLine() + " powered";
}