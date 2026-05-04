#include <gtest/gtest.h>
#include <random/random.hpp>

TEST(UniformIntTest, SingleValueWithinRange) {
    core::UniformInt gen(1, 10);
    for(int i = 0; i < 100; ++i) {
        int val = gen.next();
        EXPECT_GE(val, 1);
        EXPECT_LE(val, 10);
    }
}

TEST(UniformIntTest, VectorSizeAndRange) {
    core::UniformInt gen(1, 10);
    auto vec = gen.getVector(50);
    ASSERT_EQ(vec.size(), 50);
    for(int val : vec) {
        EXPECT_GE(val, 1);
        EXPECT_LE(val, 10);
    }
}

TEST(UniformIntTest, ArrayGeneration) {
    core::UniformInt gen(1, 10);
    auto arr = gen.getArray<5>();
    EXPECT_EQ(arr.size(), 5);
}

TEST(UniformRealTest, RangeCheck) {
    core::UniformReal gen(0.0, 1.0);
    for(int i = 0; i < 100; ++i) {
        double val = gen.next();
        EXPECT_GE(val, 0.0);
        EXPECT_LE(val, 1.0);
    }
}

TEST(NormalTest, VectorGeneration) {
    core::Normal gen(0.0, 1.0);
    auto vec = gen.getVector(10);
    EXPECT_EQ(vec.size(), 10);
}

