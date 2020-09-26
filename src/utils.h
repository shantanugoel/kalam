#ifndef KALAM_SRC_UTILS_H_
#define KALAM_SRC_UTILS_H_

#include <type_traits>

namespace kalam {

template <typename T>
constexpr auto ToUnderlying(T e) noexcept {
  return static_cast<std::underlying_type_t<T>>(e);
}

}  // namespace kalam

#endif  // KALAM_SRC_UTILS_H_