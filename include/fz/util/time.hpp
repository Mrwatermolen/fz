#ifndef __FZ_UTIL_TIME_H__
#define __FZ_UTIL_TIME_H__

#include <chrono>
#include <utility>

namespace fz {

template <typename Func, typename... Args>
inline auto measureTime(Func func, Args&&... args) {
  using FuncReturnType = decltype(func(std::forward<Args>(args)...));
  using TimeDuration = decltype(std::chrono::high_resolution_clock::now() -
                                std::chrono::high_resolution_clock::now());

  const auto start = std::chrono::high_resolution_clock::now();
  if constexpr (std::is_same_v<void, FuncReturnType>) {
    func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return (end - start);
  } else {
    FuncReturnType ret = func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::tuple<TimeDuration, FuncReturnType>{(end - start), ret};
  }
}

}  // namespace fz

#endif  // __FZ_UTIL_TIME_H__
