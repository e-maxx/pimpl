#include "c.h"

class C::Impl {
public:
    std::string z;

    Impl(std::string z)
        : z(std::move(z))
    { }
};

C::C()
    : impl_("initial_z")
{ }

std::string C::z() const
{
    return impl_->z;
}

void C::setZ(std::string z)
{
    impl_->z = std::move(z);
}
