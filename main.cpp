#include <iostream>
#include <thread> // для работы с потоками
#include <windows.h>

#include <string>

using std::cout;
using std::endl;
using std::cin;

class Base {
public:
    int x = 0;
};

class Derived : public Base {};

int main() {
    Derived derived;
    Base& b = derived;
    // Derived* d = &Base() так нельзя
    static_cast<Derived&>(b);

}

