#include "random/random.hpp"

core::UniformInt::UniformInt() : _gen(std::random_device{}()), _distribution(0, 9) {}

core::UniformInt::UniformInt(const int min, const int max) : _gen(std::random_device{}()), _distribution(min, max) {}

auto core::UniformInt::next() -> int {
    return _distribution(_gen);
}

auto core::UniformInt::getVector(const std::size_t size) -> std::vector<int> {
    std::vector<int> res(size);
    for (size_t i = 0; i < size; i++) {
        res[i] = _distribution(_gen);
    }
    return res;
}

core::UniformReal::UniformReal() : _gen(std::random_device{}()) {}

core::UniformReal::UniformReal(const double min, const double max)
    : _gen(std::random_device{}()), _distribution(min, max) {}

core::UniformReal::UniformReal(const double min, const double max, const bool inclusive)
    : _gen(std::random_device{}()) {
    if (inclusive) {
        _distribution = std::uniform_real_distribution<>(min, std::nextafter(max, std::numeric_limits<double>::max()));
    } else {
        _distribution = std::uniform_real_distribution<>(min, max);
    }
}

auto core::UniformReal::next() -> double {
    return _distribution(_gen);
}

auto core::UniformReal::getVector(const std::size_t size) -> std::vector<double> {
    std::vector<double> res(size);
    for (int i = 0; i < size; i++) {
        res[i] = _distribution(_gen);
    }
    return res;
}

core::Normal::Normal() : _gen(std::random_device{}()) {}

core::Normal::Normal(const double mean, const double stdDev)
    : _gen(std::random_device{}()), _distribution(mean, stdDev) {}

auto core::Normal::next() -> double {
    return _distribution(_gen);
}

auto core::Normal::getVector(const std::size_t size) -> std::vector<double> {
    std::vector<double> res(size);
    for (int i = 0; i < size; i++) {
        res[i] = _distribution(_gen);
    }
    return res;
}
