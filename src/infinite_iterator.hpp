#ifndef INFINITE_ITERATOR_HPP
#define INFINITE_ITERATOR_HPP

#include <utility>  // std::move()

template<class Incrementable = int>
class infinite_iterator
{
 public:
  /**
   * @brief Initialize an infinite iterator with the value of start
   *
   * @param start
   */
  infinite_iterator(const Incrementable start = 0)
      : current{start}
  {
  }

  /**
   * @brief Prefix increment
   *
   * @return
   */
  infinite_iterator<Incrementable>& operator++()
  {
    ++current;
    return *this;
  }

  /**
   * @brief Prefix Decrement
   *
   * @return
   */
  infinite_iterator<Incrementable>& operator--()
  {
    --current;
    return *this;
  }

  /**
   * @brief Postfix increment
   *
   * @param int
   *
   * @return
   */
  [[nodiscard]] infinite_iterator<Incrementable> operator++(int)
  {
    auto temp = *this;
         operator++();
    return temp;
  }

  /**
   * @brief Postfix Decrement
   *
   * @param int
   *
   * @return
   */
  [[nodiscard]] infinite_iterator<Incrementable> operator--(int)
  {
    auto temp = *this;
         operator--();
    return temp;
  }

  /**
   * @brief Increment idx times
   *
   * @param idx
   *
   * @return
   */
  infinite_iterator<Incrementable>& operator+=(const Incrementable idx)
  {
    current += idx;
    return *this;
  }

  /**
   * @brief Decrement idx times
   *
   * @param idx
   *
   * @return
   */
  infinite_iterator<Incrementable>& operator-=(const Incrementable idx)
  {
    current -= idx;
    return *this;
  }

  /**
   * @brief dereference operator
   *
   * @return
   */
  [[nodiscard]] Incrementable operator*() const noexcept { return current; }
  [[nodiscard]] Incrementable operator*() noexcept { return current; }

  /**
   * @brief dereference operator
   *
   * @return
   */
  [[nodiscard]] Incrementable* operator->() const noexcept { return &current; }
  [[nodiscard]] Incrementable* operator->() noexcept { return &current; }

  /**
   * @brief casting operator
   *
   * @return
   */
  [[nodiscard]] explicit operator Incrementable() const noexcept
  {
    return current;
  }
  [[nodiscard]] explicit operator Incrementable() noexcept { return current; }

  /**
   * @brief Copy Constructor
   *
   * @param copy
   */
  infinite_iterator(const infinite_iterator<Incrementable>& copy)
      : current{copy.current}
  {
  }

  /**
   * @brief Move constructor
   *
   * @param move
   */
  infinite_iterator(infinite_iterator<Incrementable>&& move)
      : current{std::move(move.current)}
  {
  }

  /**
   * @brief Copy assignment operator
   *
   * @param copy
   *
   * @return
   */
  infinite_iterator<Incrementable>& operator=(
      const infinite_iterator<Incrementable>& copy) noexcept
  {
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
  infinite_iterator<Incrementable>& operator=(
      infinite_iterator<Incrementable>&& move)
  {
    current = std::move(move.current);
    return *this;
  }

 private:
  Incrementable current;
};
#endif  // ! INFINITE_ITERATOR_HPP
