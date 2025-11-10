#ifndef MYSTL_CONTAINERS_MDSPAN_HPP
#define MYSTL_CONTAINERS_MDSPAN_HPP

// placeholder for mystl::mdspan (C++23)
// Multi-dimensional span over contiguous memory
// Similar to std::mdspan

namespace mystl {

// Layout policies (simplified)
struct layout_left;    // row-major
struct layout_right;   // column-major
struct layout_stride;  // custom stride

template <class ElementType, class Extents, class LayoutPolicy = layout_right,
          class AccessorPolicy = void>
class mdspan;  // to be implemented

// Helper to create mdspan
template <class ElementType, class Extents, class LayoutPolicy = layout_right>
mdspan<ElementType, Extents, LayoutPolicy> make_mdspan(ElementType* data, const Extents& extents);  // to be implemented

}  // namespace mystl

#endif  // MYSTL_CONTAINERS_MDSPAN_HPP
