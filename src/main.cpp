#include <iostream>
#include <memory>
#include "Fleet.h"
#include "ElectricBus.h"
#include "GarbageTruck.h"
#include "PatrolBike.h"
#include "WaterTanker.h" // עבור הבונוס

int main() {
    try {
        Fleet fleet;

        fleet.add(std::make_unique<ElectricBus>(1, "Bus-1", "DepotA", 100.0, 40));
        fleet.add(std::make_unique<GarbageTruck>(2, "Truck-1", "DepotB", 150.0, 10.0));
        fleet.add(std::make_unique<PatrolBike>(3, "Bike-1", "DepotC", "North"));
        
        // הוספת הבונוס
        fleet.add(std::make_unique<WaterTanker>(4, "Tanker-1", "DepotA", 120.0, 1000.0));

        fleet.runDay(1);

        std::cout << "--- Fleet Report ---\n";
        fleet.printReport();

        std::cout << "\nTotal Daily Cost: " << fleet.totalDailyCostILS() << " ILS\n";
        std::cout << "Vehicles in service: " << fleet.inServiceCount() << "\n";
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}