#include "b.h"

class B::Impl {
public:
    Impl()
    { }
    Impl(const Impl&) = delete;
};

B::B()
{ }
