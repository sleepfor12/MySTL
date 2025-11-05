#ifndef MYSTL_STL_HPP
#define MYSTL_STL_HPP

// Aggregated public header for MySTL

#include "config/config.hpp"

// core utilities
#include "core/assert.hpp"
#include "core/utility.hpp"
#include "core/move_if_noexcept.hpp"

// memory
#include "memory/allocator.hpp"
#include "memory/allocator_traits.hpp"
#include "memory/uninitialized.hpp"
#include "memory/unique_ptr.hpp"
#include "memory/shared_ptr.hpp"
#include "memory/weak_ptr.hpp"

// iterator
#include "iterator/concepts.hpp"
#include "iterator/traits.hpp"
#include "iterator/facade.hpp"

// containers (most used)
#include "containers/vector.hpp"
#include "containers/array.hpp"
#include "containers/deque.hpp"
#include "containers/list.hpp"
#include "containers/forward_list.hpp"
#include "containers/string.hpp"
#include "containers/string_view.hpp"
#include "containers/span.hpp"

// algorithms
#include "algorithms/non_modifying.hpp"
#include "algorithms/modifying.hpp"
#include "algorithms/sorting.hpp"
#include "algorithms/heap.hpp"
#include "algorithms/numeric.hpp"

#endif  // MYSTL_STL_HPP


