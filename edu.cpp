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


template<typename T>
struct remove_reference {
    using type = T;
}; 

template<typename T>
struct remove_reference<T&> {
    using type = T;
};

template<typename T>
struct remove_reference<T&&> {
    using type = T;
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
// CRTP (Иметация полиморфизма)
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

// копирование объекта исключения 
struct S {
    int x;

    S() : x(0) {
        cout << "constr" << endl;
    }

    S(const S& other) {
        x = other.x;
        cout << "copy" << endl;
    }
};

void foo() {
    static S s;
    throw s;
}


int main() {

    try {
        foo();
    } catch (const S& s) {

    } 

    // constr
    // copy
}  

// срезка при копировании
struct Base {

};

struct Derived : Base {

};

int main() {
    try {  
        Derived d;
        Base b& = d;
        throw b; // создастся копия части Base и d
    } catch (Base& b) {

    }
}  


// throw без параметра
int main() {
    try {
        throw Derived();
    } catch (Base &b) {
        throw b; // копия Base;
        throw; // без копии Base;
    }
}  


//----------------------------------------------------------------------------
// noexcept 
void foo() noexcept { // говорит о том, что функция не бросает исключения

}

int main() {
    noexcept(foo()); // true, если функция noexcept
}

void foo() noexcept(true) { // условный noexcept

}


//----------------------------------------------------------------------------
// Исключение в конструкторе
struct U {
    int* p;
    U() {
        p = new int;
        throw 1;
        // деструктор не вызывается - утечка памяти
    }

    ~U() {
        delete p;
    }
};


int main() {
    try {
        U u;
    } catch (...) {
         
    }
}  

// паттерн такой утечки
void f() {
    p = new ...
    throw ...;
    delete p;
}


//----------------------------------------------------------------------------
// Исключение в конструкторе
struct U {
    int* p;
    U() {
        p = new int;
    }

    ~U() {
        delete p;
        throw 1; // может быть ошибка
        //---------------------
        delete p;
        if (std::uncaught_exceptions()) { // решение проблеммы 
            throw 1; 
        }
    }
};

void foo() {
    U u;
    throw 1;     // runtime error
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// аллокаторы
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// new expression 
int* n = new int(1);
int* arr = new int[100] {};

// operator new
void* n2 = ::operator new(10);
void* arr2 = ::operator new[](10);
int* n3 = static_cast<int*>(::operator new(sizeof(int)));


struct S {
    int i;
    S(int i) : i() {};
};


int main() {
    void* ptr = operator new(sizeof(S));
    //placement new
    S* S_ptr = new(ptr) S(1);

    //new
    S* S_ptr2 = new S(1);
}


//----------------------------------------------------------------------------
// перегрузки operator new

// перегрузка operator new
void* operator new(size_t sz) {
    cout << "operator new" << endl;
    return malloc(sz);
}

struct S {
    int i;
    S(int i) : i() {
        cout << "S constructor" << endl;
    };

    void* operator new(size_t sz, void* ptr) {
        cout << "placement new" << endl;
        return ptr;
    }
};

// Если происходит вызов перегруженного operator new, то должен вызываться перегруженный operator delete.
struct S {
    int i;
    S(int i) : i(i) {
        cout << "S constructor" << endl;
    };

    void* operator new(size_t sz, void* p) {
        cout << "S::operator placement new" << endl;
        return p;
    }

    void* operator new(size_t sz,int tst) {
        tst++; // do something with tst
        cout << "S::operator new" << endl;
        return malloc(sz);
    }

    void operator delete(void* ptr, int tst) {
        tst--;
        cout << "S::operator delete" << endl;
        free(ptr);
    }

};



int main() {
    S* sPtr = static_cast<S*>(S::operator new(sizeof(S), 1));
    sPtr = new(sPtr) S(1);
    S& s = *sPtr;
    cout << s.i << endl;
    S::operator delete(sPtr, 1);
    cout << s.i << endl;
}

//----------------------------------------------------------------------------
// nothrow new
#include <new>

void* operator new(size_t sz, const std::nothrow_t& x) noexcept {
    return malloc(sz);
}

int main() {
    void* ptr = ::operator new(sizeof(int), std::nothrow);
}


//----------------------------------------------------------------------------
// new_handler
#include <new>

void foo() {
    std::cerr << "Не удалось выделить память! Попытка очистить ресурсы и повторить...\n";
    // Попытка освободить память или выполнить другие действия по очистке
    // Например:
    // some_cleanup_function();

    // Если мы не можем освободить достаточно памяти, нужно завершить программу
    std::terminate();
}


int main() {
    SetConsoleOutputCP(CP_UTF8); 
    std::set_new_handler(foo);
    long long *arr = new long long[1000000000000];
}

//----------------------------------------------------------------------------
// std::allocator
template<class T>
struct my_allocator {
    typedef T value_type;
    T* allocate(size_t count) const {
        return static_cast<T*>(::operator new[](sizeof(T) * count));
    }

    void deallocate(T* p, size_t count) const {
        delete[] p;
    }

    template<typename ... Args>
    void* construct(T* p, Args& ... args) const noexcept(noexcept(T(args ...))) {
        return new(p) T(args ...);
    }

    void destroy(T* p) const noexcept(noexcept(~T())) {
        p->~T();
    }
};


int main() {
    my_allocator<int> alloc;
    int* nw_int = alloc.allocate(100);
    int i = 1;
    alloc.construct(nw_int, i);

    for (int i = 0; i < 100; i++) {
        cout << nw_int[i] << endl;
    }
}

//----------------------------------------------------------------------------
// std::allocator_traits
template<class Alloc>
struct std_allocator_traits {
    static Alloc::value_type* allocate(/*не const ссылка*/ Alloc& a, size_t n) {
        return a.allocate(n);
    }
};

//----------------------------------------------------------------------------
// Итераторы

int main() {
    std::vector<int> v = {1, 2, 3, 4};

    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << endl;
    }

}
template<class T>
class MyContainer {
public:
    using valu_type = T;

    MyContainer() = default;

    MyContainer(std::initializer_list<T> list) : v(list) {};

    size_t size() const {
        return v.size();
    }

    T& operator[](size_t ind) {
        return v[ind];
    }
    void push_back(const T& x) {
        v.push_back(x);
    }

    const T& operator[](size_t ind) const {
        return v[ind];
    }


    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(T* v): it(v) {}
        
        iterator& operator++() {
            ++it;
            return *this;
        }

        iterator& operator--() {
            --it;
            return *this;
        }

        iterator& operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        reference operator*() const {
            return *it;
        }

        pointer operator->() const {
            return it;
        }

        friend bool operator==(const iterator& a, const iterator& b)  {
            return a.it == b.it;
        }

        friend bool operator!=(const iterator& a, const iterator& b) {
            return !(a.it == b.it);
        }
        
    private:
        T* it;
    };

    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator(const T* v) : it(v) {};
        
        const_iterator& operator++() {
            ++it;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        const_iterator& operator--() {
            --it;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        pointer operator->() const {
            return it;
        }

        const reference operator*() const {
            return *it;
        }

        friend bool operator==(const const_iterator& a, const const_iterator& b)  {
            return a.it == b.it;
        }

        friend bool operator!=(const const_iterator& a, const const_iterator& b) {
            return !(a.it == b.it);
        }
    private:
        const T* it;
    };


    using iterator_trais = std::iterator_traits<MyContainer<T>::iterator>;

    iterator begin() {
        return iterator(v.data());
    }

    iterator end() {
        return iterator(v.data() + v.size());
    }
    
    using reverse_iterator = std::reverse_iterator<MyContainer<T>::iterator>;

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    

    const_iterator begin() const {
        return const_iterator(v.data());
    }

    const_iterator end() const {
        return const_iterator(v.data() + v.size());
    }

    const_iterator cbegin() const {
        return const_iterator(v.data());
    }

    const_iterator cend() const {
        return const_iterator(v.data() + v.size());
    }

    using const_reverse_iterator = std::reverse_iterator<MyContainer<T>::const_iterator>;

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }


private:
    std::vector<T> v;
};


int main() {
    const MyContainer<int> mc = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    
    for (const auto& n : mc) {
        cout << n << endl;
    }

}  

//----------------------------------------------------------------------------
// iterator_traits
template<class Iter>
void process_sequence(Iter begin, Iter end) {
    using T = typename std::iterator_traits<Iter>::value_type;
    T val = *begin;
}


//----------------------------------------------------------------------------
// insert iterators
#include <iterator>
int main() {

    // back_insert_iterator
    // вызывается push_back
    std::vector<int> v = {1, 2, 5, 6};
    std::vector<int> v2;
    std::back_insert_iterator<std::vector<int>> vIt(v2);

    // копирует в конец v2
    std::copy(v.begin(), v.end(), vIt);

    
    // template<class Container>
    // std::back_insert_iterator<Container> back_inserter(Container& c)
    // {
    //     return std::back_insert_iterator<Container>(c);
    // }
    std::vector<int> v3;
    std::copy(v.begin(), v.end(), std::back_inserter(v3));

    std::vector<std::pair<int, char>> ic = {{1, '1'}, {2, '2'}, {3, '3'}};

    std::map<int, char> mp;

    std::insert_iterator<std::map<int, char>> it(mp, mp.end());

    std::copy(ic.begin(), ic.end(), it);


    for (const auto& p : mp) {
        cout << p.second << endl;
    }
} 

//----------------------------------------------------------------------------
// stream iterators
int main() {
    std::locale::global(std::locale(""));
    std::vector<char> in;
    std::istream_iterator<char> start(std::cin), stop;
    std::ostream_iterator<char> out(std::cout);
    
    std::copy(start, stop, std::back_inserter(in));

    std::copy(in.begin(), in.end(), out);

    std::ifstream inputFile("t.txt"); 

    std::istream_iterator<char> beginFile(inputFile), endFile;

    for (auto& c = beginFile; c != endFile; ++c) {
        cout << *c;
    }
    inputFile.close();
}  

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-------------------------------move семантика-------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
template<class T>
T&& move(T& v) {
    return static_cast<T&&>(v);
}

struct S {
    std::vector<int> v;
    int i;
    int* arr;

    S() = default;
    S(const S&) = default;
    S& operator=(const S& other) = default;


    // перемещающий конструктор 
    S(S&& other): 
        v(std::move(other.v)),
        i(other.i),
        arr(other.arr)
    {
        other.arr = nullptr;
    }

    // перемещающий оператор присваивания
    S& operator=(S&& other) {
        v = std::move(other.v);
        arr = other.arr;
        other.arr = nullptr;
        i = other.i;
    }
}; 

int main() {
    S s;
    S s2(s);
}  


//----------------------------------------------------------------------------
struct S {
    int i = 1; 
};

void foo(S& s) {
    cout << "lvalue" << endl;
} 
void foo(S&& s) {
    cout << "rvalue" << endl;
}

int main() {
    S s;
    foo(false ? S() : s); // каст к rvalue
    foo(true ? S() : s);  // каст к rvalue

    /*
        PS D:\eduCpp\build\Debug> ."D:/eduCpp/build/Debug/eduCpp.exe"
        rvalue
        rvalue
    */
}  



//----------------------------------------------------------------------------
struct S {
    int i = 1; 
    S() {
        cout << "S()" << endl;
    };
    S(const S& s) : i(s.i) {
        cout << "S(const S& s)" << endl;
    } 
    S(S&& s) : i(s.i){  
        cout << "S(S&& s)" << endl;
    }
};

S foo() {
    S s;
    return s;
}

int main() {
    S s = foo(); // move construct 
}  


//----------------------------------------------------------------------------
//rvalue ref. propertes
int main() {
    int x = 5;
    // int&& y = x;  запрещенно не явное преобразрование lvalue в rvalue
    int&& y = std::move(x);

    // int&& z = y; запрещено

    int& z = y; // rvalue-ref to lvalue-ref

    const int& t =  std::move(y); 

    const int&& u = std::move(t);

    int&& t1 = 5;
    t1++;

    cout << t1 << endl;

}  

//----------------------------------------------------------------------------
// реализация std::move

template<typename T>
void func(T&& arg) {
    // T&& - universal reference 
}

auto&& z = r; // universal ref

template<typename T>
std::remove_reference_t<T>&& move(T&& x) {
    return static_cast<std::remove_reference_t<T>&&>(x);
}


//----------------------------------------------------------------------------
// perfect forward #include <type_traits>
// переменные имеют имена, и все именованные переменные являются lvalue.

template<typename T>
T&& forward(std::remove_reference_t<T>& x) {
    // x - всегда lvalue
    return static_cast<T&&>(x); 
}

template<class T, class... Args> 
// в зависимости от типа args это будет коставаться либо к && либо к &
void construct(T* p, Args&& ... args) { //allocator::
	new(p) T(std::forward<Args>(args)...);
}

//----------------------------------------------------------------------------
// copy elision
struct S {
    int* arr = new int[10];
    S() {
        cout << "S()" << endl;
    }

    S(const S&) {
        cout << "S(const S&)" << endl;
    }

    S(S&&) {
        cout << "S(S&&)" << endl;
    }

    S& operator=(S&&) {
        cout << "S& operator=(S&&) " << endl;
        return *this;
    }

    S& operator=(const S&) {
        cout << "S& operator=(const S&) " << endl;
        return *this;
    }

    const S operator+(const S& other) {
        S s;
        memcpy(s.arr, arr, 10);
        for (int i = 0; i < 10; i++) {
            s.arr[i] += other.arr[i];
        }
        return s;
    }
};


// unamed return value optimization
S urvo() {
    return S();
}

// named return value optimization
S nrvo() {
    S s;
    return s;
}

int main() {
    S s = S(); 
    S s2 = urvo(); 
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-------------------------------inline---------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/*
    Единица трансляции (translation unit) в C++ — это минимальная
    единица кода, которая компилируется компилятором в объектный файл.
    Она представляет собой исходный файл вместе со всеми заголовочными
    файлами, которые подключены в нем через директивы #include.


    Директива #include — это команда препроцессору, которая указывает
    включить содержимое указанного заголовочного файла на место директивы
    в исходном файле. В результате исходный файл получает доступ к объявлениям
    и типам из заголовочного файла.

    Встроенная функция или встроенная переменная (начиная с C++17) имеет
    следующие свойства:

    Определение встроенной функции или переменной (начиная с C++17) должно быть
    доступно в той единице трансляции, где к ней осуществляется доступ (не
    обязательно до точки доступа). Встроенная функция или переменная (начиная с
    C++17) с внешним связыванием (например, не объявленная как static) меет
    следующие дополнительные свойства: В программе может быть более одного
    определения встроенной функции или переменной (начиная с C++17), если каждое
    определение появляется в другой единице трансляции и (для нестатических
    встроенных функций и переменных (начиная с C++17)) все определения идентичны.
    Например, встроенная функция или встроенная переменная (начиная с C++17) может
    быть определена в заголовочном файле, включенном в несколько исходных файлов.
    Она должна быть объявлена как inline в каждой единице трансляции.
    Она имеет один и тот же адрес в каждой единице трансляции.
*/

//inline namespace
namespace n1 {
    inline namespace n2 {
        using size = unsigned int;
    }
}


int main() {
    n1::n2::size i;
    n1::size i;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-------------------------------auto-----------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
auto f(int x) {
    if (x > 0) {
        return 0;
    }
    return 1;
    // для вывода auto oreturn должен иметь 1 тип    
}


auto f(auto  x) { // c++ 20

}

int main() {
    int x = 1;
    int& y = x;

    auto r = y; // auto = int, правила, такие же, как и при передачи в шаблонную функцию

    auto&& z = r; // universal ref
    z = 2;
    cout << x << endl;


    std::vector<bool> v;
    auto b = v[0]; // vb-ref, лучше так не делать

    cout << typeid(b).name() << endl;
}  


//----------------------------------------------------------------------------
//decltype
int main() {
    int x = 5;
    decltype(x) y = 5;

    int& r = x;
    decltype(r) z = x;
    z++;
    cout << z << endl;

    decltype(++x) /*выражение не вычисляется*/ b = x;
    // навесит & если выражение lvalue
    b = 10;

    cout << b << endl;

    // decltype((S().m)) v = x;
    // не скомпилируется слева int&&
    decltype((S().m)) v = std::move(x);


}  

//----------------------------------------------------------------------------
//decltype(auto)
template<typename Container>
decltype(auto) getByIndex(Container& container, size_t index) {
    std::cout << "out\n";
    return container[index]; 
}

//----------------------------------------------------------------------------
//class templates argument deduction
int main() { 
    std::vector v = {1, 2, 3}; // c++17
}

int main() { 
    std::vector v = {1, 2, 3}; // c++17

    //конструктор с init. list будет для компилятора предпочтительнее
    std::vector vv = {v.begin(), v.end()}; // вектор из 2х итераторов

}

//----------------------------------------------------------------------------
//user-defined deduction rules
template<class T>
struct S {
    S(T t) {}

    template<class Iter>
    S(Iter beg, Iter end) :
        v(beg, end)
    {}

    using type = T;

    void printType() {
        std::cout << typeid(type).name();
    }

    std::vector<T> v;
};

template<class Iter> 
S(Iter beg, Iter end) -> S<typename std::iter_value_t<Iter>>;

int main() { 
    std::vector v = {1, 2, 3};

    S s(v.begin(), v.end());

    s.printType();

}

//----------------------------------------------------------------------------
//user defined deduction rules
int main() { 
    std::map<std::string, int> m = {
        {"abc", 1},
        {"asdf", 2}
    };

    for (const auto& [key, value] : m) {
        std::cout << key << ' ' << value << '\n';
    }

    //array 
    int arr[3] = {1, 2, 3};
    auto [x, y, z] = arr;
}



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-------------------------------умные укатели--------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//unique_ptr указатель с единоличным владением
template<typename T>
class UniquePtr {
public: 
    UniquePtr(T* ptr) : ptr(ptr) {}

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    } 

    auto& operator=(UniquePtr&& other) noexcept {
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    ~UniquePtr() {
        delete ptr;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

private:
    T* ptr;
};


int main() { 
    std::vector<UniquePtr<int>> v;

    UniquePtr up = new int(5);

    v.push_back(std::move(up));
}

struct S {
    S(int i) : i(i) {}
    int i;
};

int main() { 
    std::unique_ptr<S> up = std::make_unique<S>(1);
    std::cout << up->i << '\n';
}


//----------------------------------------------------------------------------
//shared_ptr указатель с разделяемым владением

template <class T>
class MVPSharedPtr;

template <typename T, typename ... Args> 
MVPSharedPtr<T> makeShared(Args&& ... args);

template <class T>
class MVPSharedPtr {
public:
    MVPSharedPtr(T* ptr) : 
        count(new size_t(0))
    {
        this->ptr = new ControlBlock();
        this->ptr->ptr = ptr;
    }

    MVPSharedPtr(const MVPSharedPtr& other) :
        ptr(other.ptr),
        count(other.count)
    {       
        if (count) {
            ++*count;
        } else {
            ++*(ptr->count);
        }
    }   

    MVPSharedPtr(MVPSharedPtr&& other) noexcept :
        ptr(other.ptr),
        count(other.count)
    {
        other.ptr = nullptr;
        other.count = nullptr;
    }

    ~MVPSharedPtr() {
        int c = count ? --*count : --*(ptr->count);
        
        if (c == 0) {
            ::operator delete(ptr);
            delete (count);
        }
    }

    T& operator*() {
        return *(ptr->ptr);
    }

    T* operator->() {
        return ptr->ptr;
    }

   template<typename T, typename ... Args>
   friend MVPSharedPtr<T> makeShared(Args&& ... args);
private:
    struct ControlBlock {
        T* ptr;
        size_t* count;
        ~ControlBlock() {
            ::operator delete(static_cast<void*>(ptr));
        }
    }* ptr;

    size_t* count;


    MVPSharedPtr(ControlBlock* control) : 
        ptr(control), 
        count(nullptr)
    {}
};

template<typename T, typename ... Args>
MVPSharedPtr<T> makeShared(Args&& ... args) {
    void* ptr = ::operator new(sizeof(T) + sizeof(size_t));
    new(static_cast<T*>(ptr)) T(std::forward<Args>(args)...);
    new(static_cast<void*>(static_cast<T*>(ptr) + 1)) size_t(1);

    auto* block = new MVPSharedPtr<T>::ControlBlock();
    block->ptr = static_cast<T*>(ptr);
    block->count = reinterpret_cast<size_t*>(static_cast<T*>(ptr) + 1);
    return MVPSharedPtr<T>(block);
}

template<typename T, typename ... Args>
MVPSharedPtr(T, Args...) -> MVPSharedPtr<T>;

int main() { 
    auto sp = makeShared<int>(1);
    std::cout << *sp << std::endl;

    auto sp2(sp);

    std::cout << *sp2 << std::endl;
}




























































//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-------------------------------pure virtual call----------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/*
    Шаг 1. Сконструировать базовую часть верхнего уровня:
        а) Установить указатель __vtbl на vtbl родительского класса;
        б) Сконструировать переменные экземпляра базового класса;
        в) Выполнить тело конструктора базового класса.

    Шаг 2. Наследуемая часть(-и) (рекурсивно):
        а) Поменять указатель __vtbl на vtbl класса-потомка;
        б) Сконструировать переменные класса-потомка;
        в) Выполнить тело конструктора класса-потомка.

    В случае с абстрактным классом и чистой виртуальной функцией, указатель всё равно есть, 
    но на стандартный обработчик __pure_virtual_func_called()
*/

struct Base {

    Base() {
        f();
    }

    void f() {
        Base::foo(); 
    }
 
    virtual void foo() = 0; 
};

void Base::foo() { 
    cout << "Base::foo()" << endl;
} 

struct Derived : Base {
    void foo() override { 
        cout << "Derived::foo()" << endl; 
    }
};
