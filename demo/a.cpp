#include "a.h"

class A::Impl {
public:
    std::string x;
    std::string y;

    Impl(std::string x, std::string y)
        : x(std::move(x))
        , y(std::move(y))
    { }
};

A::A()
    : impl_("initial_x", "initial_y")
{ }

std::string A::x() const
{
    return impl_->x;
}

void A::setX(std::string x)
{
    impl_->x = std::move(x);
}

std::string A::y() const
{
    return (*impl_).y;
}

void A::setY(std::string y)
{
    (*impl_).y = std::move(y);
}
