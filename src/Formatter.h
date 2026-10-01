#ifndef FORMATTER_H
#define FORMATTER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <memory>
#include <type_traits>
#include <utility>
#include "Measurement.h"

// תבנית ראשית
template <typename T>
struct Formatter {
    static std::string format(const T& val) {
        std::ostringstream oss;
        oss << val;
        return oss.str();
    }
};

// התמחות מלאה double
template <>
struct Formatter<double> {
    static std::string format(const double& val) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << val;
        return oss.str();
    }
};

// התמחות מלאה bool
template <>
struct Formatter<bool> {
    static std::string format(const bool& val) {
        return val ? "full" : "empty";
    }
};

// התמחות חלקית Measurement<U>
template <typename U>
struct Formatter<Measurement<U>> {
    static std::string format(const Measurement<U>& m) {
        return Formatter<U>::format(m.value) + m.unit + " @d" + std::to_string(m.day);
    }
};

// התמחות חלקית std::vector<U>
template <typename U>
struct Formatter<std::vector<U>> {
    static std::string format(const std::vector<U>& vec) {
        std::string res = "[";
        for (std::size_t i = 0; i < vec.size(); ++i) {
            res += Formatter<U>::format(vec[i]);
            if (i + 1 < vec.size()) res += ", ";
        }
        res += "]";
        return res;
    }
};

// התמחות חלקית std::unique_ptr<U>
template <typename U>
struct Formatter<std::unique_ptr<U>> {
    static std::string format(const std::unique_ptr<U>& ptr) {
        if (!ptr) return "<null>";
        return Formatter<U>::format(*ptr);
    }
};

// פונקציית עטיפה
template <typename T>
std::string fmt(const T& val) {
    return Formatter<T>::format(val);
}

// בדיקה האם T באמת ניתן להצגה (לא רק ש-fmt קיים בחתימה)
template <typename T, typename = void>
struct IsFormattable : std::false_type {};

template <typename T>
struct IsFormattable<T, std::void_t<decltype(std::declval<std::ostringstream&>() << std::declval<const T&>())>>
    : std::true_type {};

template <typename U>
struct IsFormattable<Measurement<U>, std::enable_if_t<IsFormattable<U>::value>> : std::true_type {};

template <typename U>
struct IsFormattable<std::vector<U>, std::enable_if_t<IsFormattable<U>::value>> : std::true_type {};

template <typename U>
struct IsFormattable<std::unique_ptr<U>, std::enable_if_t<IsFormattable<U>::value>> : std::true_type {};

#endif // FORMATTER_H
