#include <concepts>
#include <iostream>
#include <utility>

#include "things/same.hpp"

template <class... Args>
    requires msa::are_same_v<Args...>
auto sum_all(Args&&... args) {
    return (args + ...);
}

template <std::same_as<int>... Args>
int sum_allInt32(Args&&... args) {
    return (args + ...);
} 

int main() {   
    std::cout << sum_allInt32(1, 2, 3, 4) << '\n';
    std::cout << sum_all(1, 2, 3, 4, 1) << '\n';

}
