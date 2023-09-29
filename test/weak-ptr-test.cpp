#include "shared-ptr.h"
#include "test-object.h"

#include <gtest/gtest.h>

class weak_ptr_test : public ::testing::Test {
protected:
  test_object::no_new_instances_guard instances_guard;
};

TEST_F(weak_ptr_test, weak_ptr_lock) {
  shared_ptr<test_object> p(new test_object(42));
  weak_ptr<test_object> q = p;
  shared_ptr<test_object> r = q.lock();
  EXPECT_TRUE(r == p);
  EXPECT_EQ(42, *r);
}

TEST_F(weak_ptr_test, weak_ptr_lock_nullptr) {
  shared_ptr<test_object> p(new test_object(42));
  weak_ptr<test_object> q = p;
  p.reset();
  instances_guard.expect_no_instances();
  shared_ptr<test_object> r = q.lock();
  EXPECT_FALSE(static_cast<bool>(r));
}

TEST_F(weak_ptr_test, weak_ptr_lock_nullptr_2) {
  weak_ptr<test_object> q;
  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(weak_ptr_test, make_shared) {
  shared_ptr<test_object> p = make_shared<test_object>(42);
  EXPECT_EQ(42, *p);
}

TEST_F(weak_ptr_test, make_shared_weak_ptr) {
  weak_ptr<test_object> p;
  {
    shared_ptr<test_object> q = make_shared<test_object>(42);
    p = q;
  }
  instances_guard.expect_no_instances();
}

TEST_F(weak_ptr_test, weak_ptr_copy_ctor) {
  shared_ptr<test_object> p(new test_object(42));
  weak_ptr<test_object> q = p;
  weak_ptr<test_object> r = q;
  EXPECT_TRUE(r.lock() == p);
  EXPECT_TRUE(q.lock() == p);
}

TEST_F(weak_ptr_test, weak_ptr_copy_ctor_nullptr) {
  weak_ptr<test_object> p;
  weak_ptr<test_object> q = p;
  EXPECT_FALSE(static_cast<bool>(p.lock()));
  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_move_ctor) {
  shared_ptr<test_object> p(new test_object(42));
  weak_ptr<test_object> q = p;
  weak_ptr<test_object> r = std::move(q);
  EXPECT_TRUE(r.lock() == p);
  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_move_ctor_nullptr) {
  weak_ptr<test_object> p;
  weak_ptr<test_object> q = p;
  EXPECT_FALSE(static_cast<bool>(p.lock()));
  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_assignment_operator) {
  shared_ptr<test_object> p1(new test_object(42));
  weak_ptr<test_object> q1 = p1;
  shared_ptr<test_object> p2(new test_object(43));
  weak_ptr<test_object> q2 = p2;

  q1 = q2;

  EXPECT_TRUE(q1.lock() == p2);
  EXPECT_TRUE(q2.lock() == p2);
}

TEST_F(weak_ptr_test, weak_ptr_assignment_operator_from_nullptr) {
  shared_ptr<test_object> p1(new test_object(42));
  weak_ptr<test_object> q1 = p1;
  weak_ptr<test_object> q2;

  q1 = q2;

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_assignment_operator_to_nullptr) {
  shared_ptr<test_object> p(new test_object(42));
  weak_ptr<test_object> q1;
  weak_ptr<test_object> q2 = p;

  q1 = q2;

  EXPECT_TRUE(q1.lock() == p);
  EXPECT_TRUE(q2.lock() == p);
}

TEST_F(weak_ptr_test, weak_ptr_assignment_operator_nullptr) {
  weak_ptr<test_object> q1;
  weak_ptr<test_object> q2;

  q1 = q2;

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_assignment_operator_self) {
  shared_ptr<test_object> p(new test_object(42));
  weak_ptr<test_object> q = p;

  q = q;

  EXPECT_TRUE(q.lock() == p);
}

TEST_F(weak_ptr_test, weak_ptr_assignment_operator_self_nullptr) {
  weak_ptr<test_object> q;

  q = q;

  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_assignment_operator_shared) {
  shared_ptr<test_object> p1(new test_object(42));
  shared_ptr<test_object> p2(new test_object(43));
  weak_ptr<test_object> q = p1;

  q = p2;

  EXPECT_TRUE(q.lock() == p2);
}

TEST_F(weak_ptr_test, weak_ptr_assignment_operator_shared_aliased) {
  test_object x(43);
  shared_ptr<test_object> p1(new test_object(42));
  shared_ptr<test_object> p2(p1, &x);
  weak_ptr<test_object> q = p1;

  q = p2;

  EXPECT_TRUE(q.lock() == p2);
}

TEST_F(weak_ptr_test, weak_ptr_move_assignment_operator) {
  shared_ptr<test_object> p1(new test_object(42));
  weak_ptr<test_object> q1 = p1;
  shared_ptr<test_object> p2(new test_object(43));
  weak_ptr<test_object> q2 = p2;

  q1 = std::move(q2);

  EXPECT_TRUE(q1.lock() == p2);
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_move_assignment_operator_from_nullptr) {
  shared_ptr<test_object> p1(new test_object(42));
  weak_ptr<test_object> q1 = p1;
  weak_ptr<test_object> q2;

  q1 = std::move(q2);

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_move_assignment_operator_to_nullptr) {
  shared_ptr<test_object> p(new test_object(42));
  weak_ptr<test_object> q1;
  weak_ptr<test_object> q2 = p;

  q1 = std::move(q2);

  EXPECT_TRUE(q1.lock() == p);
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_move_assignment_operator_nullptr) {
  weak_ptr<test_object> q1;
  weak_ptr<test_object> q2;

  q1 = std::move(q2);

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(weak_ptr_test, weak_ptr_move_assignment_operator_self) {
  shared_ptr<test_object> p(new test_object(42));
  weak_ptr<test_object> q = p;

  q = std::move(q);

  EXPECT_TRUE(q.lock() == p);
}

TEST_F(weak_ptr_test, weak_ptr_move_assignment_operator_self_nullptr) {
  weak_ptr<test_object> q;

  q = std::move(q);

  EXPECT_FALSE(static_cast<bool>(q.lock()));
}
