#include <concepts>
#include <iostream>
#include <utility>
#include <memory>

#include "things/same.hpp"


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
