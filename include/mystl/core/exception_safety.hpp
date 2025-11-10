#ifndef MYSTL_CORE_EXCEPTION_SAFETY_HPP
#define MYSTL_CORE_EXCEPTION_SAFETY_HPP

// Exception safety guarantees documentation
// Following Effective C++ guidelines: exceptions only for unrecoverable errors

namespace mystl {

// Exception safety levels (documentation only, not enforced at compile time)
//
// NO-THROW: Operation never throws exceptions
// STRONG: Operation either succeeds or has no effect (rollback on failure)
// BASIC: Operation maintains invariants, but may leave objects in modified state
// NONE: No guarantees

// Most container operations provide STRONG guarantee where possible
// Move operations are typically NO-THROW
// Constructors may throw if initialization fails (as per Effective C++)

}  // namespace mystl

#endif  // MYSTL_CORE_EXCEPTION_SAFETY_HPP
