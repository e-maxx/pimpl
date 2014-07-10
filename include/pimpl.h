#pragma once

#include "copy_ptr.h"
#include <utility>


template <typename T>
class Pimpl {
private:
    CopyPtr<T> ptr_;

public:
    template <typename... Args>
    explicit Pimpl(Args&&... args)
        : ptr_(new T(std::forward<Args>(args)...))
    { }

    Pimpl(const Pimpl&) = default;
    Pimpl(Pimpl&&) = default;

    Pimpl& operator=(const Pimpl&) = default;
    Pimpl& operator=(Pimpl&&) = default;

    T& operator*()
    {
        return *ptr_;
    }

    const T& operator*() const
    {
        return *ptr_;
    }

    T* operator->()
    {
        return ptr_.operator->();
    }

    const T* operator->() const
    {
        return ptr_.operator->();
    }
};
