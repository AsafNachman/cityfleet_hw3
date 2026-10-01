#ifndef PATROL_BIKE_H
#define PATROL_BIKE_H

#include "ManualVehicle.h"

class PatrolBike : public ManualVehicle {
private:
    std::string district_;

public:
    PatrolBike(int id, const std::string& name, const std::string& depot, 
               const std::string& district, bool inService = true);

    std::string kind() const override { return "PatrolBike"; }
    double dailyCostILS() const override;
    int ridesPerDay() const override { return 8; }

    std::string getDistrict() const { return district_; }
};

#endif // PATROL_BIKE_H