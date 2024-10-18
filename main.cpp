#include <iostream>

struct S {
    // operator int() {
    //     return 0;
    // }
};

int main() {
    S s;
    auto x = int(s);
}

