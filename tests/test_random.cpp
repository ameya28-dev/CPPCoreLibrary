#include <gtest/gtest.h>
#include <core/random/random.hpp>

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

TEST(UniformIntTest, EdgeRanges) {
    core::UniformInt gen(0, 0); // Min == Max
    for(int i = 0; i < 10; ++i) {
        EXPECT_EQ(gen.next(), 0);
    }
    
    core::UniformInt gen2(-5, 5);
    for(int i = 0; i < 100; ++i) {
        int val = gen2.next();
        EXPECT_GE(val, -5);
        EXPECT_LE(val, 5);
    }
}

TEST(UniformIntTest, LargeRange) {
    core::UniformInt gen(1000000, 2000000);
    for(int i = 0; i < 50; ++i) {
        int val = gen.next();
        EXPECT_GE(val, 1000000);
        EXPECT_LE(val, 2000000);
    }
}

