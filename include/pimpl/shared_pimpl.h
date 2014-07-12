#pragma once

#include <memory>
#include <utility>

namespace pimpl {

/**
 * Pimpl ("pointer-to-implementation") idiom implementation with shared state.
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
 * This class, in contrast with the Pimpl class, allows to create copies
 * pointing to the same underlying Impl object (so, in fact, it behaves like
 * std::shared_ptr).
 */
template <typename Impl>
class SharedPimpl {
    std::shared_ptr<Impl> ptr_;

public:
    /**
     * Construct and store a new Impl with the specified arguments.
     *
     * The Impl type must be a complete type when instantiating this
     * function.
     */
    template <typename... Args>
    explicit SharedPimpl(Args&&... args)
        : ptr_(std::make_shared<Impl>(std::forward<Args>(args)...))
    { }

    /**
     * Construct a new "shell" copy pointing to the same underlying object.
     *
     * Provides basic exception safety guarantee.
     */
    SharedPimpl(const SharedPimpl&) = default;

    /**
     * Move from the specified object, taking ownership over the Impl object.
     */
    SharedPimpl(SharedPimpl&&) noexcept = default;

    /**
     * Construct a new "shell" copy pointing to the same underlying object.
     *
     * Provides strong exception safety guarantee.
     */
    SharedPimpl& operator=(const SharedPimpl&) = default;

    /**
     * Move from the specified object, taking ownership over the Impl object.
     */
    SharedPimpl& operator=(SharedPimpl&&) noexcept = default;

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
