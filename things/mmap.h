#ifndef MMAP_H
#define MMAP_H

#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>

namespace mmap {

template<
    class Key,
    class T,
    class Allocator = std::allocator<std::pair<const Key, T>>
> 
class map {
public:
    // usingи
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;

    map() = default;
    
    map(std::initializer_list<value_type> list) {
        for (auto& val : list) {
            root = m_put(root, val);
        }
    } 

    ~map() {
        if (root == nullptr) {
            return;
        }
        m_deleteTree(root);
    }

    mapped_type& operator[](const key_type& key) {
        Node* g = m_get(root, key);
        if (g == nullptr) {
            root = m_put(root, value_type(key, mapped_type()));
            g = m_get(root, key);
        }
        return g->val.second;
    }

    const mapped_type& operator[](const key_type& key) const {
        Node* g = m_get(root, key);
        return g->val.second;
    }

    size_t size() const {
        return m_size(root);
    }

    void remove(const key_type& key) {
        root = m_delete(root, key);
    }

    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    friend class iterator;

    class iterator {
    private: 
        using Node = typename ::mmap::map<Key, T, Allocator>::Node;
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = value_type;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator(Node* it) :
            it(it)
        {}

        bool operator==(const iterator& other) const {
            return it == other.it;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        
        reference operator*() {
            return it->val;
        }

        pointer operator->() {
            return &(it->val);
        }        

        iterator& operator++() {
            it = incIterator(it, it);
            return *this;
        }

        iterator operator++(int) {
            iterator it = *this;
            ++(*this);
            return it;
        }

        iterator& operator--() {
            it = decIterator(it, it);
            return *this;
        }

        iterator operator--(int) {
            iterator it = *this;
            --(*this);
            return it;
        }


    private:
        Node* it;

        Node* incIterator(Node* cur, Node* prev) {
            if (cur == nullptr || cur->left == prev) { 
                return cur;
            }
            if (cur->right != nullptr && cur->right != prev) {
                return cur->right;
            } 
            return incIterator(cur->par, cur);
        }

        Node* decIterator(Node* cur, Node* prev) {
            if (cur == nullptr || cur->right == prev) { 
                return cur;
            }
            if (cur->left != nullptr && cur->left != prev) {
                return cur->left;
            } 
            return decIterator(cur->left, cur);
        }        
    };

    class const_iterator {
    private: 
        using Node = typename ::mmap::map<Key, T, Allocator>::Node;
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = value_type;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator(const Node* it) :
            it(it)
        {}

        bool operator==(const const_iterator& other) const {
            return it == other.it;
        }

        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
        
        reference operator*() {
            return it->val;
        }

        pointer operator->() {
            return &(it->val);
        }        

        const_iterator& operator++() {
            it = incIterator(it, it);
            return *this;
        }

        const_iterator operator++(int) {
            iterator it = *this;
            ++(*this);
            return it;
        }

        const_iterator& operator--() {
            it = decIterator(it, it);
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator it = *this;
            --(*this);
            return it;
        }


    private:
        const Node* it;

        const Node* incIterator(const Node* cur, const Node* prev) {
            if (cur == nullptr || cur->left == prev) { 
                return cur;
            }
            if (cur->right != nullptr && cur->right != prev) {
                return cur->right;
            } 
            return incIterator(cur->par, cur);
        }

        const Node* decIterator(const Node* cur, const Node* prev) {
            if (cur == nullptr || cur->right == prev) { 
                return cur;
            }
            if (cur->left != nullptr && cur->left != prev) {
                return cur->left;
            } 
            return decIterator(cur->left, cur);
        }        
    };


    iterator begin() {
        return iterator(m_min(root));
    }

    iterator end() {
        return iterator(nullptr);   
    }

    const_iterator begin() const {
        return const_iterator(m_min(root));
    }

    const_iterator end() const {
        return const_iterator(nullptr);
    }
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

private:
    // -----------------------------------------------
    // -----------------------------------------------
    struct Node {
        Node(
            const value_type& val,
            Node* par = nullptr,
            Node* left = nullptr,
            Node* right = nullptr
        ) :
            par(par),
            left(left),
            right(right),
            val(val)
        {}

        Node* par;
        Node* left;
        Node* right;
     
        value_type val;
    }
    * root = nullptr;
    

    void m_deleteTree(Node* x) {
        if (x->left != nullptr) {
            m_deleteTree(x->left);
        }
        if (x->right != nullptr) {
            m_deleteTree(x->right);
        }
        Traits::destroy(nalloc, x);
        Traits::deallocate(nalloc, x, 1);
    }

    // -----------------------------------------------
    // -----------------------------------------------
    
    void ostream_print() const {
        m_print(root);
    }

    void m_print(const Node* x) const {
        if (x == nullptr) {
            return;
        }
        std::cout << x->val.first << " => " << x->val.second << '\n';
        m_print(x->left);
        m_print(x->right);
    }

    using NodeAllocator = typename Allocator::template rebind<Node>::other;
    using Traits = std::allocator_traits<NodeAllocator>;
    NodeAllocator nalloc;

    size_t m_size(const Node* x) const {
        if (x == nullptr) {
            return 0;
        }
        return m_size(x->left) + m_size(x->right) + 1;
    }

    Node* m_get(Node* x, const key_type& key) {
        if (x == nullptr) {
            return nullptr;
        }
        if (key < x->val.first) {
            return m_get(x->left, key);
        } else if (key > x->val.first) {
            return m_get(x->right, key);
        } else {
            return x;
        }
    }

    const Node* m_get(Node* x, const key_type& key) const {
        if (x == nullptr) {
            return nullptr;
        }
        if (key < x->val.first) {
            return m_get(x->left, key);
        } else if (key > x->val.first) {
            return m_get(x->right, key);
        } else {
            return x;
        }
    }

    Node* m_put(Node* x, const value_type& val, Node* prev = nullptr) {
        if (x == nullptr) {
            Node* n = Traits::allocate(nalloc, 1);
            Traits::construct(nalloc, n, val, prev);
            return n;
        }
        if (val.first < x->val.first) {
            x->left = m_put(x->left, val, x);
        } else if (val.first > x->val.first) {
            x->right = m_put(x->right, val, x);
        } else {
            x->val.second = val.second;
        }
        return x;
    }
    
    Node* m_max(Node* x) {
        if (x == nullptr) {
            return nullptr;
        }
        if (x->right != nullptr) {
            return m_max(x->right);
        }
        return x;
    }

    Node* m_min(Node* x)  {
        if (x == nullptr) {
            return nullptr;
        }
        if (x->left != nullptr) {
            return m_min(x->left);
        }
        return x;
    }

    const Node* m_max(const Node* x) const {
        if (x == nullptr) {
            return nullptr;
        }
        if (x->right != nullptr) {
            return m_max(x->right);
        }
        return x;
    }

    const Node* m_min(const Node* x) const {
        if (x == nullptr) {
            return nullptr;
        }
        if (x->left != nullptr) {
            return m_min(x->left);
        }
        return x;
    }

    Node* m_deleteMin(Node* x) {
        if (x->left == nullptr) {
            if (x->right != nullptr) {
                x->right->par = x->par;
            }
            return x->right;
        }
        x->left = m_deleteMin(x->left);
        return x;
    }

    Node* m_delete(Node* x, const key_type& key) {
        if (x == nullptr) {
            return nullptr;
        }

        if (key < x->val.first) {
            x->left = m_delete(x->left, key);
        } else if (key > x->val.first) {
            x->right = m_delete(x->right, key);
        } else {
            if (x->left == nullptr) {
                if (x->right != nullptr) {
                    x->right->par = x->par;
                } 
                Node* right = x->right;
                Traits::destroy(nalloc, x);
                Traits::deallocate(nalloc, x, 1);
                return right;
            }
            if (x->right == nullptr) {
                if (x->left != nullptr) {
                    x->left->par = x->par;
                } 
                Node* left = x->left;
                Traits::destroy(nalloc, x);
                Traits::deallocate(nalloc, x, 1);
                return left;
            }
            Node* tmp = x;
            x = m_min(x->right);
            x->right = m_deleteMin(tmp->right);
            x->left = tmp->left;
            x->par = tmp->par;
            Traits::destroy(nalloc, tmp);
            Traits::deallocate(nalloc, tmp, 1);
        }
        return x;
    }

};

}

template<
    class Key,
    class T,
    class Allocator = std::allocator<std::pair<const Key, T>>
> 
std::ostream& operator<<(std::ostream& out, const mmap::map<Key, T, Allocator>& m ) {
    m.ostream_print();
    return out;
}

#endif // MMAP_H