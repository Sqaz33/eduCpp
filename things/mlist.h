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
