#pragma once


template <typename T>
class CopyPtr {
    struct TraitsBase {
        virtual ~TraitsBase()
        { }

        virtual void destroy(T* pointer) const = 0;

        virtual T* clone(const T& other) const = 0;
    };

    struct Traits : public TraitsBase {
        void destroy(T* pointer) const
        {
            delete pointer;
        }

        T* clone(const T& other) const
        {
            return new T(other);
        }
    };

    static TraitsBase* makeTraits()
    {
        static Traits instance;
        return &instance;
    }

    TraitsBase* traits_;
    T* pointer_;

public:
    explicit CopyPtr(T* pointer)
        : traits_(makeTraits())
        , pointer_(pointer)
    { }

    CopyPtr(const CopyPtr& other)
        : traits_(other.traits_)
        , pointer_(traits_->clone(*other.pointer_))
    { }

    CopyPtr(CopyPtr&& other) noexcept
        : traits_(other.traits_)
        , pointer_(other.pointer_)
    {
        other.pointer_ = nullptr;
    }

    ~CopyPtr()
    {
        if (pointer_) {
            traits_->destroy(pointer_);
        }
    }

    CopyPtr& operator=(const CopyPtr& other)
    {
        if (this != &other) {
            T* const newPointer = traits_->clone(*other.pointer_);
            traits_->destroy(pointer_);
            pointer_ = newPointer;
        }
        return *this;
    }

    CopyPtr& operator=(CopyPtr&& other) noexcept
    {
        if (this != &other) {
            traits_->destroy(pointer_);
            pointer_ = other.pointer_;
            other.pointer_ = nullptr;
        }
        return *this;
    }

    T& operator*()
    {
        return *pointer_;
    }

    const T& operator*() const
    {
        return *pointer_;
    }

    T* operator->()
    {
        return pointer_;
    }

    const T* operator->() const
    {
        return pointer_;
    }
};
