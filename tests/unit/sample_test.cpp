#include "tests/framework/mystl_test.hpp"

int main() {
  return ::mystl_test::run_all();
}

MYSTL_TEST(core_smoke, {
  MYSTL_EXPECT(true);
  MYSTL_EXPECT_EQ(1, 1);
});


