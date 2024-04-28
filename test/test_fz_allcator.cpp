#include <gtest/gtest.h>

#include <random>
#include <ranges>
#include <vector>

#include "fz/allocator.hpp"

TEST(Allocator, BasicBuildInType) {
  /*
   * Test the Allocator class with a build-in type
   Allocator needs to implement the following functions:
    - allocate
    - deallocate
    - destroy
   */

  using BuildInType = int;

  auto ia = std::ranges::iota_view(1, 4);
  auto res = std::vector<BuildInType>{ia.begin(), ia.end()};
  auto iv = std::vector<BuildInType, fz::Allocator<BuildInType>>{ia.begin(),
                                                                 ia.end()};

  EXPECT_EQ(iv.size(), 3);

  for (auto&& [i, j] : std::views::zip(iv, res)) {
    EXPECT_EQ(i, j);
  }

  iv.emplace_back(4);
  res.emplace_back(4);

  EXPECT_EQ(iv.size(), 4);

  for (auto&& [i, j] : std::views::zip(iv, res)) {
    EXPECT_EQ(i, j);
  }

  iv.resize(3);
  res.resize(3);

  EXPECT_EQ(iv.size(), 3);

  for (auto&& [i, j] : std::views::zip(iv, res)) {
    EXPECT_EQ(i, j);
  }

  iv.clear();
  res.clear();

  EXPECT_EQ(iv.size(), 0);

  std::default_random_engine gen;
  std::uniform_int_distribution<int> dist(0, 100);
  auto dice = [&gen, &dist]() { return dist(gen); };

  for (BuildInType i = 0; i < 2; ++i) {
    auto n = dice();
    iv.emplace_back(n);
    res.emplace_back(n);
  }

  EXPECT_EQ(iv.size(), 2);

  for (auto&& [i, j] : std::views::zip(iv, res)) {
    EXPECT_EQ(i, j);
  }

  iv.reserve(6);
  res.reserve(6);

  EXPECT_EQ(iv.size(), 2);

  for (BuildInType i = 0; i < 4; ++i) {
    auto n = dice();
    iv.emplace_back(n);
    res.emplace_back(n);
  }

  EXPECT_EQ(iv.size(), 6);

  for (auto&& [i, j] : std::views::zip(iv, res)) {
    EXPECT_EQ(i, j);
  }
}
