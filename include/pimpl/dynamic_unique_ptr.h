#pragma once

namespace pimpl {

/**
 * Smart pointer providing sole ownership.
 *
 * From the user point of view, that's a std::unique_ptr, but with dynamic
 * deleter. This means that he completeness of type T is required only when
 * instantiating the DynamicUniquePtr(T*) constructor; all other
 * DynamicUniquePtr members can work when the T type is incomplete yet.
 */
template <typename T>
class DynamicUniquePtr {
    /**
     * Abstract class providing methods for T cloning and destruction.
     *
     * The actual implementation is located inside the Traits subclass; this
     * trick is necessary in order to work when T type is incomplete yet. The
     * magic is that completeness of T is required only when we instantiate
     * the Traits class, that happens only in the DynamicUniquePtr(T*)
     * constructor. All other DynamicUniquePtr members operate only with a
     * pointer to the TraitsBase class, and thus don't require T completeness.
     */
    class TraitsBase {
    public:
        virtual ~TraitsBase()
        { }

        virtual void destroy(T* pointer) const = 0;
    };

    /**
     * Class with implementation of methods for T cloning and destruction.
     *
     * Instantiation of this class must happen when the T is already a
     * complete type.
     */
    class Traits : public TraitsBase {
    public:
        void destroy(T* pointer) const
        {
            delete pointer;
        }
    };

    /**
     * Get pointer to a Traits object.
     *
     * This function must be called only when the T is already a complete
     * type.
     */
    static Traits* makeTraits()
    {
        static Traits instance;
        return &instance;
    }

    /**
     * Pointer to the traits providing methods for T cloning and destruction.
     *
     * Actually, this will always point to the static object of Traits class
     * (which, in turn, is the same among all DynamicUniquePtr's of the same
     * T).
     */
    TraitsBase* traits_;
    /**
     * Raw pointer to the T object held.
     */
    T* pointer_;

public:
    /**
     * Construct from a pointer, taking an ownership over it.
     */
    explicit DynamicUniquePtr(T* pointer)
        : traits_(makeTraits())
        , pointer_(pointer)
    { }

    /**
     * Move from another DynamicUniquePtr, taking ownership over its object.
     *
     * The other DynamicUniquePtr will be left in an unspecified state.
     */
    DynamicUniquePtr(DynamicUniquePtr&& other) noexcept
        : traits_(other.traits_)
        , pointer_(other.pointer_)
    {
        other.pointer_ = nullptr;
    }

    ~DynamicUniquePtr()
    {
        if (pointer_) {
            traits_->destroy(pointer_);
        }
    }

    /**
     * Move from another DynamicUniquePtr, taking ownership over its object.
     */
    DynamicUniquePtr& operator=(DynamicUniquePtr&& other) noexcept
    {
        if (this != &other) {
            /* Here we don't use the "canonical" implementation as it'll be
             * sub-optimal.
             */
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

} // namespace pimpl
