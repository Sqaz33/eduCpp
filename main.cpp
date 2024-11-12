#include <memory>
#include <array>
#include <iostream>
#include <utility>
#include <thread>
#include <future>


struct S {
    int x = 0;

    operator bool() { return x; }
};

int main() {    
    S v;
    v.x = 1;

    bool y = v;

    if (v) std::cout << 1 << '\n'; 
    else std::cout << 0 << '\n';
}