#include "Vehicle.h"

Vehicle::Vehicle(int id, const std::string& name, const std::string& depot, bool inService)
    : id_(id), name_(name), depot_(depot), inService_(inService) {
    if (id <= 0) {
        throw std::invalid_argument("Vehicle ID must be positive.");
    }
    if (name.empty()) {
        throw std::invalid_argument("Vehicle name cannot be empty.");
    }
    if (depot.empty()) {
        throw std::invalid_argument("Vehicle depot cannot be empty.");
    }
}

std::string Vehicle::statusLine() const {
    return "[" + depot_ + "] " + name_ + " (" + kind() + ")";
}