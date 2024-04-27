/**
 * @file array.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __FZ_ARRAY_H__
#define __FZ_ARRAY_H__

#include <initializer_list>
#include <numeric>
#include <ranges>
#include <vector>

namespace fz {

#ifdef __FZ_ARRAY_HETERO_COMPUTATION__
#define __FZ_ARRAY_DUAL__ __HOST__ __DEVICE__
#else
#define __FZ_ARRAY_DUAL__
#endif

using SizeType = std::size_t;

template <typename T>
__FZ_ARRAY_DUAL__ concept Range = requires(T t) {
  std::ranges::begin(t);
  std::ranges::end(t);
  { t.size() } -> std::convertible_to<SizeType>;
};

// support for array-like objects
// operations: size, shape, strides, (), []
template <typename T>
__FZ_ARRAY_DUAL__ concept ArrayConcept = requires(T t) {
  { t.shape() } -> Range;
  { t.strides() } -> Range;
  { t(0) } -> std::convertible_to<T &>;
  { t[0] } -> std::convertible_to<T &>;
};

template <typename T, Range Shape = std::vector<SizeType>,
          Range Stride = std::vector<SizeType>>
class Array {
 public:
  static auto empty(Shape shape) -> Array;

  static auto zeros(Shape shape) -> Array;

  static auto emptyLike(const Array &arr) -> Array;

 public:
  Array() = default;

  Array(std::initializer_list<T> data);

  Array(const Array &other);

  auto operator=(const Array &other) -> Array &;

  Array(Array &&other) noexcept;

  auto operator=(Array &&other) noexcept -> Array &;

  ~Array();

 public:
  __FZ_ARRAY_DUAL__ [[nodiscard]] auto size() const -> SizeType;

  __FZ_ARRAY_DUAL__ auto shape() const -> Shape;

  __FZ_ARRAY_DUAL__ auto strides() const -> Stride;

  template <typename... Args>
  __FZ_ARRAY_DUAL__ auto operator()(Args &&...args) -> T &;

  template <typename... Args>
  __FZ_ARRAY_DUAL__ auto operator()(Args &&...args) const -> const T &;

  template <typename Arg>
  __FZ_ARRAY_DUAL__ auto operator[](Arg &&arg) -> T &;

  template <typename Arg>
  __FZ_ARRAY_DUAL__ auto operator[](Arg &&arg) const -> const T &;

  template <typename... Args>
  __FZ_ARRAY_DUAL__ auto at(Args &&...args) -> T &;

  template <typename... Args>
  __FZ_ARRAY_DUAL__ auto at(Args &&...args) const -> const T &;

  __FZ_ARRAY_DUAL__ auto begin() -> T *;

  __FZ_ARRAY_DUAL__ auto end() -> T *;

  // TODO(franzero): implement const_iterator

  // __FZ_ARRAY_DUAL__ auto begin() const -> const_iterator;

  // __FZ_ARRAY_DUAL__ auto end() const -> const_iterator;

  __FZ_ARRAY_DUAL__ auto resize(Shape shape) -> void;

  __FZ_ARRAY_DUAL__ auto reshape(Shape shape) -> void;

 private:
  Shape _shape{};
  Stride _strides{};
  // using Pointer = std::pointer_traits<T *>;
  using Pointer = T *;
  Pointer _begin{};
  Pointer _end{};

  template <SizeType dim>
  __FZ_ARRAY_DUAL__ static auto rawOffset(const Stride &strides) -> SizeType;

  template <SizeType dim, typename Arg, typename... Args>
  __FZ_ARRAY_DUAL__ static auto rawOffset(const Stride &strides, Arg &&arg,
                                          Args &&...args) -> SizeType;

  template <typename Arg, typename... Args>
  __FZ_ARRAY_DUAL__ static auto dataOffset(const Stride &stride, Arg &&arg,
                                           Args &&...args) -> SizeType;
};

template <typename T, Range Shape, Range Stride>
auto Array<T, Shape, Stride>::emptyLike(const Array &arr) -> Array {
  Array new_arr;
  new_arr._shape = arr._shape;
  new_arr._strides = arr._strides;
  new_arr._begin = new T[arr.size()];
  new_arr._end = new_arr._begin + arr.size();
  return new_arr;
}

template <typename T, Range Shape, Range Stride>
auto Array<T, Shape, Stride>::empty(Shape shape) -> Array {
  Array arr;
  arr._shape = shape;
  arr._strides = shape;
  std::reverse(arr._strides.begin(), arr._strides.end());
  arr._strides[0] = 1;
  for (SizeType i = 1; i < arr._strides.size(); ++i) {
    arr._strides[i] = arr._strides[i - 1] * arr._shape[i - 1];
  }
  auto size = static_cast<SizeType>(
      std::reduce(shape.begin(), shape.end(), 1, std::multiplies<>()));
  arr._begin = new T[size];
  arr._end = arr._begin + size;
  return arr;
}

template <typename T, Range Shape, Range Stride>
auto Array<T, Shape, Stride>::zeros(Shape shape) -> Array {
  Array arr;
  arr._shape = shape;
  arr._strides = shape;
  std::reverse(arr._strides.begin(), arr._strides.end());
  arr._strides[0] = 1;
  for (SizeType i = 1; i < arr._strides.size(); ++i) {
    arr._strides[i] = arr._strides[i - 1] * arr._shape[i - 1];
  }
  auto size = static_cast<SizeType>(
      std::reduce(shape.begin(), shape.end(), 1, std::multiplies<>()));
  arr._begin = new T[size];
  arr._end = arr._begin + size;
  return arr;
}

template <typename T, Range Shape, Range Stride>
Array<T, Shape, Stride>::Array(std::initializer_list<T> data) {
  _begin = new T[data.size()];
  _end = _begin + data.size();
  std::copy(data.begin(), data.end(), _begin);
  _shape = {data.size()};
  _strides = {1};
}

template <typename T, Range Shape, Range Stride>
Array<T, Shape, Stride>::Array(const Array &other) {
  _shape = other._shape;
  _strides = other._strides;
  _begin = new T[other.size()];
  _end = _begin + other.size();
  std::copy(other._begin, other._end, _begin);
}

template <typename T, Range Shape, Range Stride>
Array<T, Shape, Stride>::Array(Array &&other) noexcept {
  _shape = std::move(other._shape);
  _strides = std::move(other._strides);
  _begin = std::move(other._begin);
  _end = std::move(other._end);
  other._begin = nullptr;
  other._end = nullptr;
}

template <typename T, Range Shape, Range Stride>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::operator=(
    const Array &other) -> Array & {
  if (this == &other) {
    return *this;
  }
  _shape = other._shape;
  _strides = other._strides;
  delete[] _begin;
  _begin = new T[other.size()];
  _end = _begin + other.size();
  std::copy(other._begin, other._end, _begin);
  return *this;
}

template <typename T, Range Shape, Range Stride>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::operator=(
    Array &&other) noexcept -> Array & {
  if (this == &other) {
    return *this;
  }
  _shape = std::move(other._shape);
  _strides = std::move(other._strides);
  delete[] _begin;
  _begin = std::move(other._begin);
  _end = std::move(other._end);
  other._begin = nullptr;
  other._end = nullptr;
  return *this;
}

template <typename T, Range Shape, Range Stride>
Array<T, Shape, Stride>::~Array() {
  delete[] _begin;
  _begin = nullptr;
  _end = nullptr;
}

template <typename T, Range Shape, Range Stride>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::size() const
    -> SizeType {
  return static_cast<SizeType>(_end - _begin);
}

template <typename T, Range Shape, Range Stride>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::shape() const -> Shape {
  return _shape;
}

template <typename T, Range Shape, Range Stride>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::strides() const
    -> Stride {
  return _strides;
}

template <typename T, Range Shape, Range Stride>
template <typename... Args>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::operator()(
    Args &&...args) -> T & {
  return _begin[dataOffset(_strides, std::forward<Args>(args)...)];
}

template <typename T, Range Shape, Range Stride>
template <typename... Args>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::operator()(
    Args &&...args) const -> const T & {
  return _begin[dataOffset(_strides, std::forward<Args>(args)...)];
}

template <typename T, Range Shape, Range Stride>
template <typename Arg>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::operator[](Arg &&arg)
    -> T & {
  return _begin[dataOffset(_strides, std::forward<Arg>(arg))];
}

template <typename T, Range Shape, Range Stride>
template <typename Arg>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::operator[](
    Arg &&arg) const -> const T & {
  return _begin[dataOffset(_strides, std::forward<Arg>(arg))];
}

template <typename T, Range Shape, Range Stride>
template <typename... Args>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::at(Args &&...args)
    -> T & {
  auto &&offset = dataOffset(_strides, std::forward<Args>(args)...);
  if (size() <= offset) {
    throw std::out_of_range("Index out of range");
  }

  return _begin[offset];
}

template <typename T, Range Shape, Range Stride>
template <typename... Args>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::at(Args &&...args) const
    -> const T & {
  auto offset = dataOffset(_strides, std::forward<Args>(args)...);
  if (size() <= offset) {
    throw std::out_of_range("Index out of range");
  }

  return _begin[offset];
}

template <typename T, Range Shape, Range Stride>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::begin() -> T * {
  return _begin;
}

template <typename T, Range Shape, Range Stride>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::end() -> T * {
  return _begin + size();
}

template <typename T, Range Shape, Range Stride>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::resize(Shape shape)
    -> void {
  auto new_size = static_cast<SizeType>(
      std::reduce(shape.begin(), shape.end(), 1, std::multiplies<>()));
  if (new_size != size()) {
    delete[] _begin;
    _begin = new T[new_size];
    _end = _begin + new_size;
  }

  _shape = shape;
  _strides = shape;
  std::reverse(_strides.begin(), _strides.end());
  _strides[0] = 1;
  for (SizeType i = 1; i < _strides.size(); ++i) {
    _strides[i] = _strides[i - 1] * _shape[i - 1];
  }
}

template <typename T, Range Shape, Range Stride>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::reshape(Shape shape)
    -> void {
  auto new_size = static_cast<SizeType>(
      std::reduce(shape.begin(), shape.end(), 1, std::multiplies<>()));
  if (new_size != size()) {
    throw std::invalid_argument("Invalid shape");
  }

  _shape = shape;
  _strides = shape;
  std::reverse(_strides.begin(), _strides.end());
  _strides[0] = 1;
  for (SizeType i = 1; i < _strides.size(); ++i) {
    _strides[i] = _strides[i - 1] * _shape[i - 1];
  }
}

template <typename T, Range Shape, Range Stride>
template <typename Arg, typename... Args>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::dataOffset(
    const Stride &stride, Arg &&arg, Args &&...args) -> SizeType {
  constexpr std::size_t nargs = sizeof...(Args) + 1;
  auto stride_size = static_cast<SizeType>(stride.size());
  if (nargs == stride.size()) {
    return rawOffset<static_cast<SizeType>(0)>(stride, std::forward<Arg>(arg),
                                               std::forward<Args>(args)...);
  }

  throw std::invalid_argument("Invalid number of arguments");
}

template <typename T, Range Shape, Range Stride>
template <SizeType dim>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::rawOffset(
    const Stride &strides) -> SizeType {
  return 0;
}

template <typename T, Range Shape, Range Stride>
template <SizeType dim, typename Arg, typename... Args>
__FZ_ARRAY_DUAL__ inline auto Array<T, Shape, Stride>::rawOffset(
    const Stride &strides, Arg &&arg, Args &&...args) -> SizeType {
  return static_cast<std::ptrdiff_t>(arg) * strides[dim] +
         rawOffset<dim + 1>(strides, std::forward<Args>(args)...);
}

}  // namespace fz

#endif  // __FZ_ARRAY_H__
