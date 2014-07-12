#pragma once

#include <pimpl.h>
#include <string>

class C {
    class Impl;
    pimpl::SharedPimpl<Impl> impl_;

public:
    C();
    std::string z() const;
    void setZ(std::string z);
};
