#include <iostream>
#include <type_traits>

// 16.5 Typelist metacontainer and value-based metaprogramming.

template <typename... Ts>
struct typelist;

template <class... Ts>
struct front;

template <class T, class... Ts>
struct front<typelist<T, Ts...>> : 
    std::type_identity<T>
{};

template <class T, class... Ts>
using front_t = front<T, Ts...>::type;

template <class... Ts>
struct pop_front;

template <class T, class... Ts>
struct pop_front<typelist<T, Ts...>> : 
    std::type_identity<typelist<Ts...>> 
{};

template <class T, class... Ts>
using pop_front_t = pop_front<typelist<T, Ts...>>::type;

int main() {
    static_assert(
        std::is_same_v<
            pop_front_t<typelist<int, double>>,
            typelist<int>
        >
    );
}