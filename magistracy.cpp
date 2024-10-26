//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//---------------------------------convertion---------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int foo(int) {return 0;}
int main() {
    int a = 5, b; b = a + 2;  // lvalue to rvalue

    int a1[10], *b1; b1 = a1; // array to pointer

    int (b*)(int);
    b = foo; // function to pointer
}

// побочный эффект (side efect) - то, что не может соптимизировать компилятор
// компилятор при оптимизации обязан сохранять порядок побочных эффектов



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//---------------------------------string_view--------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int main() {
    std::string str = "   trim me   ";
    std::string_view strv = str;

    auto trimfst = strv.find_first_not_of(" ");
    strv.remove_prefix(std::min(trimfst, strv.size()));

    auto trimlst = strv.find_last_not_of(" ");
    strv.remove_suffix(strv.size() - std::min(trimlst, strv.size()));

    std::cout << strv << '\n';

    std::string_view testv = "0123456789";
    testv.remove_prefix(1); // 1...9
    testv.remove_suffix(1); // 1...8
    std::cout << testv << '\n';
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//--------------------------small string optimization-------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// идея
class string {
    size_type size_;
    union {
        struct {
            char* data_
            size_type capacity_,
        } large_,
        char small_[sizeof(large_)]
    };  
};


template <class CharT> 
class basic_string {
    CharT* data;
    size_type capacity;
    union {
        enum {SZ = (sizeof(data)  + sizeof(size_type) * 2 + 31) / 32},
        CharT small_[SZ]; 
    } sso;
public:
    // ...
};


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//--------------------------------char_traits---------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Traits - харрактеристика типа.
// должен знать как: сравнивать, мувать, копировать.




//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//--------------------------------шаблоны функции-----------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// полиморфная функция - функция, ведущая себя по разному в зависимости от типов
// аргументов

// обобщение функции при помощи traits
namespace t {
    template <class T>
    struct default_id_trait {
        static constexpr auto id() {
            return T(1);
        };
    };

    template <>
    struct default_id_trait<int> {
        static constexpr auto id() {
            return 1;
        };
    };
} // namespace t

template <class T, class Trait = t::default_id_trait<T>>
T nth_power(T x, uint32_t n) {
    T acc = Trait::id();
    if ((x == acc) || (n == 1)) return x;
    while (n > 0) {
        if ((n & 0x1) == 0x1) { acc *= x; n -= 1; } 
        else { x *= x; n /= 2; }
    }
    return acc;
}

int main() {   
    int p = nth_power(10, 5);
    std::cout << p << '\n';
}

//----------------------------------------------------------------------------
// инстанцирование - пораждение экземпляра специализации 
// написание специализации  - ручное инстанцирование 

// Правила игры
// * Явно инстанцирование единожды в программе.
// * Явная специализация единожды в программе.
// * Явное инстанцирование должно следовать за явной специализацией.

template <class T> T max(T x, T y) { .... }
template <> int max<int>(int x, int y) { .... }
template int max<int>(int, int); // вынудили инстанцировать

// иначе IFNDR (ill formed no dignostic required)

// запрет специализации 
template <> void foo<char>(char*) = delete;

//----------------------------------------------------------------------------
// Non-type параметры - структрурные типы

/* 
Структурные типы: 
    * Скалярные типы.
    * Левые ссылки.
    * Структуры/классы и их базовые структруры/классы с public и не mutable полями.
      При этом все поля и поля базовых классов/структур тоже структурные типы или массивы.
*/

struct Pair {int x, y;};

template <int N, int* PN, int& RN, Pair P> int foo(); // все compili-time

// специализация по non-type 
template <class T, int N> void foo(T(&Arr)[N]);

template <> foo<int, 3>(int(&Arr)[3]) { .... }; 



//----------------------------------------------------------------------------
// Вывод типов

template <class T>
void foo(T v) { .... } // режутся ссылки и внешнии cv-квалификаторы

template <class T>
void foo(const T& v) { .... } // "уточнение" - ничего не режится

// Частичный вывод типов
template <class DstT, class SrcT>
DstT implicit_cast(const SrcT& x) {
    return x;
}

double val = implicit_cast<double>(-1); 


// параметры по умолчянию в шаблонных функциях 
template <class T = int> void foo(T x = 1) {};


// auto для non-type параметров

struct S {
    int i = 5;
};

std::ostream& operator<<(std::ostream& out, const S& s) {
    return out << s.i;
}

template <auto n>
void foo() {
    std::cout << n << '\n';
}

int main() {   
    foo<5>();
    foo<1>();

    constexpr S s;
    foo<s>();
}


// специализирующий тип
template <class T> 
T foo(T x) { .... }

template <>
int foo(int x) { .... }


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------Правила перегрузки------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/*
 * Выбирается множество перегруженных имен.
 *
 * Выбирается множесвто кандидатов.
 * 
 * Из множества кандидатов выбираются жизнеспособные (viable) кандидаты для данной перегрузки.
 * 
 * Лучший из жизнеспособных кандидатов выбирается на основании цепочек неявных преобразований для 
   каждого параметра.
 *
 * Если лучший кандидат существует и является единственным, то перегрузка разрешена успешно,
   иначе программ ill-formed.
*/

//--------------------------------------
// Поиск Кёнига
// Компилятор ищет имя функции из текущего и всех охватывающих скопах.
// Если оно не найдено, компилятор ищет имя функции в скопах ее аргументов.

namespace N { struct A; int f(A*); }

int g(N::A* a) { int i = f(a); return i; }

//--------------------------------------
// цепочки преобразований 

struct S { S(int) {} };

void foo(int) {}
void foo(S) {}
void foo(...) {}


// пользовательские преобразования
struct S{ S(long) {} };
void foo(S) {}
int x = 42;
foo(x); // int -> long -> S


struct T { T(int) {} };
struct S { S(T) {} };
void foo(S) {}
int x = 42;
foo(x); // int -> T -> S // не верно

// в цепочке преобразований может быть только 1 пользовательское


//--------------------------------------
// Стандартные преобразования:
// Трансформация объектов (ранг точного совпадения).
int arr[10]; int* p = arr; 
// Коррекции квалификаторов (ранг точного совпадения).
int x; const int* px = &x;
// Продвижения (ранг продвижения).
int res = true;
// Конверсии (ранг конверсии).
float f = 1;


//--------------------------------------
// Частичный порядок шаблонов функций.
template <class T> void f(T); // (1)
template <class T> void f(T*); // (2)

int*** a;   
f(a); // -> 2;

// Из всех перегрузок шаблонной функции выбирается наиболее специализированная. 

//--------------------------------------
// Специализация при перегрузках
template <class T> void f(T); // (1)

template <> void f<int*>f(int*); // (2)

template <class T> void f(T*); // (3)

int x; foo(&x); // При разрешении перегрузки выберет 3, но вызовет 2.
// Специализация не участвует в разрешении перегрузки.

template <class T, class U> void foo(T, U) // (1)
{std::cout << "foo(T, U)\n";}

template <class T, class U> void foo(T*, U*) // (2)
{std::cout << "foo(T*, U*)\n";}

template <> void foo<int*, int*>(int*, int*) // (3)
{std::cout << "foo(int*, int*)\n";}


int main() {
    int x;
    foo(&x, &x); // -> (2)
}



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-----------------------------Шаблоны классов--------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// PoD - точка объявление, место с которого компилятор знает (имя типа)/(тип и имя) класса/объекта.

template <class T> struct fwnode /* POD */ { // forward node
    T data;
    fwnode<T>* next_;
};

//----------------------------------------
template <class T> class Stack {
    fmnode<T>* top;
};

// Stack<int>  -> CE

template <> class Stack<int> {
    int* content;
};

// специализировать типы только в хедерах

// частичная специализация
template <class T> class Stack<T*> {
    T** content;
};

// частичная специализация должна быть менее общей.


// Задача: разрешение синтаксической неоднозначности
struct Data {
    template <class T> T read() const;
};

template <class Reader> class DataReader {
    const Reader& source_;
public:
    DataReader(const Reader& s) : source_(s) {};
    template <class R> R read(); // source_.read()
};

template <class Reader>
template <class R>
R DataReader::read() {
    R res = source_.template read<Reader>(); // решение
    return res;
}


// Как-бы частичная специализацию функции

template <class T> struct FImp {
    static int f(T t) { return 0; }
};

template <class T> struct FImp<T*> {
    static int f(T* t) { return 1; }
};

template <class T>
int f(T t) { return FImp<T>::f(t); }



int main() {
    int i;
    std::cout << f(i) << ' ' << f(&i) << '\n';
}


//----------------------------------------
// Вывод типов шаблона 
// - процес получение типа шаблонна через аргументы.

// Хинт для агрегатной инициализации.
template <class T>
struct NamedValue {
    std::string name;
    T value;
}
NamedValue(const char*, const char*) -> NamedValue<std::string>;
NamedValue n{"hello", "world"}; // OK



// неявные хинты
template <class T> struct Value {
    T value_;
    Value(T val) : value_(val) {}  // если есть конструкторю
};
template <class T> Value(T) -> Value<T>;      // implicit 
template <class> Value(Value<T>) -> Value<T>; // implicit


//----------------------------------------
// Разрешение имен в шаблонах.

class Bar {
private:
    int foo(int);
public:
    int foo(char);
};

int main() {
    Bar b;
    b.foo(1); // CE <-> сначало разрешается перегрузка, затем чекается модификатор доступа.
}


//----------------------------------------
/* 
    Двухвазное разрешение имен

    1. До инстанцирования. Шаблоны проходят общую синтаксическую проверку.
       Разрешаются независимые имена.

    2. Во время инстанцирования. Происходит специальная синтаксическая проверка.
       Разрешаются зависимые имена.

    * Зависимое имя - имя, семантически зависит от шаблонного параметра.

    Разрешение зависмых имен откладывается до подстановки шаблонного параметра.
*/ 


template <class T> void foo(T) { std::cout << "T\n"; }

struct S { };

template <class T> void call_foo(T t, S x) {
    foo(x); // независимое имя 
    foo(t); // зависимое имя
}

void foo(S) { std::cout << "S\n"; }

int main() {
    S s;
    call_foo(s, s);   // T S
}



//----------------------------------------
// Пример Вандерворда

template <class T> struct Base {
    void exit();
};

template <class T> struct Derived : Base<T> {
    void foo() {
        exit(); // exit - не зависимое имя, поэтому, может связаться не с Base::exit
        
         Base::exit(); // ok 
        // или 
        this->exit(); // ok
    }
}

//----------------------------------------
// Зависимые имена типов.

template <class T> struct S {
    template <class U> void foo(U* p) { }
};

template <class T> int foo(const T& x) {
    /*typename*/ T::subtype* y; // считается за статическое поле.
    
    S<T> s;

    s. /* template */ foo<T>(y); // Считается за дефолтное поле.
}








//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-----------------------------------ODR--------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// Все объявление внтури 1 файла исходного текста - единица трансляции.

// * Каждый файл после препроцессинга транслируется раздельно.

// myfile.cc
int bar(); // declaration bar.

int foo() { return bar() + 1; } // difinition и declaration foo. 

// Мы говорим, что у foo и bar здесь внешнее связывание - видно из другой единицы трансляции

// Здесь мы верим:
//    У bar где-то есть определение.
//    Что мы угадали сигнатуру.


//----------------------------------
// Внутреннее связывание
extern int v0; // объявление, внешнее связывание.
static int v1; // объявление и определение, внутреннее связывание.

int foo() {extern int y; /* Внешнее связывание. */ }
int bar() {extern int z; /* Связывания нет - переменная никак не может быть использованна */ }

template <class T> int bar(T x); // Внешнее.
namespace { int buz(); } // Внутреннее.

struct T; // Внешнее
template <class T> struct U; // Внешнее.
struct S { static int x; }; // Внешнее.


//################################################
// test.cpp
#include <iostream>

void hi() { std::cout << "hi from test.cpp\n"; }


// main.cpp
#include <iostream>

extern void hi();

int main() {
    hi();
}
//################################################


//----------------------------------
// One Definition Rule
/*
    1. No translation unit shall contain more than one definition of any variable,
       function, class type, enumeration type, template [...].
    
    2. Every program shall contain exatly one definition of every non-inline function or 
       variable that is odr-used in that program outside of a discarded statement.

    * Discarded statement - if noexcept(false);
    * Odr-usege - нуждается в определении.
*/

// header.h
#pragma once  

int x; // Потенциальное нарушение ODR.

int f(int) { return n; } // Потенциальное нарушение ODR.

struct S { int x; }; // Все ОК.f


//----------------------------------
// inline
// * inline - не влияет на связывание 

// header.h
#pragma once

inline int foo(int n) { return n; } // Внешнее связывание. Ок. Исключение из ODR.

static int foo(int n) { return n; } // Внутренее связывание. Ok.



//----------------------------------
// Специализация и inline/
// Любая специализация должна быть помечена inline <- правило.


//----------------------------------
// ODR-usage

struct S {
    static const int n = 5; // declaration
};

int x = S::n + 1; // n not ODR-used

int foo(const int *x) { return *x; }

const int S::n; // definiton

int y = foo(&S::n) + 1; // n ODR-used



// #################################################################
/*
    Разрешение имен — это процесс, при котором 
    компилятор определяет, какое имя (идентификатор) соответствует 
    конкретному объекту, функции, классу, пространству имен и т.д., к
    огда оно используется в коде. 
    В C++ разрешение имен происходит на этапе компиляции, 
    и для этого компилятор использует несколько правил.

    Компилятор ищет идентификаторы в ближайшей области видимости, 
    начиная с самой вложенной (локальной) и заканчивая глобальной.

    Вывод типов — это механизм, который позволяет компилятору автоматически 
    определять тип переменной или выражения, основываясь на контексте
*/
// #################################################################





//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-----------------------------------SFINAE-----------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// системный подход
template <class...> void_t = void;

template <class, class = void> has_typedef_foobar : false_type{} 
template <class T, class void_t<typename T::foobar>> has_typedef_foobar : true_type {};

struct S { using foobar = float; };



//----------------------------------
// SFINAE в функциях на enable_if'e 
template <class T, std::enable_if_t<(sizeof(T) <= 4)>* = nullptr>
void foo(T) { std::cout << "may be int\n"; }

template <class T, std::enable_if_t<(sizeof(T) > 4)>* = nullptr>
void foo(T) { std::cout << "not int\n"; }


int main() {
    int x;
    foo<int>(x); 
}







//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//---------------------------------constexpr-----------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// constexpr and const
constexpr int arr[] = {2, 3, 5, 7, 11};
constexpr const int* x = &arr[3]; // stateful pointer на const данные в массиве


// constexpr control flow
template <class T> 
void foo(T x) {
    if constexpr (sizeof(T) > 4) { // желательно использовать в template функциях 
        // точно проверка первой фазы процесса разрешения имен и __может быть__ второй.
    }
    // точно провекра первой фазы и точно проверка второй.
}



template <class T>
constexpr size_t ilist_sz(std::initializer_list<T> intit) {
    constexpr size_t init_sz = init.size(); // CE: не пройдет синтаксической проверки.
    return init_sz;
}



//----------------------------------
// constexpr virtual функции
struct B { 
    constexpr B() = default; 
    virtual constexpr int data() const { return 1; }
};

struct D : B { 
    constexpr D() = default;
    constexpr int data() const override { return 2; }
};


consteval int foo() {
    const B b;
    const D d;
    const B* bases[] = {&b, &d}; // псевдо стирание типов

    return 
        bases[0]->data()      // здесь
        + bases[1]->data();   // нет вызова через vtable
}


int main() {
    static constinit int x = foo();
    std::cout << x << '\n';
}




//----------------------------------
// Вариабельные суффиксы

// default суффикс
struct S {
    int i;
    friend std::ostream& operator<<(std::ostream& out, const S& s) {
        return out << "int: " << s.i; 
    }
};

S operator "" _s(unsigned long long arg) {
    S s;
    s.i = arg;
    return s;
}

int main() {
    std::cout << 1_s << '\n';
}

// Вариабельный суффикс
template <class... Ts> 
constexpr auto binparser(unsigned long long acc, char c, Ts... cs) {
    unsigned digit = (c == '1') ? 1 :
                     (c == '0') ? 0 : throw "out of range";

    auto next = (acc << 1) + digit;
    if constexpr (sizeof...(Ts) != 0) {
        return binparser(next, cs...);
    }
    return next;
}

template <char... Chars>
constexpr auto operator "" _bin() {
    return binparser(0ull, Chars...);
}


// суффикс для строчки
template <class... Ts> 
constexpr auto tritparser(unsigned long long acc, const char* s, size_t len) {
    char c;
    int digit;
    size_t pwd = len - 1;
    while ((c = *s++)) {
        digit = (c == 'j') ? -1 
                         : (c == '0') ? 0 
                         : (c == '1') ? 1 
                         : throw "out of range";
        acc += digit * std::pow(3, pwd--);
    }
    return acc;
}

constexpr auto operator "" _trit(const char *s, size_t len) {
    return tritparser(0ull, s, len);
}

int main() {
    std::cout << "10j01"_trit << '\n';
}




//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//---------------------------------concepts-----------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


// в requires может быть constexpr/consteval функция
// для разрешения имен функции с requires - разные функции

//----------------------------------
// сложные requries
template <class T> constexpr int somepred() { return 14; }

template <class T>
    requires(somepred<T> == 42) // значение реально вычисляется
bool foo(...) {} // будет выкинута

template <class T>
    requires requires(T t) { somepred<T>() == 42; } // просто проверка, что операция возможна
bool foo(...) {} // не будет


// #################################################################
/*
Манглирование (англ. name mangling) в C++ — это процесс, при котором компилятор изменяет имена
функций и переменных, чтобы включить информацию о типах их аргументов, возвращаемых значениях и
других деталях, необходимых для работы механизма перегрузки функций и связывания при компиляции.

В C++ перегрузка функций позволяет объявлять несколько функций с одним и тем же именем, но с разными
параметрами. Для того чтобы различать эти функции на уровне машинного кода, компилятор выполняет
манглирование, добавляя к имени функции сигнатуру типов её параметров и возвращаемого значения.
*/
// #################################################################


//----------------------------------
// возможный синтаксис в requires() {}

requires(T t) {
    t + t; // если возмножно [simple] 
    typename T::inner; // если есть вложенный тип [type]

    {*t} -> typename T::iner; // [compound]
    {*t} -> convertible_to<typename T::inner /*, 2-ой аргумент будет подставленн сам */>; // [compound]

    requires( noexcept(++t) ); // вычисляется [nested]
} 

//----------------------------------
// возможный синтаксис для concept

template <class From, class To>
concept convertible_to = 
    std::is_convertible_v<From, To> &&
    requires(From (&f)()) { static_cast<To>(f()); };

template <class T> int foo(T x);
requires convertible_to<T, int>




//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//--------------------------------вывод типов---------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Связывание (binding) - сопоставление ссылки как альтернативного имени именованному.
// объекту в памяти (gvalue).


int x = 1;
const int& c = x + 1; // продление жизни объекту для const ссылки.
          
int&& d = /*prvalue - рецепт построения объекта*/ x + 2; 
// специальный объект, позволяющий одновременно связывание с временным объектом и изменяемое значение.

//----------------------------------
// Кросс-связывание

// *value - характеристика выражения

// rvalue ref не может быть связан с lvalue
int x = 1;
int&& y = x * 3; // ok
int&& z = x; // ne ok

// не const lvalue ref не может быть связан с rvalue
int& c = x * 3;
const int& d = x * 3; // ok, продление жизни

// но при этом сама ссылка задает имя и адрес и является lvalue.

int&& e = y; // fail, не rvalue
int& f = y; // ok

//----------------------------------
// Конструкторы, rule of five

/*
    Деструктор по умолчанию
        • defaulted если нет ни одного пользовательского деструктора

    Конструктор копирования (аналогично ведёт себя копирующее присваивание)
        • deleted если его нет, но есть пользовательское перемещение или move ctor.
        • Иначе defaulted если его нет (но если при этом есть деструктор или парный копирующий метод, то это
            deprecated).

    Перемещающий конструктор (аналогично перемещающее присваивание)
        • deleted если его нет, но при этом есть парный перемещающий метод или copy ctor или копирующее
            присваивание или деструктор. 
        • Иначе defaulted если его нет.
*/

// Правило пяти
// Если ваш класс требует определение одно из 5 методов (....), то лучше нетривиально определить все 5.

//----------------------------------
// Moved-from state
// В стандарте не определен.
// Обычно либо не изминит rhs (right-hand side), либо привет его в 
// "консистентное но не определенное состояние".

//----------------------------------
// Аннотация методов

class X{
    std::vector<char> data_;
    public:

    X() = default;

    const std::vector<char>& data() const & { return data_; } 
    const std::vector<char>&& data() const && { return std::move(data_); } 
};

X obj;
vector<char> a = obj.data(); // copy
vector<char> b = X().data(); //move. Избегаем провисания ссылок.


//----------------------------------
// Правила auto

const int& x = 42;
auto y = x; // -> int y = x;
auto y = x; // -> const int& z = x;


//----------------------------------
// Идентичность и перемещаемость
// 2 фундаментальные характеристики выражения.

int foo();

int x = foo(); // перемещаемость. rvalue expression
int y = x; // идентичность. lvalue expression

Buffer d; Buffer b = std::move(b); // идентичность И перемещаемость. xvalue expression


//----------------------------------
// Правила свертки ссылок

template <class T
          class R = std::remove_reference_t<T>&&>
R almost_move(T&& a) {
    return static_cast<R(a);
}
int x; auto&& t = almost_move(x); // T -> int&, T&& -> int&,
                                  // R -> int&&, t -> int&&.
                                  // T -> int&, а не int - исключение для rvalue ссылок


//----------------------------------
// Неуниверсальные сслыки.
const auto &&x = y; // не UR


//----------------------------------
// decltype
// auto - либо режит либо добавляет уточнятили

const int x = 5;

auto y = x; // y -> int
auto& z = x; // z -> const int&

decltype(x) t = 6; // t -> const int

//----------------------------------
// decltype: name and expression

struct P { int x, y; };

P p {1, 2};
const P& pr = p;

// decltype от name. '.' - часть имени
decltype(pr.x) x = 0; // -> int x = 0; точный тип

// decltyep от expression.
decltype((p.x)) x = 0; // -> const int& x = 0;

// decltype работает в с завивисимости от категории значений.
int x;

decltype((x)) t2 = y; // lvalue expr -> int&
decltype(std::move(x)) t3 = 1; // xvalue expr -> int&&
decltype(x + 0) t4; // prvalue expr -> int


//----------------------------------
// decltype(auto)
double x = 1.0;
decltype(auto) tmp = x; // -> double tmp = x;
decltype(auto) tmp = (x); // -> double& tmp = (x);



//----------------------------------
// Lvalue - это прописка

// Необязтально то, что может стоять слева от =.
int x[10], y[10];
x = y; // err

// Необязательно есть адресс.
struct S { char t; int bf:1; } sobj; // у bitfield нет адреса.
auto addr = &sobj.bf; // err

// Lvalue - выражение, результат которого обладает идентичостью.
int&& s = 1;
const int& t = 2;


//----------------------------------
// Xvalue - выражение ведущее себя, как функция, возвращающая правую ссылку.


//----------------------------------
// Дополнительный синтаксис записи concepts

// Аббревиированный аргумент.
void sort(Sortable auto&);

std::vector V;
// ....
std::input_iterator auto In = std::back_inserter(V);