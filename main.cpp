#include <iostream>
#include <thread> // для работы с потоками
#include <windows.h>

#include <string>

using std::cout;
using std::endl;
using std::cin;

class A {
public:
    // virtual ~A() = default
    virtual void hi() {
        cout << "hi_A" << endl;
    }
};

class B : public A {
public:
    void hi() override {
        cout << "hi_B" << endl;
    }
};

class C : public A {
public:
    void hi() override {
        cout << "hi_C" << endl;
    }
};




int main() {
    A* arr[2];
    arr[0] = new B();
    arr[1] = new C();

    arr[0]->hi();
    arr[1]->hi(); 
    
    A &a = *arr[0];
    B& b = dynamic_cast<B&>(a);
    
    b.hi();
}

