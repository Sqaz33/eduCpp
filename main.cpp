#include <concepts>
#include <iostream>

template <std::signed_integral T>
void foo(T) { std::cout << "signed\n"; }

template <std::unsigned_integral T>
void foo(T) { std::cout << "unsigned\n"; }

int main() {
    foo(1u);
    foo(1);
}
