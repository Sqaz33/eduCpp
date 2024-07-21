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
#define OCUPIED true
#define UNOCUPIED false
#define OcupiedStatus bool 

template<class T>
class GoofyAhhAllocator final {
public:
    typedef T type_name;

    GoofyAhhAllocator()  {
        ocupiedFlags = new bool[MAX_NUM_OBJ] {};
        pool = static_cast<T*>(
            ::operator new(MEMORY_2GB)
        );
        for (int i = 0; i < MAX_NUM_OBJ; i++) {
            ocupiedFlags[i] = false;
        }
    }

    //--------------------------------------------------------
    //--------------------------------------------------------
    //--------------------alocator func-----------------------
    //--------------------------------------------------------
    //--------------------------------------------------------
    T* allocate_one() noexcept {
        int i = getFirstUnoccupied();
        setOcupied(i, OCUPIED);
        lastOcupied = i;
        return pool + i;
    }

    void dealocate_one(T* ptr) noexcept {
        int ind = static_cast<int>(ptr - pool);
        setOcupied(ind, UNOCUPIED);
    }

    template<typename ... Args> 
    void* construct(T* ptr, Args... args) const noexcept(noexcept(T(args...))) {
        return new(ptr) T(args...);
    }

    void destroy(T* p) const noexcept(noexcept(p->~T())) {
        p->~T();
    }
    //--------------------------------------------------------
    //--------------------------------------------------------
    //--------------------------------------------------------
    //--------------------------------------------------------
    //--------------------------------------------------------

    size_t maxNumObj() const noexcept {
        return MAX_NUM_OBJ;
    }

private:
    bool* ocupiedFlags;
    T* pool;

    const size_t MEMORY_2GB = 2147483648 + (2147483648 % sizeof(T));
    const size_t MAX_NUM_OBJ = MEMORY_2GB / sizeof(T);
    
    int getFirstUnoccupied() const noexcept {
        for (int i = 0; i < MAX_NUM_OBJ; i++) {
            if (getOcupied(i)) {
                return i;
            }
        }
        return -1;
    }

    //--------------------------------------------------------------
    inline void setOcupied(int ind, OcupiedStatus status) noexcept {
        ocupiedFlags[ind] = status;
    }

    inline OcupiedStatus getOcupied(int ind) const noexcept {
        return ocupiedFlags[ind];
    }
    //--------------------------------------------------------------

    GoofyAhhAllocator& operator=(const GoofyAhhAllocator& other) = delete;
    GoofyAhhAllocator(const GoofyAhhAllocator& other) = delete;
}; 
//----------------------------------------------------------------------------

// std::vector
#include <exception>
#include <memory>
#include <iostream>

template<class T, class Alloc = std::allocator<T>>
class vectorCppLess11 {
public:
    vectorCppLess11() 
    :
    m_size(0),
    m_capacity(16)
    {
        arr = traits::allocate(alloc, m_capacity);
    }

    vectorCppLess11(const vectorCppLess11<T, Alloc>& v) 
    :
    m_size(v.m_size),
    m_capacity(v.m_capacity),
    alloc(
        traits::select_on_container_copy_consruciont(v.alloc) // возвращает, нужный аллокатор (новый или тот же из старого вектора)
    )
    {
        arr = traits::alloacate(alloc, m_capacity);
        for (int i = 0; i < m_size; i++) {
            traits::construct(alloc, arr+i, v.arr[i]);
        }
    }

    void push_back(const T& x) {
        if (m_size >= m_capacity) {
            reserve(m_capacity << 1);
        }
        traits::construct(alloc, arr+m_size, x);
        m_size++;
    }

    size_t size() {
        return m_size;
    }  

    size_t capacity() {
        return m_capacity;
    }

    void shrink_to_fit() {
        // сокращает capacity до size    
    }

    void resize(size_t size, T value = T()) {
        // меняет размер
        // новые элементы инициализируются значением
    }

    void swap(const vectorCppLess11<T, Alloc> other) {
        // меняет местами вектора
    }

    T& operator[](size_t i) {
        return arr[i];
    }

    const T& operator[](size_t i) const {
        return arr[i];
    }
    
    T& at(size_t ind) {
        if (m_size <= ind) {
            throw std::out_of_range;
        }
        return arr[ind];
    }

    const T& at(size_t ind) const {
        if (m_size <= ind) {
            throw std::out_of_range;
        }
        return arr[ind];
    }

    template<class ... Args>
    void emplace_back(const Args& ... args) {
        push_back(T(args...));
    }

private:
    size_t m_size;
    size_t m_capacity;
    T* arr;
    Alloc alloc;

    using traits = std::allocator_traits<Alloc>;

    void reserve(size_t newCapacity) {
        T* newArr = traits::allocate(alloc, newCapacity);
        for (int i = 0; i < m_size; i++) {
            traits::construct(alloc, newArr+i, arr[i]);
            traits::destroy(alloc, arr+i);
        }
        traits::deallocate(alloc, arr, m_capacity);
        arr = newArr;
        m_capacity = newCapacity;
    }
};


//------------------------------------------------------
template<class T>
struct Node {
public:
    Node(const T& val) : m_val(val) {}

    inline T& val() {
        return m_val;
    } 

    inline void setVal(const T& val) {
        this->m_val = val;
    }

    inline Node<T>* prev() {
        return m_prev;
    }

    inline void setPrev(Node<T>* prev) {
        m_prev = prev;
    }

    inline Node<T>* next() {
        return m_next;
    }

    inline void setNext(Node<T>* next) {
        m_next = next;
    }

private:
    T m_val;
    Node<T>* m_prev = nullptr;
    Node<T>* m_next = nullptr;
};

template<class T, class Alloc = std::allocator<Node<T>>>
class List {
public:
    using type_name = T;

    void append(const T& val) {
        Node<T>* prev;
        if (m_size == 0) {
            start = traits::allocate(alloc, 1);
            end = start;
            prev = nullptr;
        } else {
            end->setNext(traits::allocate(alloc, 1));
            prev = end;
            end = end->next();
        }
        traits::construct(alloc, end, val);
        end->setPrev(prev);

        m_size++;
    } 

    T& at(int i) {
        if (m_size <= i) {
            throw std::out_of_range("List::at() index out of range");
        }
        Node<T>* cur = start;
        for (int j = 0; j < i; j++) {
            cur = cur->next();
        }
        return cur->val();
    }

    size_t size() const {
        return m_size;
    }


    void remove_at_end() {
        if (m_size == 0) {
            throw std::out_of_range("List::remove_at_end empty list");
        }
        Node<T>* prev = end->prev();
        traits::destroy(alloc, end);
        traits::deallocate(alloc, end, 1);
        end = prev;
        m_size--;
        if (m_size == 0) {
            start = end;
        } else {
            end->setNext(nullptr);
        }
    } 



private:
    using traits = std::allocator_traits<Alloc>;
    Node<T>* start = nullptr;
    Node<T>* end = nullptr;
    Alloc alloc;
    size_t m_size = 0;
};


int main() {
    List<int> list;
    for (int i = 0; i < 10000000; i++) {
        list.append(i);
    }
    for (int i = 0; i < 10000000; i++) {
        list.remove_at_end();
    }

    cout << list.size();
    

}

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
template<
    class Key, 
    class T, 
    class Comp = std::less<Key>,
    class Alloc = std::allocator<std::pair<const Key, T>>
>
class MyMap {
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using allocator_type = Alloc;
    using reference = value_type&;

    T& operator[](const Key& key) {
        Node* g = m_get(root, key);
        if (g == nullptr) {
            root = m_put(root, key, T());
            g = m_get(root, key);
        }
        return g->val;
    }

    size_t size() const {
        return m_size(root);
    }

    void del(const Key& key) {
        root = m_delete(root, key);
    }

    bool contain(const Key& key) const {
        return m_contain(root, key);
    }

private:

    //----------------------------------------
    struct Node {
        Node(const Key& key, const T& val) 
        : 
        vt(key, val),
        key(vt.first),
        val(vt.second) 
        {}

        value_type vt;
        const Key& key;
        T& val;
        Node* prev = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
    }* root = nullptr;
    //----------------------------------------

    using NodeAlloc = typename Alloc::template rebind<Node>::other;
    using traits = std::allocator_traits<NodeAlloc>;
    
    Comp comp;
    NodeAlloc nodeAlloc;

    bool m_contain(const Node* x, const Key& key) const {
        if (x == nullptr) {
            return false;
        }
        if (comp(key, x->key)) {
            return m_contain(x->left, key);
        } else if (comp(x->key, key)) {
            return m_contain(x->right, key);
        } else {
            return true;
        }
    }

    Node* m_put(Node* x, const Key& key, const T& val) { 
        if (x == nullptr) {
            Node* n = traits::allocate(nodeAlloc, 1);
            traits::construct(nodeAlloc, n, key, val);
            return n;
        } else if (comp(key, x->key)) {
            x->left = m_put(x->left, key, val);
        } else if (comp(x->key, key)) {
            x->right = m_put(x->right, key, val);
        } else {
            x->val = val;
        }
        return x;
    }

    Node* m_get(Node* x, const Key& key) {
        if (x == nullptr) {
            return nullptr;
        }
        if (comp(key, x->key)) {
            return m_get(x->left, key);
        } else if (comp(x->key, key)) {
            return m_get(x->right, key);
        } else {
            return x;
        }
    }

    Node* m_min(Node* x) {
        if (x == nullptr) {
            return nullptr;
        }
        if (x->left != nullptr) {
            return m_min(x->left);
        }
        return x;
    }

    Node* m_max(Node* x) {
        if (x == nullptr) {
            return nullptr;
        }
        if (x->right != nullptr) {
            return m_max(x->rigth);
        }
        return x;
    }

    size_t m_size(const Node* x) const {
        if (x == nullptr) {
            return 0;
        }
        return m_size(x->left) + m_size(x->right) + 1;
    }

    Node* m_deleteMin(Node* x) {
        if (x->left == nullptr) {
            return x->right;
        }
        x->left = m_deleteMin(x->left);
        return x;
    }

    Node* m_delete(Node* x, const Key& key) {
        if (x == nullptr) {
            return nullptr;
        }
        if (comp(key, x->key)) {
            x->left = m_delete(x->left, key);
        } else if (comp(x->key, key)) {
            x->right = m_delete(x->right, key);
        } else {
            if (x->right == nullptr) {
                Node* left = x->left;
                traits::destroy(nodeAlloc, x);
                traits::deallocate(nodeAlloc, x, 1);
                return left;
            } 
            if (x->left == nullptr) {
                Node* right = x->right;
                traits::destroy(nodeAlloc, x);
                traits::deallocate(nodeAlloc, x, 1);
                return right;
            }
            Node* tmp = x;
            x = m_min(tmp->right);
            x->right = m_deleteMin(tmp->right);
            x->left = tmp->left;
            traits::destroy(nodeAlloc, tmp);
            traits::deallocate(nodeAlloc, tmp, 1);
        }
        return x;
    }
};

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
// моменты из собеза.
struct A {
    A() {
       bar(); 
       foo(); // A::foo()
    }

    void bar() {
        A::foo(); 
        // foo() -> B::foo()
        // при вызове bar()->B::foo() в конструкторе - ub. 
    }

    virtual void foo() = 0; 
};  

void A::foo() {
    cout << "Hi" << endl;
};

struct B : A {
    void foo() {

    }
};
