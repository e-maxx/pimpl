#include "a.h"
#include <iostream>


void dump(const std::string& title, const A& a)
{
    std::cout << title << ": x=" << a.x() << " y=" << a.y() << std::endl;
}

int main()
{
    A a1;
    dump("a1", a1);

    A a2 = a1;
    dump("a2", a2);

    a1.setX("new_x_1");
    a1.setY("new_y_1");
    dump("a1", a1);
    dump("a2", a2);

    a2.setX("new_x_2");
    a2.setY("new_y_2");
    dump("a1", a1);
    dump("a2", a2);

    A a3;
    a3 = a2;
    dump("a1", a1);
    dump("a2", a2);
    dump("a3", a3);

    A a4;
    a4 = std::move(a2);
    dump("a1", a1);
    dump("a3", a3);
    dump("a4", a4);
}
