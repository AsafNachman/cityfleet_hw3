#ifndef FLEET_H
#define FLEET_H

#include <vector>
#include <memory>
#include <cstddef>
#include "Vehicle.h"
#include "PoweredVehicle.h"
#include "ManualVehicle.h"
#include "FleetRegistry.h" // שולב עבור חלק ג.4

class Fleet {
public:
    Fleet() = default;
    ~Fleet() = default;

    // מניעת העתקה למניעת דליפות וכפילויות
    Fleet(const Fleet&) = delete;
    Fleet& operator=(const Fleet&) = delete;
    Fleet(Fleet&&) = default;
    Fleet& operator=(Fleet&&) = default;

    void add(std::unique_ptr<PoweredVehicle> v);
    void add(std::unique_ptr<ManualVehicle> v);

    void runDay(int day);
    void printReport() const;
    double totalDailyCostILS() const;
    std::size_t inServiceCount() const;

private:
    // שימוש ב-FleetRegistry בהתאם לדרישות חלק ג.4
    FleetRegistry<std::unique_ptr<Vehicle>> vehicles_;
    std::vector<PoweredVehicle*> powered_;
    std::vector<ManualVehicle*> manual_;
};

#endif // FLEET_H