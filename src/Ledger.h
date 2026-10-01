#ifndef LEDGER_H
#define LEDGER_H

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include "Formatter.h"

#if defined(__cpp_concepts) && __cpp_concepts >= 201907L
#include <concepts>
template <typename T>
concept Formattable = IsFormattable<T>::value;
template <Formattable T, int N = 8>
class Ledger {
#else
template <typename T, int N = 8>
class Ledger {
    static_assert(IsFormattable<T>::value,
                  "T must be formattable via fmt(T)");
#endif
    static_assert(N >= 2, "Ledger capacity N must be at least 2.");
    static_assert(N <= 512, "Ledger capacity N must be at most 512.");

private:
    std::array<T, N> data_{};
    std::size_t head_ = 0; // next slot to overwrite
    std::size_t size_ = 0;

public:
    Ledger() = default;

    static constexpr int capacity() { return N; }
    bool empty() const { return size_ == 0; }
    bool full() const { return size_ == static_cast<std::size_t>(N); }
    std::size_t size() const { return size_; }

    void push(const T& item) {
        data_[head_] = item;
        head_ = (head_ + 1) % static_cast<std::size_t>(N);
        if (size_ < static_cast<std::size_t>(N)) ++size_;
    }

    const T& latest() const {
        if (empty()) throw std::out_of_range("Ledger is empty.");
        const auto index = (head_ + static_cast<std::size_t>(N) - 1)
                           % static_cast<std::size_t>(N);
        return data_[index];
    }

    template <typename Fn>
    void forEach(Fn fn) const {
        const std::size_t oldest = (size_ == static_cast<std::size_t>(N)) ? head_ : 0;
        for (std::size_t i = 0; i < size_; ++i)
            fn(data_[(oldest + i) % static_cast<std::size_t>(N)]);
    }

    template <typename Fn>
    std::size_t countIf(Fn fn) const {
        std::size_t count = 0;
        forEach([&](const T& item) { if (fn(item)) ++count; });
        return count;
    }
};

#endif // LEDGER_H
