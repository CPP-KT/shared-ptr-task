#include "shared-ptr.h"
#include "test-object.h"

#include <gtest/gtest.h>

#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define DISABLE_ALLOCATION_TESTS 1
#endif
#endif

#ifndef DISABLE_ALLOCATION_TESTS
namespace {
std::size_t new_calls = 0;
std::size_t delete_calls = 0;
} // namespace

void* operator new(std::size_t count) {
  new_calls += 1;
  return std::malloc(count);
}

void operator delete(void* ptr) noexcept {
  delete_calls += 1;
  std::free(ptr);
}

void operator delete(void* ptr, std::size_t) noexcept {
  delete_calls += 1;
  std::free(ptr);
}

TEST(allocation_test, weak_ptr_allocations) {
  std::size_t new_calls_before = new_calls;
  std::size_t delete_calls_before = delete_calls;
  int* i_p = new int(1337);
  weak_ptr<int> w_p;
  {
    shared_ptr<int> s_p(i_p);
    w_p = s_p;
  }
  const auto new_calls_after = new_calls;
  const auto delete_calls_after = delete_calls;
  EXPECT_EQ(new_calls_after - new_calls_before, 2);
  EXPECT_EQ(delete_calls_after - delete_calls_before, 1);
  EXPECT_FALSE(w_p.lock());
}

TEST(allocation_test, make_shared_weak_ptr_allocations) {
  size_t new_calls_before = new_calls;
  size_t delete_calls_before = delete_calls;
  weak_ptr<int> w_p;
  {
    shared_ptr<int> s_p = make_shared<int>(42);
    w_p = s_p;
  }
  const auto new_calls_after = new_calls;
  const auto delete_calls_after = delete_calls;
  EXPECT_EQ(new_calls_after - new_calls_before, 1);
  EXPECT_EQ(delete_calls_after - delete_calls_before, 0);
  EXPECT_FALSE(w_p.lock());
}

TEST(allocation_test, allocations) {
  size_t new_calls_before = new_calls;
  size_t delete_calls_before = delete_calls;
  int* i_p = new int(1337);
  {
    shared_ptr<int> p(i_p);
    EXPECT_EQ(*i_p, *p);
  }
  const auto new_calls_after = new_calls;
  const auto delete_calls_after = delete_calls;
  EXPECT_EQ(new_calls_after - new_calls_before, 2);
  EXPECT_EQ(delete_calls_after - delete_calls_before, 2);
}

TEST(allocation_test, make_shared_allocations) {
  size_t new_calls_before = new_calls;
  size_t delete_calls_before = delete_calls;
  {
    shared_ptr<int> p = make_shared<int>(42);
    EXPECT_EQ(42, *p);
  }
  const auto new_calls_after = new_calls;
  const auto delete_calls_after = delete_calls;
  EXPECT_EQ(new_calls_after - new_calls_before, 1);
  EXPECT_EQ(delete_calls_after - delete_calls_before, 1);
}
#endif
