#ifndef INFINITE_ITERATOR_HPP
#define INFINITE_ITERATOR_HPP

#include <utility> // std::move()

/**
 * @brief A smart iterator that allows for infinite iteration
 *
 * @tparam Incrementable must have ++ and -- operators.
 */
template <class Incrementable = int> class infinite_iterator {
public:
  /**
   * @brief constructor
   *
   * @param start value
   */
  infinite_iterator(const Incrementable start = 0) : current{start} {}

  /**
   * @brief prefix increment operator
   *
   * @return
   */
  infinite_iterator<Incrementable> &operator++() {
    ++current;
    return *this;
  }

  /**
   * @brief prefix decrement operator
   *
   * @return
   */
  infinite_iterator<Incrementable> &operator--() {
    --current;
    return *this;
  }

  /**
   * @brief postfix increment operator
   *
   * @param int
   *
   * @return
   */
  [[nodiscard]] infinite_iterator<Incrementable> operator++(int) {
    auto temp = *this;
    operator++();
    return temp;
  }

  /**
   * @brief postfix decrement operator
   *
   * @param int
   *
   * @return
   */
  [[nodiscard]] infinite_iterator<Incrementable> operator--(int) {
    auto temp = *this;
    operator--();
    return temp;
  }

  /**
   * @brief increment by x operator
   *
   * @param idx
   *
   * @return
   */
  infinite_iterator<Incrementable> &operator+=(const Incrementable idx) {
    current += idx;
    return *this;
  }

  /**
   * @brief decrement by x operator
   *
   * @param idx
   *
   * @return
   */
  infinite_iterator<Incrementable> &operator-=(const Incrementable idx) {
    current -= idx;
    return *this;
  }

  /**
   * @brief dereference operators: get current
   *
   * @return
   */
  [[nodiscard]] Incrementable operator*() const noexcept { return current; }
  [[nodiscard]] Incrementable operator*() noexcept { return current; }

  /**
   * @brief arrow operators: get current
   *
   * @return
   */
  [[nodiscard]] Incrementable *operator->() const noexcept { return &current; }
  [[nodiscard]] Incrementable *operator->() noexcept { return &current; }

  /**
   * @brief casting operators
   *
   * @return
   */
  [[nodiscard]] explicit operator Incrementable() const noexcept {
    return current;
  }
  [[nodiscard]] explicit operator Incrementable() noexcept { return current; }

  /**
   * @brief Copy constructor
   *
   * @param copy
   */
  infinite_iterator(const infinite_iterator<Incrementable> &copy)
      : current{copy.current} {}

  /**
   * @brief Move constructor
   *
   * @param move
   */
  infinite_iterator(infinite_iterator<Incrementable> &&move)
      : current{std::move(move.current)} {}

  /**
   * @brief Copy assignment operator
   *
   * @param copy
   *
   * @return
   */
  infinite_iterator<Incrementable> &
  operator=(const infinite_iterator<Incrementable> &copy) noexcept {
    current = copy.current;
    return *this;
  }

  /**
   * @brief Move assignment operator
   *
   * @param move
   *
   * @return
   */
  infinite_iterator<Incrementable> &
  operator=(infinite_iterator<Incrementable> &&move) {
    current = std::move(move.current);
    return *this;
  }

private:
  Incrementable current;
};
#endif // ! INFINITE_ITERATOR_HPP
