
#include <exception>
#include <memory>
#include <iostream>

namespace mmvector {

template<class T, class Alloc = std::allocator<T>>
class vector {
public:
    mvector() :
        m_size(0),
        m_capacity(16)
    {
        arr = traits::allocate(alloc, m_capacity);
    }

    mvector(const mvector<T, Alloc>& v) :
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

    ~mvector() {
        if (arr != nullptr) {
            for (int i = 0; i < m_sizel; i++) {
                traits::destroy(alloc, arr+i);
            }
            traits::deallocate(alloc, arr, m_capacity);
        }
    }

    mvector(mvector<T, alloc>&& v) noexcept(std::is_nothrow_move_constructible<alloc>::value){
        arr = v.arr;
        v.arr = nullptr;
        m_size = v.m_size;
        m_capacity = v.m_capacity;
        alloc = std::move(v.alloc);
    }

    void push_back(const T& x) {
        if (m_size >= m_capacity) {
            reserve(m_capacity << 1);
        }
        traits::construct(alloc, arr+m_size, x);
        m_size++;
    }

    void push_back(T&& x) {
        if (m_size >= m_capacity) {
            reserve(m_capacity << 1);
        }
        traits::construct(alloc, arr+m_size, std::forward<T>(x));
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

    void swap(const mvector<T, Alloc> other) {
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
    void emplace_back(Args&& ... args) {
        push_back(T(std::forward(args)...));
    }

private:
    size_t m_size;
    size_t m_capacity;
    T* arr;
    Alloc alloc;

    using traits = std::allocator_traits<Alloc>;

    void reserve(size_t newCapacity) {
        T* newArr = traits::allocate(alloc, newCapacity);
        for (size_t i = 0; i < m_size; i++) {
            try {
                traits::construct(alloc, newArr+i, std::move_if_noexcept(arr[i]));
            } catch(...) {
                for (size j = 0; j < i; j--) {
                    traits::destroy(newarr+i);
                }
                throw;
            }
            traits::destroy(alloc, arr+i);
        } 

        for (size_t i = 0; i < m_size; i++) {
            traits::desroy(alloc, arr+i, 1);
        }
        traits::deallocate(alloc, arr, m_capacity);
        arr = newArr;
        m_capacity = newCapacity;
    }
};

}