#ifndef __FZ_PARALLEL_PERFORMANCE_H__
#define __FZ_PARALLEL_PERFORMANCE_H__

#include <chrono>
#include <utility>

#include "fz/util/time.hpp"

namespace fz {

class Performance {
 public:
  class ExecutionProfile {
    using TimeDuration = decltype(std::chrono::high_resolution_clock::now() -
                                  std::chrono::high_resolution_clock::now());

    TimeDuration _duration{};

    [[nodiscard]] auto duration() const { return _duration; }

    template <typename Func, typename... Args>
    auto execute(Func&& func, Args&&... args) {
      auto res =
          measureTime(std::forward<Func>(func), std::forward<Args>(args)...);
      if constexpr (std::is_same_v<TimeDuration, decltype(res)>) {
        _duration = res;
      } else {
        _duration = std::get<0>(res);
        return std::get<1>(res);
      }
    }

    template <typename StringStream = std::stringstream,
              typename DurationCastType = std::chrono::milliseconds>
    auto toString(StringStream&& ss = {}) const -> std::string {
      auto time_ms = std::chrono::duration_cast<DurationCastType>(_duration);
      ss << "Elapsed Time: " << time_ms.count();

      if constexpr (std::is_same_v<DurationCastType,
                                   std::chrono::milliseconds>) {
        ss << " ms";
      } else if constexpr (std::is_same_v<DurationCastType,
                                          std::chrono::microseconds>) {
        ss << " us";
      } else if constexpr (std::is_same_v<DurationCastType,
                                          std::chrono::nanoseconds>) {
        ss << " ns";
      } else if constexpr (std::is_same_v<DurationCastType,
                                          std::chrono::seconds>) {
        ss << " s";
      } else if constexpr (std::is_same_v<DurationCastType,
                                          std::chrono::minutes>) {
        ss << " min";
      } else if constexpr (std::is_same_v<DurationCastType,
                                          std::chrono::hours>) {
        ss << " h";
      }

      return ss.str();
    }
  };

 public:
 private:
};

}  // namespace fz

#endif  // __FZ_PARALLEL_PERFORMANCE_H__
