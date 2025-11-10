#ifndef MYSTL_STL_HPP
#define MYSTL_STL_HPP

// Aggregated public header for MySTL

#include "config/config.hpp"

// core utilities
#include "core/assert.hpp"
#include "core/move_if_noexcept.hpp"
#include "core/utility.hpp"

// memory
#include "memory/allocator.hpp"
#include "memory/allocator_traits.hpp"
#include "memory/shared_ptr.hpp"
#include "memory/uninitialized.hpp"
#include "memory/unique_ptr.hpp"
#include "memory/weak_ptr.hpp"

// iterator
#include "iterator/concepts.hpp"
#include "iterator/facade.hpp"
#include "iterator/traits.hpp"

// containers (most used)
#include "containers/array.hpp"
#include "containers/deque.hpp"
#include "containers/forward_list.hpp"
#include "containers/list.hpp"
#include "containers/span.hpp"
#include "containers/string.hpp"
#include "containers/string_view.hpp"
#include "containers/vector.hpp"

// algorithms
#include "algorithms/heap.hpp"
#include "algorithms/modifying.hpp"
#include "algorithms/non_modifying.hpp"
#include "algorithms/numeric.hpp"
#include "algorithms/sorting.hpp"

#endif  // MYSTL_STL_HPP
