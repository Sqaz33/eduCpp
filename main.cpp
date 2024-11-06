#include <iostream>
#include <type_traits>
#include <thread>
#include <mutex>

namespace prekol {

struct Buf {
    Buf() { std::cout << "Buf::Buf()\n"; }
    ~Buf() { std::cout << "Buf::~Buf()\n"; }
};

void do_tmng() { throw std::bad_exception(); }

struct Impl : Buf {
    Impl() : Buf() {
        do_tmng();
    }
};

} // namespace prekol

int main() {
    using namespace prekol;

    try { Impl i; } catch(...) { }
    // on display
    // Buf::Buf() 
    // Buf::~Buf()
}