#include <iostream>
#include <compare>
#include <generator>


std::generator<int> lessTen() {
    int n = 0;
    for (;;) {
        if (n > 10) break;
        co_yield n;
        n += 1;
    }
}

int main() {
    for (auto n : lessTen()) {
        std::cout << n << ' ';
    }
    std::cout << std::endl;
}