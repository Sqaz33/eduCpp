#ifndef THINGS_VARIANT_H
#define THINGS_VARIANT_H

namespace my {

template <class T, class... Types>
struct get_index_by_types {
    static const size_t value = -1;
};

template <class T, class Head, class... Tail>
struct get_index_by_type {
    static const size_t value = std::is_same_v<T, Head>
        ? 0 : get_index_by_type<T, Tail...>::value + 1;
};

template <class T, class... Types>
constexpr size_t get_index_by_type_v = get_index_by_type<T, Types...>::value;

// не работает
template <typename... Types>
class variant;

template <class T, class... Types>
struct VariantAlternative {
    VariantAlternative() {}
    ~VariantAlternative() {}

    using Derived = variant<Types...>;
    static const size_t Index = get_index_by_type_v<T, Types...>;

    VariantAlternative(const T& value) {
        auto this_ptr = static_cast<Derived*>(this);
        this_ptr->storage.template put<T>(value);
        this_ptr->index = Index;
    }
};

struct Empty {};
Empty empty;

template <typename... Types>
class variant : private VariantAlternative<Types, Types...>...{
public:

template <size_t Index, class T, class... Ts>
friend T& get(variant<Ts...>& v);

template <class T, class... Ts>
friend struct VariantAlternative<T, Ts...>;

using VariantAlternative<Types, Types...>::VariantAlternative...;

private:
    template <class...>
    union VariadicUnion { 
        template <size_t Index>
        auto& get() {
            return empty;
        }
    };

    template <class Head, class... Tail>
    union VariadicUnion<Head, Tail...> {
        Head head;
        VariadicUnion<Tail...> tail; 

        template <size_t Index> 
        auto& get() {
            if constexpr (Index == 0) {
                return head;
            } else {
                return tail.template get<Index-1>();
            }
        }

        template <typename T>
        void put(const T& value) {
            if constexpr (std::is_same_v<T, Head>) {
                new (&head) T(value);
            } else {
                tail.template put<T>(value);
            }
        }
    };
    

private:
    VariadicUnion<Types...> storage; 
    size_t index;
};


template <size_t Index, class T, class... Types>
T& get(variant<Types...>& v) {
    if (v.index != Index) {
        throw std::bad_variant_access("e");
    }
    return v.storage.template get<Index-1>();
}

template <class T, class... Types>
auto& get(variant<Types...>& v) {
    return get<get_index_by_type_v<T, Types...>>(v);
}
 

} // namespace my


#endif // THINGS_VARIANT_H