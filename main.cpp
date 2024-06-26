#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

// области видимости
// int x = 10; // Глобальная переменная
// int main() {
//     int x = 20; // Локальная переменная
//     std::cout << "Локальная x: " << x << std::endl;
//     std::cout << "Глобальная x: " << ::x << std::endl; // Обращение к глобальной переменной x
//     {
//         cout << x << endl;
//     }
// }

int main() {
    const int x = 10;
    int i[x] {};

    for (auto &n : i) {
        cout << n << endl;
    }


}