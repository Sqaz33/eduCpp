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

//#### requires - хук в разрешение имен.


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
    {*t} -> convertible_to<typename T::inner /*, 1-ый аргумент будет подставленн сам */>; // [compound]

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



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//--------------------------------кейсы Йосьюттиса----------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

class Customer {
    std::string fst, snd;
public:
    template <class S1, class S2 = const char*> // для каждого аргумента по умолчанию - тип по умолчанию.
        requires(
            std::is_convertible_v<S1, std::string> && 
            std::is_convertible_v<S2, std::string>
        )
    Customer(S1&& s1, S2&& s2 = ""): // universal ref
        fst(std::forward<S1>(s1)), snd(std::forward<S2>(s2)) // perfect forwarding 
    {}
};


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------Вариабельные шаблоны----------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//----------------------------------
// Паттерны раскрытия

// g(int(x), double(y));
template <class... T> void g(T... args) {
    f(args...); // -> f(x, y);
    f(&args...); // -> f(&x, &y);
    f(h(args)); // -> f(h(x), h(y));
    f(const_cast<const Types*>(&args)...); // f(const_cast<const Types*>(x), ...
}


auto sum_all() { return 0; }

template <class T, class... Ts>
auto sum_all(T&& arg, Ts&&... args) {
    return arg + sum_all(std::forward<Ts>(args)...);
}


//----------------------------------
//вызов функции для каждого аргумента пака

struct expand_t {
    template <class... T> expand_t(T...) {}
};

template <class... T> void foo(T... ts) {
    expand_t{(bar(ts), void(), 0)...}; // void() - блокировка user оператора ',' - запятая.
}

//----------------------------------
// 2 пака
template <class I, I... is1, I... is2>
constexpr auto concat(std::integer_sequence<I, is1...>, std::integer_sequence<I, is2...>) { // пак типов несет на себе объекты
    return std::integer_sequence<I, is1..., is2...>;
}

template <class I, I i1, I... is1>
constexpr auto sum(std::integer_sequence<I, i1, is1...>) {
    if constexpr(sizeof...(is1) != 0) {
        return i1 + sum(std::integer_sequence<I, is1...>{});
    }
    return i1;
}


//----------------------------------
// свертки (fold expression) 
template <class T, class... Ts>
void print(T&& arg, Ts&&... args) { 
    std::cout << arg;
    ((std::cout << ' ' << args), ...); 
    std::cout << '\n';
}

// задачка
template <class T> struct Node {
    T data;
    Node* left;
    Node* right;
};

template <class T, class... Args>
Node<T>* tree_get(Node<T>* root, Args... args) {
    return (top ->* ... ->* args);
}


int main() {
    auto left = &Node<int>::left;
    auto right = &Node<int>::right;

    // auto* res = tree_get(top, left, right);

}


//----------------------------------
// Свертки с пустыми пачками
// Операции для пустых пачек в свертках - ошибка, кроме ка

// (... + args) -> bool = true
// (... && args) -> bool = false
// (..., args) = void



namespace msa {

template <class T, class... Ts>
constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

template <class... T>
struct are_same : std::bool_constant<are_same_v<T...>> { };

} // namespace issametypes


template <class... Args>
    requires msa::are_same_v<Args...>
auto sum_all(Args&&... args) {
    return (args + ...);
}

template <std::same_as<int>... Args>
int sum_allInt32(Args&&... args) {
    return (args + ...);
} 

int main() {   
    std::cout << sum_allInt32(1, 2, 3, 4) << '\n';
    std::cout << sum_all(1, 2, 3, 4, 1) << '\n';

}

//----------------------------------
// жадный матчинг
template <typename T>
class Stack {
public:   
    template <typename... Args>
    void emplase_back(Args&&... args) {
        top_ = std::make_unique<StackElem>(std::move(top_), std::forward<Args>(args)...); 
    }   

    template <typename... Args>
    void push_back(const Args&... args) {
        top_ = std::make_unique<StackElem>(std::move(top_), args...);
    }
private:
    struct StackElem {
        StackElem(std::unique_ptr<StackElem> nxt, T e) :
            elem(e)
            , next(std::move(nxt))
        {}
        
        template <class... Args>
        StackElem(std::unique_ptr<StackElem> nxt, Args&&... args) : // поменять местами пачку и указатель
            elem(std::forward<Args>(args)...)
            , next(std::move(nxt))
        {}

        T elem;
        std::unique_ptr<StackElem> next;    
    };

private:
    std::unique_ptr<StackElem> top_;

};

struct S {
    S() { std::cout << "S()\n"; }
    S(int) { std::cout << "S(int)\n"; }
    S(const S&) { std::cout << "S(const S&)\n"; }
    S(S&&) { std::cout << "S(S&&)\n"; }
};


int main() {   
    Stack<S> s;
    s.emplase_back(1);
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//------------------------------------лямбда----------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//----------------------------------
// лямбда без захвата - объект с перегруженным приведением к указателю на функцию.
// лямбда с захватом - объект с перегруженным operator();

auto adder = [](int x, int y) -> int { return x + y; };

struct Closure {
    static int func(int x, int y) { return x + y; }
    using func_t = std::decay_t<decltype(func)>;
    operator func_t() const { return func; };
};

// есть приведения типов
int (*pf)(int x, int y) = adder; // implicit cast


//----------------------------------
// Callable:
// Указатель на функцию.
// Объект с приведением к указателю на функцию.
// Объект с круглыми скобками.


//----------------------------------
// std::invoke - унифицирует вызов


//----------------------------------
// closure
auto test = []{}; // уникальная closure
test = []{}; // CE

// хак
auto test = +[]{}; // test - уже указтель на функцию
test = []{};


//----------------------------------
// для лучшего понимания лямбда

decltype([](auto x) { return 2 * x; }) twice;

auto x = twice(2);

//----------------------------------
// Явные шаблонные переменные в лямбде
template <typename T, typename U>
concept Addble = requires(T t, U u) { t + u; };


auto l = []<class T, class U>(T x, U y) requires Addble<T, U> { return x + y; };


//----------------------------------
// Захват аргументов

int a = 1, b = 2;

auto param_adder = [a, b](int x, int y) {
    return x * a + y * b;
};

// реализация лямбды
struct Closure {
    int a_, b_;
    Closure(int a, int b) : a_(b), b_(b) {}
    auto operator()(int x, int y) const { return x * a_ + y * b; }
};


//----------------------------------
// mutable
 
struct S {
    int f() { return 1; }
};


int main() { 
    S s;  
    auto f = [s]() mutable { return s.f(); }; // захват по значению - const
    auto f2 = [&s]() { return s.f(); }; // захват по значению - const
}

//----------------------------------
// Зазват ссылки, как константы
int main() { 
    int a = 5;
                    // rename
    auto p = [&r = std::as_const(a)](int x) mutable { r += x; return x; }; // CE
}


//----------------------------------
// Улучшение проброса

std::unique_ptr<int> i = std::make_unique<int>(1);

auto l = [r = std::move(i)]() { return 1; }; // не копируется 

template <class Fun, class... Args>
decltype(auto) transparent(Fun&& fun, Args&&... args) {
    return std::invoke(std::forward<Fun>(fun), std::forward<Args>(args)...);
}


//----------------------------------с
// паки в лямбдах
template <class... Args>
void f(Args&& ... args) {
    auto varPack = [...a = std::forward<Args>(args)] { return sizeof...(args); };
    std::cout << varPack() << '\n';
} 


//----------------------------------
// Каррирование

auto add = [](auto x, auto y) { return x + y; };

template <class Func, class... Args>
auto curry(Func func, Args&&... Args) {
    return [=](auto... rest) {
        return func(std::foward<Args>(args)..., rest...);
    }
}

auto add4 = curry(add, 4);
int main() {
    assert(add4(11) == 15);
}


//----------------------------------
// Правило захвата
// [=] {} - Захватывается только локольный нестатический контекст. (global: int n; static int n = 0;)


//----------------------------------
// Проброс в зависимости от value
template <class T> 
auto fwd_capture(T&& x) {
    struct CapT { T value; };
    return CapT{ std::forward<T>(x) };
}

int main() {
    int x = 1;

    auto foo = []<class T>(T&& a) {
        return [a = std::forward<T>(a)]() mutable { return ++a; }; // берет только по значению.
    };

    auto f = foo(x);
    f();
    assert(x == 1); // OK

    auto bar = []<class T>(T&& a) {
        return [a = fwd_capture(a)]() mutable { return ++a.value; };
    };

    auto b = bar(x);
    b();
    assert(x == 2); // OK
}


// Обобщение пробросса
template <class... T> 
auto fwd_capture(T&&... x) {
    return std::tuple<T...>(std::forward<T>(x)...);
}


int main() {
    int x = 1;

    auto foo = []<class... T>(T&&... a) {
        return [a = std::fwd_capture(a...)]() mutable { 
            return ++std::get<0>(a); 
        }; 
    };
}



// #################################################################
// Еще про structure bindings

// на tuple
auto [xv, yv, zv] = make_tuple("a", 1.0, 4); 
auto [xrv, yrv] = make_tuple(2, 3);

// на массивы
int a[2] = {1, 2};
auto& [xr, yr] = a;

// на структуры
struct { int x, y; };
const auto& [xcr, ycr] = b;
// #################################################################


// Улучшение обобщенного пробросса
int main() {
    int x;

    auto foo = []<class... T>(T&&... a) {
        return 
        [a = std::forward_as_tuple(a...)]() mutable {
            return ++std::get<0>(a);
        };
    };
}


//----------------------------------
// Применение кортежа как аргументов
std::apply([&fn]<class... T>(T&& ... xs) {(fn(std::forward<T>(xs)), ...);}, t);



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//---------------------------------type erasue--------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

int main() {
    std::any x = 1;
    std::cout << std::any_cast<int>(x) << '\n';
    x = std::string("asfd");
    x.has_value();




    std::variant<int, double, char> v;

    v = 1;

    std::cout << std::holds_alternative<int>(v) << ' '
              << std::holds_alternative<char>(v) << '\n';

    std::cout << std::get<int>(v) << '\n';

    try { std::get<double>(v); } 
        catch(const std::exception& e) {std::cout << e.what() << '\n'; } 

    


    std::vector<decltype(v)> vec(2);
    vec[0] = v;
    vec[1] = 1.1;

    for (const auto& var : vec) {
        std::visit(
            [](auto&& arg) { std::cout << arg << '\n'; }
            , var
        );
    }
}  


//----------------------------------
// перегрузка лямбд
template <class... F> struct overload : F... { using F::operator()...; };
template <class... F> overload(F&&...) -> overload<F...>;


int main() {
    std::vector<std::variant<int, double>> v(2);
    v[0] = 1;
    v[1] = 2.1;

    auto o = overload(
        [](int) { std::cout << "int\n"; },
        [](double) { std::cout << "double\n"; }
    );

    for (const auto& var : v) {
        std::visit(o, var);
    }
}  


// #################################################################
// rodate
/*
        .rodata (Read-Only Data) — секция памяти, где хранится
        константные данные программы, такие как
        строковые литералы и значения const. Эта секция
        является только для чтения и обычно располагается в
        памяти с правами доступа, запрещающими запись.
*/
// #################################################################



// #################################################################
// heap indirection
/*
        Heap indirection (перенаправление через кучу) — это 
        паттерн проектирования структуры данных, при котором 
        указатели или ссылки на данные, находящиеся в куче (heap), 
        хранятся в другой структуре данных
*/
// #################################################################



//----------------------------------
// рекурсия для лямбд
int main() {
    std::function<int (int)> factorial = [&] (int i) {
        return (i == 1) ? 1 : i * factorial(i - 1);
    };

    auto closure []() {}; // объект на стеке
    std::function<void(void)> func = []() {}; // heap indirection
}  

// c++23 move only function
std::move_only_function<int(int)> func = [x == std::move(v)] { std::move(f)();} // вызывается 1 раз





//----------------------------------
// явный аргумент this 

// проблема
// std::function<int(int)> func = [x](int a) {...}
// func(1) func captured x
// std::move(func)(2) finally use captured x  x - можно мувноть

int main() {
    int m = 1;
    std::vector<int> v;
    auto callback = [m, &v](this auto&& self) {
        v.push_back(std::forward_like<decltype(self)>(m));
    }; 

    callback(); // x -> copy to v
    std::move(callback)(); // x -> move to v
}   



//----------------------------------
// c++23 улучшение
int main() {
    // c++ 23
    auto factorial = [](this auto&& self, int i) {
        return (i == 1) ? 1 : i * self(i - 1);
    }; 
}   



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//------------------------------диапазоны (range)-----------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


int main() {

    std::vector v = {1, 2, 3, 4};

    auto double_even = v 
                    | std::views::filter([](int a) -> bool { return !(a % 2); })  
                    | std::views::transform([](int a) -> int { return a * 2; });

    std::vector<int> res;


    std::copy(double_even.begin(), double_even.end(), std::back_inserter(res));
}   


//----------------------------------
// mem_fn
struct S { int x, y; };

int main() {
    S s; s.x = 1;

    auto sx = std::mem_fn(&S::x);
    std::cout << sx(s) << '\n'; // 1


    std::array<S, 4> arr({ {1, 1}, {2, 2}, {3, 3}, {4, 4} });  
    
    auto vv = std::views::transform(arr, std::mem_fn(&S::x));
    auto it = std::ranges::find(vv, 4);


    std::cout << it.base()->x << '\n';  // 4

}   

//----------------------------------
// провисание итератора на view

struct S { int x, y; };

int main() {
    std::array<S, 4> arr({ {1, 1}, {2, 2}, {3, 3}, {4, 4} });  
    
    auto it = std::ranges::find(
            std::views::transform(arr, std::mem_fn(&S::x)), 4); 
    // it - ranges::dengling


    // std::cout << *it.x << '\n';  
}   




//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//---------------------------Полиморфные аллокаторы---------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


// #################################################################
// alignment
/*
        В C++ выравнивание (alignment) — это характеристика 
        данных в памяти, которая определяет, на каком
        адресе памяти должен начинаться объект данного типа. 
        Оно связано с тем, что процессоры могут быть
        настроены на наиболее эффективное обращение к данным, 
        когда они расположены по определённым адресам.
*/
// #################################################################


/*
    A polymorphic_allocator is intended to let you have an 
    allocator whose behavior is dynamically determined at runtime.
*/

//----------------------------------
// Абстракция ресурса в памяти.
struct memory_resource {
    void* allocate(size_t n, size_t align = alignof(max_align_t));
    void deallocate(void* p, size_t n);
    bool is_equal(const memory_resource&) const noexcept;

    virtual ~memory_resource() {}
protected:
    virtual void* do_allocate(size_t n, size_t align) = 0;
};

int main() {

    char buffer[10] alignas(double);

    std::cout << sizeof buffer << '\n';
}   


//----------------------------------
// pmr
int main() {
    namespace pmr = std::pmr;

    constexpr size_t sz = 1000 * sizeof(int);
    std::array<std::byte, sz> buf;
    pmr::monotonic_buffer_resource mem_resource(&buf, sz);

    pmr::vector<int> v(&mem_resource);
}   

//----------------------------------
// Абстракция аллокатора
template <class T> struct polymorphic_allocator 
{ /* реализует аллокаторы из с++11, хранит ссылку на memory_resource.*/ };

// Полиформные аллокаторы - не пропагируется.



//----------------------------------
// Про pmr-контейнеры


// list и трюк с union
template <class T> struct node;

template <class Tp> struct node_base { 
    node<Tp>* next_ = nullptr; 
    node_base(const node_base&) = delete;
    node_base(node_base&&) = delete;
};

template <class Tp> struct node : node_base<Tp> {
    union { Tp value_; };
    // начиная с с++11, union может содержать классы с конструктором, но не вызывает их. 
};

template <class T> struct slist {
    using value_type = T;
    using iterator = <итератор>;
    
    // все остальное
private:

    using allocator_type = pmr::polymorphic_allocator<std::byte>;

    node_base head_; // сторожевой узел пустого списка.
    node_base* ptail;
    size_t size;
    allocato_type alloc_; // храним аллокатор.
};


// использование аллокатора
template <class T>
template <class... Args>
iterator slist<T>::emplace(iterator i, Args&&... args) {
    void* mem = alloc.resource()->allocate(sizeof(node), alignof(node));
    node* ret = static_cast<node*>(mem);
    ret->next = i.prev->next;
    alloc_.construct(std::addressof(ret->value_), // * std::addresof - всегда возвращает адрес, даже если перегружен &.
                    std::forward<Args>(args)...);
    i.prev->next = ret;
}

// прекол жесткий
pmr::vector<int> foo() { return pmr::vector<int> {some_alloc}; }
pmr::vector<int> v = foo(); // v.alloc - some_alloc
pmr::vector<int> w;
w = foo(); // w.alloc - default alloc

// аллокаторы - привязаны к месту.





//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//-------------------------------Умные указатели------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// Владелец памяти (ресурса)- тот, кто может ее выделить и освободить.

// value ресурсом владеет компилятор.

// RAII: resouce acquisition is initialization


//----------------------------------
// prvalue elision 
// URVO
void foo(SP<int>); // SP<> - не копируется и не перемещается

foo(SP<int>(new int(42))); // OK

auto bar() {
    return SP<int>(new int(42));
}
auto n = bar(); // OK

auto buz() {
    SP<int> t(new int(42));
    return t; // FAIL, NRVO case // * пытается выбрать move темы, implicit move.
}


//----------------------------------
// aliasing construction
/*
 * Конструктор aliasing (алиасный конструктор): создаёт shared_ptr, который разделяет информацию о
 * владении с начальным значением r, но хранит несвязанный и неуправляемый указатель ptr. Если этот
 * shared_ptr будет последним в группе и выйдет из области видимости, он вызовет хранимую функцию
 * удаления для объекта, изначально управляемого r. Однако вызов get() у этого shared_ptr всегда
 * будет возвращать копию ptr. На программисте лежит ответственность за то, чтобы ptr оставался
 * допустимым, пока существует этот shared_ptr, как, например, в типичных случаях использования, где
 * ptr является членом объекта, управляемого r, или является алиасом (например, результатом
 * downcast) r.get(). Для второй перегрузки, принимающей rvalue, r становится пустым, и r.get() ==
 * nullptr после вызова. (начиная с C++20)
 */
template <class Data>
class Tree {

    struct Node {
        std::shared_ptr<Node> left, right;
        Data d;
    };
    shared_ptr<Node> top_;

public:
    shared_ptr<Data> find(int inorder_pos) {
        // поиск shared_ptr<Node> spn;
        return {spn, &(spn->d)};
    }

};

//----------------------------------
// Приведение 
 
class B { public: virtual ~B() {} };

class D : public B {};


int main() {
    using namespace std;
    shared_ptr<B> b = std::make_shared<D>();
    shared_ptr<D> d = std::static_pointer_cast<D>(b);

    shared_ptr<D> d1 = std::dynamic_pointer_cast<D>(b);
}

//----------------------------------
// Про провисание weak pointer
int main() {
    std::weak_ptr<int> b;

    {
        auto a = std::make_shared<int>(1);
        std::weak_ptr<int> b = a;

        auto l = b.lock();
    }

    assert(b.expired());
    assert(b.lock() == nullptr);
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//---------------------------Динамический полиморфизм-------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// Мотивация полиморфизма.
/*
    * Нужно переиспользовать интерфейс базового класса для разных 
      произовадных классаов для вызова через косвенность.
    
    * Нужно вернуть из функции что-то в зависимости от условий времени 
      исполнения.
    
    * Нужно положить в контейнер много разнородных объектов.

    * Нужно переиспользовать реализацию.

*/

template <class Derived> struct object_t {
    void draw(std::ostream& os, size_t pos) const {
        static_cast<const Derived*>(this)->draw(os, pos);
    }

protected: object_t() = default;
};


struct int_t : object_t<int_t> {
    int data;
    void draw(std::ostream& os, size_t pos) const {
        os << std::string(pos, ' ') << data;   
    }
};



// #################################################################
// mixin
/*
        В C++ mixin — это шаблон проектирования, который 
        позволяет добавлять функциональность к классу через
        множественное наследование. Mixin-классы не 
        предназначены для самостоятельного использования, их
        цель — предоставлять методы и данные, которые могут 
        быть "подмешаны" в другие классы.
*/
// #################################################################

//----------------------------------
// CRTP-mixin clone
struct object_t {
    virtual std::unique_ptr<object_t> clone() const = 0;
    virtual ~object_t() {}
};

template <class Base, class Derived> 
struct MixClonable : public Base {
    std::unique_ptr<Base> clone() const override {
        return std::unique_ptr<Base>(
            new Derived(static_cast<const Derived&>(*this))
        );
    }
};

struct int_t : MixClonable<object_t, int_t> { };


//----------------------------------
// Симметрия исключений
struct File;

struct IFile : File;
struct OFile : File;

struct IOFile : IFile, OFile;

try {
    IOFile iof;
    File& f = iof; 
    throw IOFile();
} catch(File&) {
    // сюда мы не попадем.
}
// Через RTTI не видно неоднозначные базы.



// База должна быть правильно по модификаторам, чтобы работал RTTI.
struct InputFile : protected File {
    File* spawn() { return static_cast<File*>(this); }
};

InputFile* inf = new InputFile(11);
File* f = inf->spawn(); // OK
f = static_cast<File*>(inf);

try { throw inf; }
catch(File& f) { /*Мы не поймаем InputFile inf. */ }


//----------------------------------
// Влияние на инициализацию

struct File {
    File(int);
};

struct IFile : virtual File {
    IFile(int b) : File(b) {}
};

struct OFile : virtual File {
    OFile(int c)  : File(c) {}
};

struct IOFile : IFile, OFile {
    IOFile(int d) : IFile(d), OFile(d), File(d) {} 
}


//----------------------------------
// Виды dynamic_cast

dynamic_cast<void*>(p); // к most derived

dynamic_cast<child*>(p); // вниз

dynamic_cast<sibling*>(p); // влево или вправо



//----------------------------------
// Пробеммы динамического полиморфизма

// Требует ссылочной семантики.
void f(Base& b) { b.reset(nullptr); }

// Провоцирует выделение в куче -> Ад с влодением.



























//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------Потоки------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// Поток исполнение - передача управление внутри программы.

// Исполнение программы - исполнения всех ее потоков.

// Логическая многотопоточность (concurrency) внутри 
// программы не имеет отношения к аппаратной параллельности (parallelism).


//----------------------------------
// Пробеммы динамического полиморфизма

int main() {
    std::thread t([]{
        std::cout << "Hello, world!" << std::endl; 
    }); // поток стартует с переданной в коструктор функции или функтора.


    // тут что угодно пока исполняется.
    t.join();  // в этой точке мы ждем поток.
    ....
    или 
    ....
    t.detach(); // мы не ждем завершения потока, нам все равно когда он завершится.
}
// join() или detach() должны быть обязательно.

std::this_thread::get_id(); // получить id потока.



//----------------------------------
// Объект в с++ - все, что угодно требующее места (storage) для хранения значения.

/*
    Область памяти — это либо объект скалярного типа, 
    либо максимальная последовательность
    смежных битовых полей, имеющих ненулевую ширину (C++17, 4.4)
 */

// ---Передача управления и состояние областей памяти.---


//----------------------------------
// Гонка за облать памяти.
namespace {

void __attribute__((noinline)) use(int c) { asm(""); }  

int x;

void race() {
    for (int i = 0; i < 1'000'000; ++i) { x += 1; use(x); }
    for (int i = 0; i < 1'000'000; ++i) { x -= 1; use(x); }
} // для более чем 1 потока - это UB.


} // namespace


int main() {
    std::thread t1{race};
    std::thread t2{race};

    t1.join();
    t2.join(); 

    std::cout << x << std::endl; 
}


// conflicting expressions
/*
        Два выражения  выражений конфликтуют,
        если одно из них изменяет область памяти,
        а другое считывает или изменяет
        ту же область памяти.
        [intro.races]
* /

// race
/*
        Выполнение программы содержит гонку данных, если она содержит
        два потенциально конкурентных (в разных потоках) конфликтующих выражения,
        по крайней мере одно из которых не является атомарным, и
        ни одно из них не "происходит раньше другого" (не состоит в отношении happen before),
        за исключением особого случая для обработчиков сигналов, описанных ниже.
        Любая такая гонка данных приводит к неопределенному поведению.
        [intro.races]

        Два доступа к volatile std::sig_atomic_t - не вызывают гонку.
*/


//----------------------------------
// пример, где нужен volatile
void waitForMMIO() {
    volatile unsigned short* semPtr = MMAPED_ADDR;
    while ((*semPtr) != ISOK); // без volatile - бесконечный цикл - UB.
    // volatile - добавляет побочный эффект в разыминование указателя.
}

// volatile - обещание.

char x[2] = {0, 0}; // агрегат, не скалярный тип.
int i = 0;
int j = 0;

void readerf() { while (i++ < 'g') x[0] += 0x1; }
void readerf() { while (i++ < 'g') x[1] += 0x1; }


int main() {
    std::thread t1{readerf}, t2{writerf};
    t1.join(); t2.join();
} // data race - теперь нет

// Гонка присутствуе - если внутри объекта есть скалярные не атомарные поля.

// std::cout - потоко-безопасен.


//----------------------------------
// mutex и простая синхронизация

// mutex - то класс в библиотеке C++ Standard Library, предоставляющий базовый механизм
// синхронизации для управления доступом к разделяемым ресурсам между потоками. Он позволяет
// организовать взаимоисключающий доступ к ресурсам, предотвращая состояние гонки (race condition),
// когда несколько потоков пытаются одновременно изменить общие данные.
namespace {

void __attribute__((noinline)) use(int c) { asm(""); }  

int x;
std::mutex mforx;

void race() {
    for (int i = 0; i < 1'000'000; ++i) { 
        mforx.lock(); // попытка залочить дважды - ub или std::system_error
        // lock - серилизоваться, пододжать очериди и залочить.
        // std::mutex::try_lock()  попытаться залочить, вернуть результат, и не ждать очериди.
        x += 1; use(x); 
        mforx.unlock(); // попытка разлочить не захваченный mutex - ub
    }
    for (int i = 0; i < 1'000'000; ++i) { 
        mforx.lock();  
        x -= 1; use(x); 
        mforx.unlock();
    }
} 

} // namespace

int main() {
    std::thread t1{race};
    std::thread t2{race};

    t1.join();
    t2.join(); 

    std::cout << x << std::endl; 
}

// mutex - вводит отношение happens-before между доступами.


// #################################################################
// scoped ptr
/*
        scoped_ptr — это концепция управления памятью в C++,
        представляющая умный указатель, который владеет
        динамически выделенным ресурсом (обычно объектом) и
        освобождает его при выходе из области видимости
        (scoped). Когда scoped_ptr выходит из области видимости,
        его деструктор автоматически освобождает
        связанный ресурс, что помогает избежать утечек памяти.
        Важно отметить, что scoped_ptr нельзя
        перемещать или копировать, так как он строго владеет объектом.
*/
// #################################################################


//----------------------------------
// RAII синхронизация
namespace {

void __attribute__((noinline)) use(int c) { asm(""); }  

int x;
std::mutex mforx;

void race() {
    for (int i = 0; i < 1'000'000; ++i) { 
        std::lock_guard<std::mutex> lk(mforx); 
        // lock_guard - RAII обертка над любым классом с интерфейсом lock() и unlock()
        x += 1; use(x); 
    }
    for (int i = 0; i < 1'000'000; ++i) { 
        std::lock_guard<std::mutex> lk(mforx);
        x -= 1; use(x); 
    }
} 

} // namespace

int main() {
    std::thread t1{race};
    std::thread t2{race};

    t1.join();
    t2.join(); 

    std::cout << x << std::endl; 
}

// По логике lock_guard - scoped lock по аналогии со scoped ptr. 

// Критическая секция ~ место куда может ходить только один поток.


//----------------------------------
// Проектирование потоко-безопасных классов.

// Общее между потоками и исключениями - нелокальная передача управления.



// #################################################################
// Exception guarantee
/*
    1. Nothrow (or nofail) exception guarantee. Гарантия отсутствия 
       исключений: функция никогда не выбрасывает исключений.

    2. Strong exception guarantee. Сильная гарантия исключений:
       если функция выбрасывает исключение, состояние программы
       откатывается до состояние перед вызовом функции.
    
    3. Basic exception guarantee. Базовая гарантия исключений:
       программа остается в корректном состоянии. Нет утечек ресурсов
       все инварианты объектов сохоранены. Объект остается в согласованном, 
       но не предсказуемом состоянии (можно корректно вызвать деструктор).

    4. No exception guarantee. Отсутсвие гарантий: если функция 
       выбрасывает исключение - программа может оказатсья в некоректном
       состоянии. 
*/

// * Деструкторы начиная с с++11 - noexcept
// #################################################################


// Уровни безопасности для многопоточного окружения.
/*
        Нейтральный уровень. "Нейтральные объекты"
        Если такой объект защищен синхронизацией, то он безопасен.
        Если нет, то нет. 
        Например: int.

        Ниже нейтрального уровня.
        T* - указатели. 
        То что под ними может быть не синхронизировано.

        Выше нейтрального уровня.
        Безопасным относительно многопоточного окружения
        является объект, любые операции над которым в
        многопоточном окружении не приводят к data race. 
        Иначе говоря - объект не переходит в некосистентное 
        состояние.
*/

class MyBuffer {
    std::mutex bufmut_;
public:
    void pop() {
        std::lock_guard<mutex>{bufmut_};
        --size_;
        destroy(buffer_ + size)_; 
    }

    T top() const; // аналогично
    bool empty() const; // аналогично
};

// API races
Buf s;
if (!s.empty()) {        // все
    auto elem = s.top(); // очень
    s.pop();             // плохо
    use(elem);
}
// Гонка между poo() и top().
// Хот и все 3 метода с lock_guard.


// dead lock
struct DL {
    std::mutex dlmut_;
} 
x, y;

int main() {

    auto f = [](DL& a, DL& b) {
        std::lock_guard<std::mutex> m1(a.dlmut_);
        int x;
        for (int i = 0; i < 100'000; ++i) x = i;
        std::lock_guard<std::mutex> m2(b.dlmut_);

    };

    std::thread t1(f, std::ref(x), std::ref(y)); 
    std::thread t2(f, std::ref(y), std::ref(x)); 
    t1.join();
    t2.join();

    // std::tread

}

//----------------------------------
// std::lock
struct DL {
    std::mutex dlmut_;
} 
x, y;

int main() {

    auto f = [](DL& a, DL& b) {
        std::lock(a.dlmut_, b.dlmut_); // атомарно блочит все мьютексы
        std::lock_guard<std::mutex> m1(a.dlmut_, std::adopt_lock);  
        // std::adopt_lock - просто разлочит в деструкторе
        int x;
        for (int i = 0; i < 100'000; ++i) x = i;
        std::lock_guard<std::mutex> m2(b.dlmut_, std::adopt_lock);

    };

    std::thread t1(f, std::ref(x), std::ref(y)); 
    std::thread t2(f, std::ref(y), std::ref(x)); 
    t1.join();
    t2.join();
}

// c++17 solution
struct DL {
    std::mutex dlmut_;
} 
x, y;

int main() {

    auto f = [](DL& a, DL& b) {
        std::scoped_lock sl{a.dlmut_, b.dlmut_}; // много вилок.
        // std::lock(a.dlmut_, b.dlmut_);
        // std::lock_guard<std::mutex> m1(a.dlmut_, std::adopt_lock);
        int x;
        for (int i = 0; i < 100'000; ++i) x = i;
        // std::lock_guard<std::mutex> m2(b.dlmut_, std::adopt_lock);

    };

    std::thread t1(f, std::ref(x), std::ref(y)); 
    std::thread t2(f, std::ref(y), std::ref(x)); 
    t1.join();
    t2.join();
}