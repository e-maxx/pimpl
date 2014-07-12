#pragma once

#include <pimpl/copyable_ptr.h>
#include <utility>

namespace pimpl {

/**
 * Pimpl ("pointer-to-implementation") idiom implementation with deep copying.
 *
 * This class is a storage of pointer to an Impl class object, where the Impl
 * class does not need to be complete type if you don't construct new
 * Pimpl's.
 *
 * This allows you to hide class data members and other implementation
 * details from exposing them in a header file: your class can just contain a
 * single data member Pimpl<Impl>, where Impl is some incomplete type (that
 * is defined in a cpp-file along with all other implementation details).
 *
 * The Pimpl idiom (a.k.a. "Opaque pointer" or "Cheshire Cat technique")
 * helps to reduce recompilation times and to maintain binary compatibility.
 *
 * This class, in contrast with the Pimpl class, provides copy constructor and
 * copy assignment operators, that delegate to the Impl's copy constructor and
 * copy assignment, correspondingly.
 */
template <typename Impl>
class CopyablePimpl {
    CopyablePtr<Impl> ptr_;

public:
    /**
     * Construct and store a new Impl with the specified arguments.
     *
     * The Impl type must be a complete type when instantiating this
     * function.
     */
    template <typename... Args>
    explicit CopyablePimpl(Args&&... args)
        : ptr_(new Impl(std::forward<Args>(args)...))
    { }

    /**
     * Construct and store a deep copy of the specified object.
     *
     * Provides basic exception safety guarantee.
     */
    CopyablePimpl(const CopyablePimpl&) = default;

    /**
     * Move from the specified object, taking ownership over the Impl object.
     */
    CopyablePimpl(CopyablePimpl&&) noexcept = default;

    /**
     * Construct and store a deep copy of the specified object.
     *
     * Provides strong exception safety guarantee.
     */
    CopyablePimpl& operator=(const CopyablePimpl&) = default;

    /**
     * Move from the specified object, taking ownership over the Impl object.
     */
    CopyablePimpl& operator=(CopyablePimpl&&) noexcept = default;

    Impl& operator*()
    {
        return *ptr_;
    }

    const Impl& operator*() const
    {
        return *ptr_;
    }

    Impl* operator->()
    {
        return ptr_.operator->();
    }

    const Impl* operator->() const
    {
        return ptr_.operator->();
    }
};

} // namespace pimpl
