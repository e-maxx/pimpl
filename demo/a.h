#pragma once

#include <pimpl.h>
#include <string>

class A {
    class Impl;
    pimpl::CopyablePimpl<Impl> impl_;

public:
    A();
    std::string x() const;
    void setX(std::string x);
    std::string y() const;
    void setY(std::string y);
};
