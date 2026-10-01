#include "Fleet.h"
#include <iostream>
#include <string>

void Fleet::add(std::unique_ptr<PoweredVehicle> v) {
    if (!v) return;
    powered_.push_back(v.get());
    vehicles_.push_back(std::move(v));
}

void Fleet::add(std::unique_ptr<ManualVehicle> v) {
    if (!v) return;
    manual_.push_back(v.get());
    vehicles_.push_back(std::move(v));
}

void Fleet::runDay(int day) {
    for (auto* pv : powered_) {
        if (pv && pv->isInService()) {
            pv->measure(day);
        }
    }
}

void Fleet::printReport() const {
    for (std::size_t i = 0; i < vehicles_.size(); ++i) {
        // FleetRegistry::at(i) מחזיר const Vehicle& בגלל ElementAccess
        const Vehicle& v = vehicles_.at(i);
        std::cout << v.statusLine() << " | Daily Cost: " << v.dailyCostILS() << " ILS";
        std::string last = v.lastMeasurementText();
        if (!last.empty()) {
            std::cout << " | " << last;
        }
        std::cout << "\n";
    }
}

double Fleet::totalDailyCostILS() const {
    return vehicles_.sumBy([](const Vehicle& v) { return v.dailyCostILS(); });
}

std::size_t Fleet::inServiceCount() const {
    return vehicles_.countIf([](const Vehicle& v) { return v.isInService(); });
}