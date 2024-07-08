#include <stdio.h>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

//области видимости
int x = 10; // Глобальная переменная
int main() {
    int x = 20; // Локальная переменная
    std::cout << "Локальная x: " << x << std::endl;
    std::cout << "Глобальная x: " << ::x << std::endl; // Обращение к глобальной переменной x
    {
        cout << x << endl;
    }
}


//---------------------------------------------
// выражения 
int foo();

class t {
public:
    int i;
    t() : i(1) {};

    t operator++(int) {
        t temp = *this;
        i++;
        return temp;
    }
};


int main() {
    // оператор присваивания
    // int x;
    // int y;
    // y = x = 3;
    // cout << y << " " << x << endl;

    int x = 5;
    int y = 3;
    t t1;
    cout << t1.i << endl;
}

// тернарный оператор
int foo() {
    int y = 10 + 10;
    int x = 10;

    return (
        y > x ?
        x
        :
        y
    );
}

//--------------------------------------------------
// указатели
int main() {
    SetConsoleOutputCP(CP_UTF8); 
    // указатель на функцию
    void(*f)(int) = &foo;
    (*f)(1);
    
}


void foo(int t) {
    cout << t << endl;
}

// new, delete
int main() {
    SetConsoleOutputCP(CP_UTF8); 
    int i = 0;
    int *ip = new int(1);
    cout << *ip;
    delete ip;
}


//--------------------------------------------------
// ссылки
// Другое название для объекта.
int main() {
    int x;
    int &y = x;
}

void swap(int &x, int &y) {
    int t = x;
    x = y;
    y = t;
}


//--------------------------------------------------------
// const 
int main() {
    const int x = 1;
    int y = x; // так можно
    int &z = x; // так нельзя
    const int &t = x; // так можно
    const int &v = 3; // так можно

    const int l[10] {};
    const int* t1 = l; // указатель на константу, сам t можно менять 
    t1 = t1 + 1;

    int const *t2 = &y; //константный указатель 
    const int const *t3 = &x; // константный указтатель на константу

    const int *s = new int[10]; // так можно
    const int *s1 = new const int[10]; // так нельзя

    
}

//--------------------------------------------------------
// операторы преобразования типов

//для dynamic_cast
class Base {
    virtual void foo() {};
};

class Derived : public Base {
    void foo() override {};
};

int main() {
    // 1. static_cast явное преобразование типов
    // есть нет нужного преобразования, то ошибка
    // T var = static_cast<T>(expression);
    int a = 10; 
    double b = static_cast<double>(a);

    // 2. dynamic_cast преобразование в иерархии наследования
    // T* var = dynamic_cast<T*>(expression);
    Base* base = new Derived();
    Derived* der = dynamic_cast<Derived*>(base);

    // 3. const_cast удаление или добавления const
    // T var = const_cast<T>(expression);
    const int a = 10;
    int* с = const_cast<int*>(&a);
    *с = 10; // неопределенное поведение

    // 4. reinterpret_cast небезопасное побитовое преобразование 
    // T var reinterpret_cast<T>(expression);
    int a = 10;
    int* p = &a;
    long bp = reinterpret_cast<long>(p);

}

//--------------------------------------------------------
// классы
class C {
public:
    C() {
        i = 1;
        j = 2;
    } // конструктор по умолчанию

    C(const C &var) {
        i = var.i;
        j = var.j;
    } // конструктор копирования

    C& operator=(const C &var) {
        i = var.i;
        j = var.j;
    } // оператор присваивания 


    C() = default; // конструктор по умолчанию ген. компилятором
    C() = delete; // конструктор по умолчанию не ген. компилятором
private:
    int i = 0; // значения по умолчанию 
    int j = 1;
};

// список инициализации
class Z {
public:
    Z(int i): i(i) {};
private:
    int i;
};

//explicit
class F {
public:
    explicit F(int i) : i(i) {}; // только аргументы типа заданного типа
private:
    int i;
};


//friend 
class A {
public:
    friend class B; // доступ к protecte и private полям
protected:
    int x;
    int y;
};

class B : public A {
public:
    void foo(A& a) {
        a.x = 0;
        a.y = 0;
    }
};


//mutable
class U {
public:
    U() = default;
    void helloWorld() const {
        cout << "Hello World" << endl;
        helloCount++;
    } 
private:
    mutable int helloCount = 0; // разрешает изменение из const методов
};

//делегирующий конструктор
class O {
public:
    O(char* str, int size) 
    :
    str(str),
    size(size)
    { /*какая-то 100+ строк валидация*/ }

    O(int size, char* str) 
    :
    O(str, size)
    {};
private:
    char* str;
    int size;
};

//указатель на член класса
class C1 {
public:
    int p;
};

int main() {
    int C1::* p = &C1::p;
    C1 c1;
    c1.*p = 3;

    C1 *c1p = &c1;
    c1p->*p = 4;
}   

// перегрузки арифм. операторов.
class MyInt {
public:
    MyInt(int n) : n(n) {};

    MyInt(const MyInt &other) : MyInt(other.n) {};

    MyInt& operator+=(const MyInt &other) {
        n = n + other.n;
        return *this;
    }
    
    MyInt operator+(const MyInt &other) const {
        MyInt nw = *this;
        nw += other;
        return nw;
    }

private:
    int n;
};

// перегрузки [] и ()
class MyStr {
public:
    MyStr() 
    :
    str(new char[100])
    {};

    char& operator[](int index) {
        return str[index];
    }

    void operator()() {
        return;
    }
private:
    char* str;
    int len;
    int power;

 
};

#include <cstring>
// перегрузка c-style cast
class MyStr2 {
public:
    MyStr2() 
    :
    str(new char[100])
    {};

    explicit operator char*() { // explicit - только этот тип без перескастовки
        char *nw = new char[100];
        std::strcpy(nw, str);
        return nw;
    }
private:
    char* str; 
};


//----------------------------------------------------------------------------
// наследование


// Сокрытие имен при наследовании
class A {
public:
    // virtual ~A() = default
    void hi(int) {
        cout << "hi_A" << endl;
    }
};

class B : public A {
public:
    void hi() {
        cout << "hi_B" << endl;
    }
};

int main() {
    B b;
    b.hi(1); // не будет компилироваться
}

// явный квалификатор
int main() {
    B b;
    b.A::hi(1);
}

// использование using
class A2 {
public:
    void hi(int) {
        cout << "hi_A_1" << endl;
    }

    void hi(int, int) {
        cout << "hi_A_2" << endl;
    }
};

class B2 : public A2 {
public:
    using A2::hi; // вставит все перегрузки
    void hi() {
        cout << "hi_B" << endl;
    }
};
int main() {
    B2 b;
    b.hi(1, 2); 
}

// порядок проверки доступа к методу класса
struct B4 {
    void foo() {};
};
struct D4 : B4 {
public:
    void foo(char) {};
private:
    void foo(int) {};
};
int main() {
    D4 d;
    d.foo(); // сначала  проверяет метод на перекрытие, потом только модификатор доступа 
}


// проблемы наследования
struct Granny {
    int a;
    // friend struct Son; не поможет
};

struct Mom : private Granny {
    int b;
    friend struct Son; // поможет, тк Son будет знать о наследовании
};

struct Son : public Mom {
    int c;
    void f() {
        Mom m;
        // Granny g;  нельзя вызвать конструктор, тк для Son нельзя вызвать методы Granny
    }
    // void f(Granny &g) { ошибка: весь тип Granny - запрещен из Son

    // }
    void f(::Granny &g) { // так можно, ищим в глобальной области видимости

    }
};

//----------------------------------------------------------------------------
// о friend
void f3() {
    Mom2 m;
    m.a = 0;
}

struct Granny2 {
protected: 
    int a;
};

struct Mom2 : public Granny2 {
    int b;
    friend void f(); // friend дает те же права, что доступны и методам-членам класса
};

struct Son2 : public Mom2 {
    int c;
};

// порядок вызова конструкторов и деструкторов
struct Granny {
    int a;
    Granny(int a) : a(a) {
        cout << "Grannys constructor" << endl;
    }
    ~Granny() {
        cout << "Grannys destructor" << endl; 
    }
};

struct Mom : public Granny {
    int b;
    Mom(int b, int a) : b(b), Granny(a) {
        cout << "Moms constructor" << endl;
    }  // сам вызовет контсруктор по умолчанию у Granny, если не определить явно
    ~Mom() {
        cout << "Mons destructor" << endl;
    }
};

struct Son : public Mom {
    int c;
    Son(int a, int b, int c) : c(c), Mom(b, c) {
        cout << "Sons constructor" << endl;
    }
    ~Son() {
        cout << "Sons destructor" << endl;
    }
};

int main() {
    Son(1, 2, 3);

    /*
    Grannys constructor
    Moms constructor
    Sons constructor
    Sons destructor
    Mons destructor
    Grannys destructor
    */
}


//----------------------------------------------------------------------------
// ромбовидное наследование 
struct Granny2 {
    int x = 1;
};

struct Mom2 : Granny2 {
    int y = 2;
};

struct Son2 : Mom2, Granny2 {
    int z = 0;
};

int main() {

    Son2 s;
    s.Granny2::x; // о
    s.Mom2::Granny2::x;
}

// виртуальное наследование
struct W {
    int y;
};

struct V {
    int x;
};

struct A : W, virtual V {
    int x;
    int y;
};

struct B : W, virtual V {};

struct D : A, B {};

int main() {
    D d;
    d.x;
}


//----------------------------------------------------------------------------
// приведение типов меожду наследниками
class Base {
public:
    int b = 1234;
    void foo() {
        cout << "Base" << endl;
    }
};

class Derived : public Base {
public:
    int d = 1111;
    void foo() {
        cout << "Derived" << endl;
    }
};

int main() {
    Derived d;
    Base b;
    Base& b1 = d;
    b1.foo(); // вызовется для Base
    d.foo();

    cout << static_cast<Derived&>(b).d;
}


//----------------------------------------------------------------------------
// virtual функции
struct B {
    virtual void foo() {
        cout << "B" << endl;
    }
};

struct D : B {
    void foo() override {
        cout << "D" << endl;
    };
};

int main() {
    B b;
    D d;

    B& b1 = d;
    b1.foo(); // с virtual вызовится D::foo()
}

// работа с другими методами
struct A {
    virtual void foo1() {
        cout << "A1" << endl;
    }
    void foo2() {
        cout << "A2" << endl;
    }
    virtual ~A() = default;
};

struct B : A {
    void foo1() override {
        cout << "B1" << endl;
    }
    void foo2() {
        cout << "B2" << endl;
    }
};


int main() {
    B b;
    A& ab = dynamic_cast<A&>(b);
    ab.foo1();
    ab.foo2();
    // B1  |  <<<
    // A2  |  <<< output 
}



// virtual деструктор
struct B {
    virtual ~B() = default;
    int i = 1;
};

struct D : B {
    int j = 2;
};

int main() {
    D* d = new D;
    B* b = d;
    delete b; // без virtual деструктора будет утечка памяти
}

// другой пример
struct A {
    int i = 0;
    virtual ~A() {
        cout << "A" << endl;
    }
};

struct B : A{ 
    int j = 0;
    ~B() {
        cout << "B" << endl;
    }
};

struct C : B {
    int k = 0;
    ~C() {
        cout << "C" << endl;
    }
};

int main() {
    C* c = new C;
    B* b = dynamic_cast<B*>(c);
    delete b;
    // напечатает C B A - работает верно

}


//----------------------------------------------------------------------------
// Named Return Value Optimization (NRVO)
struct B {
    int i = 1;
    // int *j = new int(1);
    B() {
        cout << "constructor" << endl;
    }  
    B(const B& other) : i(other.i) {
        cout << "copy constructor" << endl;
    };
    B& operator=(const B& other) {
        i = other.i;
        cout << "operator=()" << endl;
        return *this;
    };
    
    B foo1() {
        /*
        PS D:\eduCpp\build\Debug> ."D:/eduCpp/build/Debug/edu.exe"
        constructor
        constructor
        copy constructor
        PS D:\eduCpp\build\Debug> 
        */
        B b;
        return b;
    }

    B foo2() {
        /*
        PS D:\eduCpp\build\Debug> ."D:/eduCpp/build/Debug/edu.exe"
        constructor
        constructor
        PS D:\eduCpp\build\Debug> 
        */
        return B();
    }
};


//----------------------------------------------------------------------------
int main() {
    B b1;
    B b2 = b1.foo1();
}

// Абстрактные классы, override, final
struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;
    void foo(int) {
        int i = 0;
        i++;
};
};

struct Rect : Shape {
    double area() const override {
        return 1.0;
    }
};


//----------------------------------------------------------------------------
// Шаблоны
template<typename T>
class C {
public:
    T var;
    C(T var) : var(var) {};
};

template<typename T>
void swap(T& x, T& y) {
    T b = x;
    x = y;
    y = b;
}

int main() {
    int i = 0;
    int j = 1;
    swap(i, j); // не явное инстанцирование шаблона
    C<int> C(1);    
}

// cпециализированные шаблоны
template<typename T>
void swap(T& x, T& y) {
    T b = x;
    x = y;
    y = b;
}

template<> // cпециализированный шаблон
void swap(int& x, int& y) {

}


//----------------------------------------------------------------------------
// typedef
typedef int int2;

// шаблонизированный typedef
template<typename T>
using vec = std::vector<T>;

template<class T>
class C {
    typedef T type; // засэйвить имя типа в О.В. класса
};

int main() {
    // C<int>::type x; ошибка компиляции, компилятор считает это названием поля, а не типа
}

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

// специализация 
template<class T>
void foo1(T v) {
    cout << v << endl;
}

template<>
void foo1(int v) {
    cout << v << endl;
}

//----------------------------------------------------------------------------
// remove_const, remove_reference  
template<typename T>
struct remove_const {
    T() {
        cout << "just_T" << endl;
    }
    typedef T type;
};

template<typename T>
struct remove_const<const T> {
    T() {
        cout << "const_T" << endl;
    }
    typedef T type;
};

//----------------------------------------------------------------------------
// правила шаблонного вывода
template<class T>
void f(T& x) {

}

int main() {
    int x; f(x);             // T = int, x: int&
    int& y = x; f(y);        // T = int, x: int&
    const int z = 0; f(z);   // T = const int, x: const int&
    const int& t = z; f(t);  // T = const int, x: const int&

    return 0; 
}

template<class T>
void f(T x) {

}

int main() {
    int x; f(x);             // T = int, x: int
    int& y = x; f(y);        // T = int, x: int
    const int z = 0; f(z);   // T = int, x: int
    const int& t = z; f(t);  // T = int, x: int

    return 0; 
}

//----------------------------------------------------------------------------
// non-type template parameters
template<class T, int n> // Является свойством типа
class array {

};

//----------------------------------------------------------------------------
// template template parameters
template<typename T>
class C {

};

template<typename X, template<typename> class T> 
void foo(const T<X> &t) {

}

int main() {
    foo<int, C>(C<int>());
}
//----------------------------------------------------------------------------
// значения по умолнчанию в шаблонах
template<typename T = int>
struct C {

};


//----------------------------------------------------------------------------
// Variadic templates (функция с переменным количество шаблонных аргументов)
template<typename ... Args> 
void foo(Args ... args) {

}

template<typename Head>
void foo(Head x) {
    cout << x << endl;
} 

template<typename Head, typename ... Tail> 
void foo(Head x, Tail ... t) {
    cout << x << " ";
    foo(t...);
}



//----------------------------------------------------------------------------
// функтуры
template<class T>
struct less {
    bool operator()(const T& x, const T& y) {
        return x < y;
    }
};

template<typename T, typename Cmp>
void sort(T* begin, T* end, Cmp cmp) {
    //...
    cmp(*x, *y);
    //...
}



//----------------------------------------------------------------------------
// CRTP
template<class T>
struct Base {
    void f() const {
        static_cast<const T*>(this)->f();
    }
};

struct Derived : Base<Derived> {
    void f() const {
        std::cout << "Derived" << std::endl;
    }
};


struct Derived1 : Base<Derived1> {
    void f() const {
        std::cout << "Derived1" << std::endl;
    }
};

int main() {
    Derived d;
    Base<Derived>* b = &d;

    Base<Derived1>* b1 = new Derived1();

    b1->f();

    b->f();
}



//----------------------------------------------------------------------------
// исключения 
int main() {
    /*
     *  Приведение типов в try-catch только между родителем и наследником, 
     *  и от любого указателя к void*
    */
    try {
        throw static_cast<int>(1);
    } catch (double d) {
        cout << "double" << endl;
    } catch (int x) {
        cout << "int" << endl;
    } catch (...) {

    }

    try {
        throw static_cast<int>(1);
    } catch (const int& i) {
        // обработка чего-то
        throw; // отправить исключение на уровень выше
    } 

}  
