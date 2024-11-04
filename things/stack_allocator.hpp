#include <iostream>
#include <utility>


// declaration
namespace mvp {

template <class T>
class StackAllocator {

public:
    StackAllocator() = default;
    ~StackAllocator();

public:
    T* allocate(size_t count);
    void deallocate(T*, size_t count);

    template <class... Args>
    void construct(T* obj, Args&&... args);
    void destroy(T* obj);

private:
    T* top_ = new T[100];

};

} // namespace mvp

// difinition
namespace mvp {

template <class T>
StackAllocator<T>::~StackAllocator() {
    delete[] top_;
}

template <class T>
T* StackAllocator<T>::allocate(size_t count) {
    return std::exchange(top_, top_ + count);
} 
template <class T>
void StackAllocator<T>::deallocate(T*, size_t count) {
    top_ -= count;
}

template <class T>
template <class... Args>
void StackAllocator<T>::construct(T* obj, Args&&... args) {
    new (obj) T(std::forward<Args>(args)...);
}

template <class T>
void StackAllocator<T>::destroy(T* obj) { obj->~T(); }

} // namespace mvp

// int main() {
//     using namespace mvp;

//     StackAllocator<int> alloc;

//     int* p = alloc.allocate(100);

//     int* cur = p;
//     for (int i = 1; i < 101; ++i) {
//         alloc.construct(cur++, i);
//     }
    
//     cur = p;
//     for (int i = 0; i < 100; ++i) { alloc.destroy(cur++); }
//     alloc.deallocate(p, 100);
// }   