#include <iostream>
#include <type_traits>

// namespace impl__ {

// template <class T, class... Args>
// constexpr decltype(std::declval<T>().foo(std::declval<Args>()...), true) 
// has_method_foo_helper_(int) { return true; }

// template <class T, class... Args>
// constexpr bool has_method_foo_helper_(...) { return false; }

// } // namespace impl__

// template <class T, class... Args>
// constexpr bool has_method_foo_v = impl__::has_method_foo_helper_<T, Args...>(1);

// struct S { void foo(int) {} };




template <class T, std::enable_if_t<(sizeof(T) >= 4)>* = nullptr>
constexpr void maybe_int(T) { std::cout << "yes\n"; }

template <class T, std::enable_if_t<(sizeof(T) < 4)>* = nullptr>
constexpr void maybe_int(T) { std::cout << "no\n"; }



int main() {
    maybe_int(1);
    maybe_int('c');
}