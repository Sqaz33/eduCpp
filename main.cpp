#include <iostream>
#include <type_traits>
#include <memory>
#include <cassert>



int main() {
    std::weak_ptr<int> b;

    {
        auto a = std::make_shared<int>(1);
        std::weak_ptr<int> b = a;

        auto l = b.lock();
    }

    assert(b.expired());
    assert(b.lock() == nullptr);

}