#include <iostream>
#include <thread> // для работы с потоками
#include <windows.h>

using std::cout;
using std::endl;

void foo(int i, ...);

struct P {
    int x;
    int y;
    int z;
};

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    int x = 1;
    int &y = x;
    int &z = y;
    cout << z;
}
