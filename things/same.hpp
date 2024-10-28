#ifndef SAME_HPP
#define SAME_HPP

#include <type_traits>

namespace msa {

template <class T, class... Ts>
constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

template <class... T>
struct are_same : std::bool_constant<are_same_v<T...>> { };

} // namespace issametypes


#endif // IS_SAME_TYPES_HPP
