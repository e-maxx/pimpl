#pragma once

namespace pimpl {

/**
 * Smart pointer providing sole ownership and deep copy semantics.
 *
 * From the user point of view, that's a std::unique_ptr with additional copy
 * constructor and copy assignment operator (that delegate to the T's copy
 * constructor and copy assignment operator correspondingly), and with dynamic
 * deleter.
 *
 * The completeness of type T is required only when instantiating the
 * CopyablePtr(T*) constructor; all other CopyablePtr members can work when
 * the T type is incomplete yet.
 */
template <typename T>
class CopyablePtr {
    /**
     * Abstract class providing methods for T cloning and destruction.
     *
     * The actual implementation is located inside the Traits subclass; this
     * trick is necessary in order to work when T type is incomplete yet. The
     * magic is that completeness of T is required only when we instantiate
     * the Traits class, that happens only in the CopyablePtr(T*)
     * constructor. All other CopyablePtr members operate only with a pointer
     * to the TraitsBase class, and thus don't require T completeness.
     */
    class TraitsBase {
    public:
        virtual ~TraitsBase()
        { }

        virtual void destroy(T* pointer) const = 0;

        virtual T* clone(const T& other) const = 0;
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

        T* clone(const T& other) const
        {
            return new T(other);
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
     * (which, in turn, is the same among all CopyablePtr's of the same T).
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
    explicit CopyablePtr(T* pointer)
        : traits_(makeTraits())
        , pointer_(pointer)
    { }

    /**
     * Construct and own a deep copy of the specified object.
     *
     * Provides basic exception safety guarantee.
     */
    CopyablePtr(const CopyablePtr& other)
        : traits_(other.traits_)
        , pointer_(traits_->clone(*other.pointer_))
    { }

    /**
     * Move from another CopyablePtr, taking ownership over its object.
     *
     * The other CopyablePtr will be left in an unspecified state.
     */
    CopyablePtr(CopyablePtr&& other) noexcept
        : traits_(other.traits_)
        , pointer_(other.pointer_)
    {
        other.pointer_ = nullptr;
    }

    ~CopyablePtr()
    {
        if (pointer_) {
            traits_->destroy(pointer_);
        }
    }

    /**
     * Construct and own a deep copy of the specified object.
     *
     * Provides strong exception safety guarantee.
     */
    CopyablePtr& operator=(const CopyablePtr& other)
    {
        if (this != &other) {
            /* Here we don't use the "canonical" implementation as it'll be
             * sub-optimal.
             */
            T* const newPointer = traits_->clone(*other.pointer_);
            traits_->destroy(pointer_);
            pointer_ = newPointer;
        }
        return *this;
    }

    /**
     * Move from another CopyablePtr, taking ownership over its object.
     */
    CopyablePtr& operator=(CopyablePtr&& other) noexcept
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
