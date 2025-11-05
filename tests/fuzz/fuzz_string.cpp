#include <cstddef>
#include <cstdint>
#include <string>

// 简单 fuzz：构造 std::string 并调用若干方法（作为占位，后续替换为 mystl::string）

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  std::string s(reinterpret_cast<const char*>(data), size);
  // 调用一些方法以覆盖代码路径
  (void)s.size();
  (void)s.find('a');
  (void)s.substr(0, s.size() ? s.size() - 1 : 0);
  return 0;
}


