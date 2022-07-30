#include <gtest/gtest.h>
#include "utils/cgutils.h"

TEST(VEC2ARRAY_TEST, BasicAssertions) {
    std::vector<int> v {1, 2, 3, 4};
    std::array<int, 4> a {1, 2, 3, 4};

    for (int i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], a[i]);
    }
}
