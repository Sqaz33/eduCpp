#include <iostream>
#include <functional>
#include <cassert>
#include <algorithm>

template <class F> struct fire_once {
    fire_once(F&& f) : f(std::forward<F>(f)) {}

    template <class... Args>
    decltype(auto) operator()(Args&&... args) {
        if (count-- > 0) {
            return f(std::forward<Args>(args)...);
        }
        assert(false);
    }

private:
    int count = 1;
    F f;
};

int main() {
    fire_once square = [](int x) { return x * x; };
    // std::cout << square(2) << '\n';

    int x = 1;
    fire_once threeSum = [&x](int a, int b) {return a + b + x;};

    std::cout << threeSum(1, 2) << '\n';
    
}   