#include "PatrolBike.h"

PatrolBike::PatrolBike(int id, const std::string& name, const std::string& depot, 
                       const std::string& district, bool inService)
    : ManualVehicle(id, name, depot, inService), district_(district) {
    if (district.empty()) {
        throw std::invalid_argument("District cannot be empty.");
    }
}

double PatrolBike::dailyCostILS() const {
    return ManualVehicle::dailyCostILS() + 4.0;
}