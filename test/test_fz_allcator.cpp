#include <iostream>
#include <ranges>
#include <vector>

#include "fz/allocator.hpp"

auto main() -> int {
  auto ia = std::ranges::iota_view(1, 10);

  auto iv = std::vector<int, fz::Allocator<int>>{ia.begin(), ia.end()};

  for (auto&& i : iv) {
    std::cout << i << '\n';
  }

  return 0;
}