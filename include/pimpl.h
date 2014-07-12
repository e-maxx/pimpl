#pragma once

#include "copyable_ptr.h"
#include <utility>


/**
 * Pimpl ("pointer-to-implementation") idiom implementation.
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
 * This Pimpl implementation also provides deep copying capability: Pimpl's
 * copy construction and copy assignment delegate to the Impl's copy
 * construction and copy assignment correspondingly.
 */
template <typename Impl>
class Pimpl {
    CopyablePtr<Impl> ptr_;

public:
    /**
     * Construct and store new Impl with the specified arguments.
     */
    template <typename... Args>
    explicit Pimpl(Args&&... args)
        : ptr_(new Impl(std::forward<Args>(args)...))
    { }

    /**
     * Construct and store a deep copy of the specified object.
     *
     * Provides basic exception safety guarantee.
     */
    Pimpl(const Pimpl&) = default;

    /**
     * Move from the specified object, taking ownership over the Impl object.
     */
    Pimpl(Pimpl&&) noexcept = default;

    /**
     * Construct and store a deep copy of the specified object.
     *
     * Provides strong exception safety guarantee.
     */
    Pimpl& operator=(const Pimpl&) = default;

    /**
     * Move from the specified object, taking ownership over the Impl object.
     */
    Pimpl& operator=(Pimpl&&) noexcept = default;

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
