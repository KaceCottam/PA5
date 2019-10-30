#ifndef INFINITE_ITERATOR_HPP
#define INFINITE_ITERATOR_HPP

#include <iostream>

/**
 * @brief A smart iterator that allows for infinite iteration
 *
 * @tparam T
 */
template <class T = unsigned int> class infinite_iterator {
public:
  infinite_iterator(const T start = 0) : current{start} {}

  infinite_iterator &operator++() noexcept {
    ++current;
    return *this;
  }

  [[nodiscard]] infinite_iterator operator++(int) noexcept {
    auto temp = *this;
    operator++();
    return temp;
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
