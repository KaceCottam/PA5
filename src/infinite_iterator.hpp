#ifndef INFINITE_ITERATOR_HPP
#define INFINITE_ITERATOR_HPP

#include <cmath>
#include <iostream>

/**
 * @brief A smart iterator that allows for infinite iteration
 *
 * @tparam T
 */
template <class T = int> class infinite_iterator {
public:
  infinite_iterator(const T start = 0) : current{start} {}

  infinite_iterator<> &operator++() {
    ++current;
    return *this;
  }

  infinite_iterator<> &operator--() {
    --current;
    return *this;
  }

  [[nodiscard]] infinite_iterator operator++(int) {
    auto temp = *this;
    operator++();
    return temp;
  }

  [[nodiscard]] infinite_iterator operator--(int) {
    auto temp = *this;
    operator--();
    return temp;
  }

  infinite_iterator<> &operator+=(const int idx) {
    current += idx;
    return *this;
  }

  infinite_iterator<> &operator-=(const int idx) {
    current -= idx;
    return *this;
  }

  [[nodiscard]] T operator*() const noexcept { return current; }
  [[nodiscard]] T operator*() noexcept { return current; }

  [[nodiscard]] T *operator->() const noexcept { return &current; }
  [[nodiscard]] T *operator->() noexcept { return &current; }

  [[nodiscard]] explicit operator T() const noexcept { return current; }
  [[nodiscard]] explicit operator T() noexcept { return current; }

private:
  T current;
};
#endif // ! INFINITE_ITERATOR_HPP
