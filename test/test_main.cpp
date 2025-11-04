#include <gtest/gtest.h>
#include <mystl/stl.hpp>

TEST(BasicTest, SanityCheck) {
    EXPECT_TRUE(true);
}

TEST(BasicTest, VersionCheck) {
    EXPECT_GT(1, 0);
}
