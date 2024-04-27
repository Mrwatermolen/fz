#include <iostream>
#include <numeric>
#include <ranges>

#include "fz/array.hpp"

auto main() -> int {
  using fz::Array;

  auto arr = Array<int>::empty({2, 3});
  std::iota(arr.begin(), arr.end(), 1);
  std::cout << arr(0, 0) << '\t';
  std::cout << arr(0, 1) << '\t';
  std::cout << arr(0, 2) << '\n';
  std::cout << arr(1, 0) << '\t';
  std::cout << arr(1, 1) << '\t';
  std::cout << arr(1, 2) << '\n';

  // test reshape
  arr.reshape({3, 2});
  std::cout << "Reshaped array:\n"
            << "(" << arr.shape()[0] << ", " << arr.shape()[1] << ")\n";
  std::cout << arr(0, 0) << '\t';
  std::cout << arr(0, 1) << '\t';
  std::cout << arr(1, 0) << '\n';
  std::cout << arr(1, 1) << '\t';
  std::cout << arr(2, 0) << '\t';
  std::cout << arr(2, 1) << '\n';

  try {
    std::cout << "Try to Reshaped array:\n"
              << "(" << arr.shape()[0] << ", " << arr.shape()[1] << ")\n";
    arr.reshape({3, 3});
  } catch (const std::exception& e) {
    std::cout << e.what() << '\n';
  }

  // test resize
  arr.resize({3, 3});
  std::cout << "Resized array:\n"
            << "(" << arr.shape()[0] << ", " << arr.shape()[1] << ")\n";
  std::iota(arr.begin(), arr.end(), 1);
  std::cout << arr(0, 0) << '\t';
  std::cout << arr(0, 1) << '\t';
  std::cout << arr(0, 2) << '\n';
  std::cout << arr(1, 0) << '\t';
  std::cout << arr(1, 1) << '\t';
  std::cout << arr(1, 2) << '\n';
  std::cout << arr(2, 0) << '\t';
  std::cout << arr(2, 1) << '\t';
  std::cout << arr(2, 2) << '\n';

  // test ranges
  arr = Array<int>::empty({3, 4, 5});
  std::iota(arr.begin(), arr.end(), 1);
  std::cout << "Test Ranges:\n";
  for (auto&& i :
       arr | std::views::drop(4) |
           std::views::filter([](const auto& v) { return v % 4 == 0; })) {
    std::cout << i << '\n';
  }
}
