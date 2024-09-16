#include <iostream>
#include <type_traits>


// const metafunction 
template <class T, T v>
struct integral_constant {
    static constexpr T value = v;
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// is_same
template <class T, class U>
struct is_same : false_type {};

template <class T>
struct is_same<T, T> : true_type {};

template <class T, class U>
constexpr bool is_same_v = is_same<T, T>::value;

//-----------------------------
//type identity
template <class T>
struct type_identity {
    using type = T;
};

template <class T>
struct remove_reference : type_identity<T> {};

template <class T>
struct remove_reference<T&> : type_identity<T> {};
 
template <class T>
struct remove_reference<T&&> : type_identity<T> {};

template <class T>
using remove_reference_t = remove_reference<T>::type;

//-----------------------------
template <class T>
struct is_pointer : false_type {};

template <class T>
struct is_pointer<T*> : true_type {};

template <class T>
constexpr bool is_pointer_v = is_pointer<T>::value;


//-----------------------------
// conjunction работает с true/fasle_type, и возвращает true/false_type
template <class...> struct conjunction : true_type {};

template <class B1> struct conjunction<B1> : B1 {};

template <class B1, class... Bn> 
struct conjunction<B1, Bn...> : std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

template <class... B>
constexpr bool conjunction_v = conjunction<B...>::value; 

template <class... Args>
void f(Args&&... args) {
    static_assert(conjunction_v<std::is_lvalue_reference<Args>...>, "no");
    static_assert((std::is_lvalue_reference_v<Args> && ...), "no");

}

//-----------------------------
// SFINAE idiom, enable_if
// Subtitution Failure Is Not An Error
// на этапе выбора могут отбрасываться функции, при выборе которых происходит ошибка
// при этом ошибка компиляции не происходит, продолжается выбор наиболее подходящих 
// функций
template <class T>
decltype(auto) foo(const T&) {
    std::cout << 1 << '\n';
    return 0;
}

template <class... Args>
int foo(Args...) {
    std::cout << 2 << '\n';
    return 0;
}

// enable_if
template <bool B, class T>
struct enable_if {};

template <class T>
struct enable_if<true, T> : type_identity<T> {};

template <bool B, typename T>
using enable_if_t = typename enable_if<B, T>::type; 


template <class C, 
          enable_if_t<std::is_class_v<C>, bool> = true
         >
void bar(const C&) {
    std::cout << "class\n";
}

template <class C, 
          enable_if_t<!std::is_class_v<C>, bool> = true
         >
void bar(const C&) {
    std::cout << "not a class\n";
}

// is_class_or_union
template <class T>
struct is_class_or_union {
    static constexpr bool value = std::conditional_t<bool(std::is_class_v<T>), true_type, 
                                             std::conditional_t<bool(std::is_union_v<T>), true_type, false_type>
                                             >::value;
};

template <class T>
constexpr bool is_class_or_union_v = is_class_or_union<T>::value;


int main() {
    // std::cout << is_pointer_v<int*> << ' ' << is_pointer_v<int> << '\n';
    // std::cout << std::conjunction_v<std::bool_constant<true>>;

    class S {int x;} s;
    // foo(s); // << 1
    // foo(1); // << 2
    
    bar(1);
    bar(s);
    union v {};
    std::cout << is_class_or_union_v<S> << ' '   // << 1
              << is_class_or_union_v<int> << ' ' // << 0
              <<is_class_or_union_v<v> << '\n'; //  << 1

}