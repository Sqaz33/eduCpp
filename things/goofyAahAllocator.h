//----------------------------------------------------------------------------
#define OCUPIED true
#define UNOCUPIED false
#define OcupiedStatus bool 
#include

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

    template<class... Args> const noexcept(noexcept(T(std::forward<Args>(args)...)))
    // в зависимости от типа args это будет коставаться либо к && либо к &
    void construct(T* p, Args&& ... args) {
        new(p) T(std::forward<Args>(args)...);
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
