#include "a.h"
#include "b.h"
#include "c.h"
#include <iostream>

void dumpA(const std::string& title, const A& a)
{
    std::cout << title << ": x=" << a.x() << " y=" << a.y() << std::endl;
}

void dumpC(const std::string& title, const C& c)
{
    std::cout << title << ": z=" << c.z() << std::endl;
}

int main()
{
    A a1;
    dumpA("a1", a1);

    A a2 = a1;
    dumpA("a2", a2);

    a1.setX("new_x_1");
    a1.setY("new_y_1");
    dumpA("a1", a1);
    dumpA("a2", a2);

    a2.setX("new_x_2");
    a2.setY("new_y_2");
    dumpA("a1", a1);
    dumpA("a2", a2);

    A a3;
    a3 = a2;
    dumpA("a1", a1);
    dumpA("a2", a2);
    dumpA("a3", a3);

    A a4;
    a4 = std::move(a2);
    dumpA("a1", a1);
    dumpA("a3", a3);
    dumpA("a4", a4);

    B b1;
    B b2 = std::move(b1);

    C c1;
    C c2 = c1;
    dumpC("c1", c1);
    dumpC("c2", c2);

    c1.setZ("new_z_1");
    dumpC("c1", c1);
    dumpC("c2", c2);

    c2.setZ("new_z_2");
    dumpC("c1", c1);
    dumpC("c2", c2);
}
