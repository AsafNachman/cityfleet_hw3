#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <string>

template <typename T>
struct Measurement {
    T value;
    int day = 0;
    std::string unit;
};

template <typename T>
Measurement<T> makeMeasurement(T value, int day = 0, std::string unit = "") {
    return Measurement<T>{value, day, unit};
}

#endif // MEASUREMENT_H