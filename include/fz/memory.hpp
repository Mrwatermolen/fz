#ifndef __FZ_MEMORY_H__
#define __FZ_MEMORY_H__

#include <exception>
#include <new>
#include <utility>

namespace fz {

// construct

template <typename T>
concept FzConstructible =
    requires(T x) { new (x) typename std::remove_reference_t<decltype(*x)>(); };

template <typename T>
concept FzConstructForwardIterator =
    requires(T x) { requires std::forward_iterator<T>; };

template <typename T>
concept FzConstructInputIterator =
    requires(T x) { requires std::input_iterator<T>; };

template <FzConstructible T, typename... Args>
inline auto construct(T* p, Args&&... args) -> void {
  // placement new
  new (p) T(std::forward<Args>(args)...);
}

template <typename T>
inline auto destroy(T* p) -> void {
  p->~T();
}

template <typename ForwardIterator>
inline auto destroy(ForwardIterator first, ForwardIterator last) -> void {
  constexpr auto has_trivial_destructor = std::is_trivially_destructible_v<
      typename std::iterator_traits<ForwardIterator>::value_type>;
  if constexpr (has_trivial_destructor) {
    return;
  } else {
    for (; first != last; ++first) {
      destroy(&*first);
    }
  }
}

template <FzConstructInputIterator InputIterator,
          FzConstructForwardIterator ForwardIterator>
inline auto uninitializedCopy(InputIterator first, InputIterator last,
                              ForwardIterator result) -> ForwardIterator {
  ForwardIterator cur = result;
  constexpr auto has_trivial_copy = std::is_trivially_copy_constructible_v<
      typename std::iterator_traits<InputIterator>::value_type>;
  if constexpr (has_trivial_copy) {
    for (; first != last; ++first, ++cur) {
      // is satisfied with commit or rollback?
      *cur = *first;
    }
    return result;
  } else {
    try {
      for (; first != last; ++first, ++cur) {
        construct(&*cur, *first);
      }
    } catch (const std::exception& e) {
      destroy(result, cur);
      throw e;
    }
    return cur;
  }
}

template <FzConstructForwardIterator ForwardIterator, FzConstructible T>
inline auto uninitializedFill(ForwardIterator first, ForwardIterator last,
                              const T& x) -> ForwardIterator {
  auto cur = first;
  constexpr auto has_trivial_copy = std::is_trivially_copy_constructible_v<T>;
  if constexpr (has_trivial_copy) {
    for (; cur != last; ++cur) {
      *cur = x;
    }
  } else {
    try {
      for (; cur != last; ++cur) {
        construct(&*cur, x);
      }
    } catch (const std::exception& e) {
      destroy(first, cur);
      throw e;
    }
  }
}

template <FzConstructForwardIterator ForwardIterator, typename Size,
          FzConstructForwardIterator T>
  requires std::is_integral_v<Size> &&
           std::is_same_v<
               T, typename std::iterator_traits<ForwardIterator>::value_type>
inline auto uninitializedFillN(ForwardIterator first, Size n, const T& x)
    -> ForwardIterator {
  auto cur = first;
  constexpr auto has_trivial_copy = std::is_trivially_copy_constructible_v<T>;
  if constexpr (has_trivial_copy) {
    for (; n > 0; --n, ++cur) {
      *cur = x;
    }
  } else {
    try {
      for (; n > 0; --n, ++cur) {
        construct(&*cur, x);
      }
    } catch (const std::exception& e) {
      destroy(first, cur);
      throw e;
    }
  }
}

// alloc

class MallocAlloc {
 public:
  using size_type = std::size_t;

 public:
  static auto allocate(size_type n) -> void*;

  static auto deallocate(void* p) -> void;

  static auto reallocate(void* p, size_type n) -> void*;

 private:
};

auto MallocAlloc::allocate(size_type n) -> void* {
  std::set_new_handler(nullptr);
  auto p = ::operator new(n);
  if (p == nullptr) {
    throw std::bad_alloc();
  }
  return p;
}

auto MallocAlloc::deallocate(void* p) -> void { ::operator delete(p); }

auto MallocAlloc::reallocate(void* p, size_type n) -> void* {
  if (p == nullptr) {
    return allocate(n);
  }

  auto new_p = std::realloc(p, n);
  if (new_p == nullptr) {
    throw std::bad_alloc();
  }
  return new_p;
}

}  // namespace fz

#endif  // __FZ_MEMORY_H__
