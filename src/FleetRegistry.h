#ifndef FLEET_REGISTRY_H
#define FLEET_REGISTRY_H

#include <vector>
#include <memory>
#include <cstddef>

// Trait ראשי
template <typename T>
struct ElementAccess {
    using return_type = const T&;
    static return_type get(const T& elem) {
        return elem;
    }
};

// התמחות חלקית עבור std::unique_ptr<U>
template <typename U>
struct ElementAccess<std::unique_ptr<U>> {
    using return_type = const U&;
    static return_type get(const std::unique_ptr<U>& elem) {
        return *elem;
    }
};

template <typename T, typename Container = std::vector<T>>
class FleetRegistry {
private:
    Container container_;

public:
    void push_back(T item) {
        container_.push_back(std::move(item));
    }

    std::size_t size() const {
        return container_.size();
    }

    auto at(std::size_t i) const -> typename ElementAccess<T>::return_type {
        return ElementAccess<T>::get(container_.at(i));
    }

    template <typename Fn>
    double sumBy(Fn fn) const {
        double sum = 0.0;
        for (std::size_t i = 0; i < container_.size(); ++i) {
            sum += fn(at(i));
        }
        return sum;
    }

    template <typename Fn>
    std::size_t countIf(Fn fn) const {
        std::size_t cnt = 0;
        for (std::size_t i = 0; i < container_.size(); ++i) {
            if (fn(at(i))) cnt++;
        }
        return cnt;
    }
};

#endif // FLEET_REGISTRY_H