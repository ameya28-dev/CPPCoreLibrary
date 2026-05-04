#ifndef CORELIBRARY_INCLUDE_RANDOM_RANDOM_HPP
#define CORELIBRARY_INCLUDE_RANDOM_RANDOM_HPP

#include <array>
#include <random>
#include <vector>

namespace core {
    class UniformInt {
    public:
        UniformInt();

        UniformInt(int, int);

        int next();

        std::vector<int> getVector(size_t);

        template <size_t S>
        std::array<int, S> getArray() {
            std::array<int, S> res{};
            for (int i = 0; i < S; i++) {
                res[i] = _distribution(_gen);
            }
            return res;
        }

    private:
        std::mt19937 _gen;
        std::uniform_int_distribution<> _distribution;
    };

    class UniformReal {
    public:
        UniformReal();

        UniformReal(double, double, bool = false);

        double next();

        std::vector<double> getVector(size_t);

        template <size_t S>
        std::array<double, S> getArray() {
            std::array<double, S> res{};
            for (int i = 0; i < S; i++) {
                res[i] = _distribution(_gen);
            }
            return res;
        }

    private:
        std::mt19937 _gen;
        std::uniform_real_distribution<> _distribution;
    };

    class Normal {
    public:
        Normal();

        Normal(double, double);

        double next();

        std::vector<double> getVector(size_t);

        template <size_t S>
        std::array<double, S> getArray() {
            std::array<double, S> res{};
            for (int i = 0; i < S; i++) {
                res[i] = _distribution(_gen);
            }
            return res;
        }

    private:
        std::mt19937 _gen;
        std::normal_distribution<> _distribution;
    };
} // namespace core

#endif // CORELIBRARY_INCLUDE_RANDOM_RANDOM_HPP
