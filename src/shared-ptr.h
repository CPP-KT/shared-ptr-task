#pragma once

#include <cstddef>

template <typename T>
class shared_ptr {
public:
  shared_ptr() noexcept;

  shared_ptr(std::nullptr_t) noexcept;

  template <typename Y>
  explicit shared_ptr(Y* ptr);

  template <typename Y, typename Deleter>
  shared_ptr(Y* ptr, Deleter deleter);

  template <typename Y>
  shared_ptr(const shared_ptr<Y>& other, T* ptr) noexcept;

  template <typename Y>
  shared_ptr(shared_ptr<Y>&& other, T* ptr) noexcept;

  shared_ptr(const shared_ptr& other) noexcept;

  template <typename Y>
  shared_ptr(const shared_ptr<Y>& other) noexcept;

  shared_ptr(shared_ptr&& other) noexcept;

  template <typename Y>
  shared_ptr(shared_ptr<Y>&& other) noexcept;

  shared_ptr& operator=(const shared_ptr& other) noexcept;

  template <typename Y>
  shared_ptr& operator=(const shared_ptr<Y>& other) noexcept;

  shared_ptr& operator=(shared_ptr&& other) noexcept;

  template <typename Y>
  shared_ptr& operator=(shared_ptr<Y>&& other) noexcept;

  T* get() const noexcept;

  operator bool() const noexcept;

  T& operator*() const noexcept;

  T* operator->() const noexcept;

  std::size_t use_count() const noexcept;

  void reset() noexcept;

  template <typename Y>
  void reset(Y* new_ptr);

  template <typename Y, typename Deleter>
  void reset(Y* new_ptr, Deleter deleter);

  friend bool operator==(const shared_ptr& lhs, const shared_ptr& rhs) noexcept;

  friend bool operator!=(const shared_ptr& lhs, const shared_ptr& rhs) noexcept;

private:
};

template <typename T>
class weak_ptr {
public:
  weak_ptr() noexcept;

  template <typename Y>
  weak_ptr(const shared_ptr<Y>& other) noexcept;

  weak_ptr(const weak_ptr& other) noexcept;

  template <typename Y>
  weak_ptr(const weak_ptr<Y>& other) noexcept;

  weak_ptr(weak_ptr&& other) noexcept;

  template <typename Y>
  weak_ptr(weak_ptr<Y>&& other) noexcept;

  template <typename Y>
  weak_ptr& operator=(const shared_ptr<Y>& other) noexcept;

  weak_ptr& operator=(const weak_ptr& other) noexcept;

  template <typename Y>
  weak_ptr& operator=(const weak_ptr<Y>& other) noexcept;

  weak_ptr& operator=(weak_ptr&& other) noexcept;

  template <typename Y>
  weak_ptr& operator=(weak_ptr<Y>&& other) noexcept;

  shared_ptr<T> lock() const noexcept;

  void reset() noexcept;

private:
};

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args);
