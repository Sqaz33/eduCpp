#include <iostream>
#include <functional>
#include <thread>


int main() {

    std::move_only_function<void(int)> f = [](int a) { std::cout << a << '\n'; };
    std::invoke(std::move(f), 1);
    std::cout << (bool) f << '\n';
    f(2);

}