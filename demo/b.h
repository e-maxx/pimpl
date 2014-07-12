#pragma once

#include <pimpl.h>

class B {
    class Impl;
    pimpl::Pimpl<Impl> impl_;

public:
    B();
};
