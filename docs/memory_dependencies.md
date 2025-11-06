# Memory 模块依赖关系分析与开发规划

## 1. 依赖关系图

```
┌─────────────────┐
│ pointer_traits  │ (独立，最基础)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   allocator     │ (独立，基础)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│allocator_traits │ (依赖 allocator + pointer_traits)
└─────────────────┘

┌─────────────────┐
│  uninitialized  │ (依赖 core/move_if_noexcept，独立于其他 memory)
└─────────────────┘

┌─────────────────┐
│   unique_ptr    │ (独立)
└─────────────────┘

┌─────────────┐      ┌─────────────┐
│ shared_ptr  │◄────►│  weak_ptr   │ (相互依赖，需要一起实现)
└─────────────┘      └─────────────┘
```

## 2. 详细依赖分析

### 2.1 pointer_traits.hpp
**依赖：**
- 无（只依赖标准库：`<cstddef>`, `<type_traits>`）

**被依赖：**
- `allocator_traits.hpp`（通过标准库的 `std::pointer_traits` 间接使用）

**特点：**
- 最基础的组件，不依赖任何其他 memory 模块
- 提供统一的指针类型萃取接口

### 2.2 allocator.hpp
**依赖：**
- 无（只依赖标准库：`<cstddef>`, `<new>`, `<limits>`）

**被依赖：**
- `allocator_traits.hpp`（直接包含）

**特点：**
- 独立的基础组件
- 提供内存分配接口

### 2.3 allocator_traits.hpp
**依赖：**
- `allocator.hpp`（直接包含）
- `pointer_traits`（通过标准库间接使用，但应该使用 mystl 版本）

**被依赖：**
- 所有容器（vector、list、deque 等）
- `uninitialized.hpp`（可能间接使用）

**特点：**
- 分配器系统的核心，提供统一的分配器操作接口
- 依赖基础组件，应该较晚实现

### 2.4 uninitialized.hpp
**依赖：**
- `core/move_if_noexcept.hpp`（直接包含）
- 标准库：`<memory>`, `<type_traits>`

**被依赖：**
- 所有容器（用于构造和销毁对象）
- `allocator_traits.hpp`（可能间接使用）

**特点：**
- 相对独立，只依赖 core 模块
- 提供未初始化内存操作，是容器实现的基础

### 2.5 unique_ptr.hpp
**依赖：**
- 无（只依赖标准库：`<cstddef>`, `<utility>`, `<type_traits>`）

**被依赖：**
- 容器（可能用于内部资源管理）

**特点：**
- 完全独立，可以优先实现
- 零开销抽象，适合用于资源管理

### 2.6 shared_ptr.hpp
**依赖：**
- `weak_ptr.hpp`（前向声明，相互依赖）

**被依赖：**
- `weak_ptr.hpp`（相互依赖）

**特点：**
- 需要与 weak_ptr 一起实现
- 需要控制块（__details 中实现）

### 2.7 weak_ptr.hpp
**依赖：**
- `shared_ptr.hpp`（前向声明，相互依赖）

**被依赖：**
- `shared_ptr.hpp`（相互依赖）

**特点：**
- 需要与 shared_ptr 一起实现
- 依赖 shared_ptr 的控制块

## 3. 开发步骤规划

### 阶段 1：基础组件（最优先，可并行开发）

#### 步骤 1.1：pointer_traits
**优先级：** ⭐⭐⭐⭐⭐（最高）
**依赖：** 无
**工作量：** 小（1-2 小时）
**实现内容：**
- 基础模板定义（提取自定义指针类型的成员）
- 原生指针特化（T*, const T*, volatile T*, const volatile T*）
- rebind 机制
- pointer_to 静态函数

**验收标准：**
- 编译通过
- 类型提取正确
- rebind 功能正常

#### 步骤 1.2：allocator
**优先级：** ⭐⭐⭐⭐⭐（最高）
**依赖：** 无
**工作量：** 小（2-3 小时）
**实现内容：**
- 类型成员定义
- allocate/deallocate 实现
- rebind 机制
- construct/destroy（可选，C++17 前）
- max_size
- select_on_container_copy_construction

**验收标准：**
- 编译通过
- allocate/deallocate 功能正确
- 异常安全保证（allocate 失败抛出 bad_alloc）

#### 步骤 1.3：unique_ptr
**优先级：** ⭐⭐⭐⭐（高）
**依赖：** 无
**工作量：** 中（3-4 小时）
**实现内容：**
- default_delete（单对象和数组版本）
- unique_ptr 单对象版本
- unique_ptr 数组版本特化
- 所有构造函数、析构函数、赋值运算符
- 核心接口（get、release、reset、operator*、operator->）
- 比较运算符和非成员函数
- make_unique（C++14，可选）

**验收标准：**
- 编译通过
- 移动语义正确（noexcept）
- 删除器正确调用
- 与 std::unique_ptr 行为一致（属性测试）

### 阶段 2：分配器系统（依赖阶段 1）

#### 步骤 2.1：allocator_traits
**优先级：** ⭐⭐⭐⭐⭐（最高）
**依赖：** pointer_traits (步骤 1.1), allocator (步骤 1.2)
**工作量：** 中（3-4 小时）
**实现内容：**
- 类型提取（使用 SFINAE 处理可选类型）
- allocate/deallocate 静态函数（调用分配器或默认实现）
- construct/destroy 静态函数（使用完美转发）
- max_size 静态函数
- select_on_container_copy_construction 静态函数
- rebind 机制

**验收标准：**
- 编译通过
- 支持可选接口的默认实现
- 与 std::allocator_traits 行为一致
- 与 uninitialized 配合使用正常

### 阶段 3：未初始化内存操作（相对独立）

#### 步骤 3.1：uninitialized
**优先级：** ⭐⭐⭐⭐⭐（最高）
**依赖：** core/move_if_noexcept (已完成)
**工作量：** 大（4-5 小时）
**实现内容：**
- construct_at / destroy_at（C++17/C++20）
- destroy / destroy_n
- uninitialized_copy / uninitialized_copy_n
- uninitialized_move / uninitialized_move_n（C++17）
- uninitialized_fill / uninitialized_fill_n
- uninitialized_default_construct / uninitialized_default_construct_n（C++17）
- uninitialized_value_construct / uninitialized_value_construct_n（C++17）
- 异常安全处理（强异常保证）
- 平凡类型优化（可选）

**验收标准：**
- 编译通过
- 所有函数功能正确
- 异常安全保证（构造失败时已构造对象被销毁）
- 与 std 版本行为一致（属性测试）
- Sanitizers 通过（无内存泄漏）

### 阶段 4：共享所有权智能指针（可延后）

#### 步骤 4.1：shared_ptr + weak_ptr（一起实现）
**优先级：** ⭐⭐⭐（中，可延后到 W2 之后）
**依赖：** 无（但需要设计控制块）
**工作量：** 大（6-8 小时）
**实现内容：**
- 控制块设计（__details 命名空间）
  - 引用计数（原子操作）
  - 弱引用计数（原子操作）
  - 删除器存储
  - 分配器存储（可选）
- shared_ptr 实现
  - 所有构造函数、析构函数、赋值运算符
  - 核心接口（get、reset、use_count、operator*、operator->）
  - 引用计数管理
- weak_ptr 实现
  - 所有构造函数、析构函数、赋值运算符
  - lock、expired、use_count
- make_shared / allocate_shared
- 类型转换函数（static_pointer_cast 等）

**验收标准：**
- 编译通过
- 引用计数正确
- 线程安全（引用计数使用原子操作）
- 循环引用测试通过
- 与 std 版本行为一致（属性测试）
- Sanitizers 通过（无内存泄漏）

## 4. 开发顺序总结

### 推荐顺序（按优先级和依赖关系）

```
第1批（并行开发，无依赖）：
├── pointer_traits     (1-2 小时)
├── allocator          (2-3 小时)
└── unique_ptr         (3-4 小时)

第2批（依赖第1批）：
└── allocator_traits   (3-4 小时，依赖 pointer_traits + allocator)

第3批（相对独立，可并行）：
└── uninitialized      (4-5 小时，依赖 core/move_if_noexcept)

第4批（可延后）：
└── shared_ptr + weak_ptr (6-8 小时，相互依赖，一起实现)
```

### 最小可用集合（MVP）

要实现第一个容器（如 vector），至少需要：
1. ✅ pointer_traits（最基础）
2. ✅ allocator（内存分配）
3. ✅ allocator_traits（统一接口）
4. ✅ uninitialized（对象构造/销毁）

unique_ptr 可以稍后实现（用于容器内部资源管理）。
shared_ptr/weak_ptr 可以延后到后续阶段。

## 5. 实现建议

### 5.1 测试策略
- 每个组件实现后立即编写单元测试
- 使用属性测试验证与 std 版本的行为一致性
- 使用 Sanitizers 检测内存问题
- 异常安全测试（构造失败场景）

### 5.2 代码质量
- 遵循《Effective C++》和《More Effective C++》
- 异常安全保证明确标注
- 使用 move_if_noexcept 优化
- 禁止未测量的过早优化

### 5.3 性能考虑
- 平凡类型的特化（uninitialized 中）
- 零开销抽象（unique_ptr）
- 原子操作优化（shared_ptr 引用计数）

## 6. 里程碑检查点

- **里程碑 1**：阶段 1 完成（pointer_traits + allocator + unique_ptr）
  - 可以开始测试基础内存管理功能

- **里程碑 2**：阶段 2 完成（allocator_traits）
  - 分配器系统完整，可以开始容器开发

- **里程碑 3**：阶段 3 完成（uninitialized）
  - 可以开始实现第一个容器（vector）

- **里程碑 4**：阶段 4 完成（shared_ptr + weak_ptr）
  - Memory 模块完整，支持所有智能指针功能

