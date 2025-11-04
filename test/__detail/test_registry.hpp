//
// MySTL Test - Test Registry Implementation
// C++23 compatible
//
// 自动注册与执行单元测试：
// - 测试用例注册机制
// - 测试套件管理
// - 测试执行框架
// - 测试结果收集和报告
//

#ifndef MYSTL_TEST_DETAIL_TEST_REGISTRY_HPP
#define MYSTL_TEST_DETAIL_TEST_REGISTRY_HPP

#include <mystl/config.hpp>
#include <functional>
#include <vector>
#include <string>
#include <map>

namespace mystl::test::__detail {

// 测试用例注册表
class test_registry {
public:
    static test_registry& instance() {
        static test_registry registry;
        return registry;
    }
    
    void register_test(const std::string& suite, const std::string& name, 
                       std::function<void()> test_func) {
        tests_[suite].emplace_back(name, std::move(test_func));
    }
    
    const auto& get_tests() const {
        return tests_;
    }
    
private:
    std::map<std::string, std::vector<std::pair<std::string, std::function<void()>>>> tests_;
};

} // namespace mystl::test::__detail

#endif // MYSTL_TEST_DETAIL_TEST_REGISTRY_HPP

