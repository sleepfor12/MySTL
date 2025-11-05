#include "tests/framework/mystl_prop.hpp"
#include <vector>
#include <algorithm>
#include <cstdint>

// 示例性质：反转两次等于原序列（使用 std::vector 以演示框架）

MYSTL_PROP(vector_reverse_twice, {
  std::uniform_int_distribution<int> len_dist(0, 100);
  int n = len_dist(rng);
  std::vector<int> v(n);
  for (int i = 0; i < n; ++i) {
    v[i] = static_cast<int>(rng() & 0xFFFF);
  }
  auto orig = v;
  std::reverse(v.begin(), v.end());
  std::reverse(v.begin(), v.end());
  if (v != orig) throw std::runtime_error("reverse twice property failed");
});

int main() {
  int failed = 0;
  failed += mystl_prop_fn_vector_reverse_twice();
  return failed;
}


