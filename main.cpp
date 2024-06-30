#include <iostream>
#include <thread> // для работы с потоками
#include <windows.h>
#include <string>
#include <type_traits>

using std::cout;
using std::endl;
using std::cin;


// частичная специализация 
template<class T>
void foo(T& v) {
    T vb = v;
    cout << "Regular foo called" << endl;
}

// Перегрузка для const T
template<class T>
void foo(const T& v) {
    T vb = v;
    cout << "Overloaded foo for const called" << endl;
}

template<typename T>
struct S {
    typedef T type;
};

template<typename T>
struct S <const T> {
    typedef T type;
};


int main() {
    // Пример использования

    return 0;
}


 
