#include <iostream>
#include <limits>


constexpr int f() {
    if consteval {
        return 1;
    } else {
        return 0;
    }
}


int main() {
    constexpr int x = f();
    int y = f();

    std::cout << x << ' ' << y << '\n';


}
