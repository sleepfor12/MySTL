#include "tests/framework/mystl_bench.hpp"
#include <vector>

int main() {
  MYSTL_BENCH(vector_push_back_1e5, {
    std::vector<int> v;
    v.reserve(100000);
    for (int i = 0; i < 100000; ++i) v.push_back(i);
  });
  return 0;
}


