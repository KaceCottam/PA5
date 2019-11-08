#ifndef INFINITE_ITERATOR_HPP
#define INFINITE_ITERATOR_HPP

#include <utility>  // std::move()

template<class Incrementable = int>
class infinite_iterator
{
 public:
  infinite_iterator(const Incrementable start = 0)
      : current{start}
  {
  }

  infinite_iterator<Incrementable>& operator++()
  {
    ++current;
    return *this;
  }

  infinite_iterator<Incrementable>& operator--()
  {
    --current;
    return *this;
  }

  infinite_iterator<Incrementable> operator++(int)
  {
    auto temp = *this;
         operator++();
    return temp;
  }

  infinite_iterator<Incrementable> operator--(int)
  {
    auto temp = *this;
         operator--();
    return temp;
  }

  infinite_iterator<Incrementable>& operator+=(const Incrementable idx)
  {
    current += idx;
    return *this;
  }

  infinite_iterator<Incrementable>& operator-=(const Incrementable idx)
  {
    current -= idx;
    return *this;
  }

  Incrementable operator*() const noexcept { return current; }
  Incrementable operator*() noexcept { return current; }

  Incrementable* operator->() const noexcept { return &current; }
  Incrementable* operator->() noexcept { return &current; }

  explicit operator Incrementable() const noexcept { return current; }
  explicit operator Incrementable() noexcept { return current; }

  infinite_iterator(const infinite_iterator<Incrementable>& copy)
      : current{copy.current}
  {
  }

  infinite_iterator(infinite_iterator<Incrementable>&& move)
      : current{std::move(move.current)}
  {
  }

  infinite_iterator<Incrementable>& operator=(
      const infinite_iterator<Incrementable>& copy) noexcept
  {
    current = copy.current;
    return *this;
  }

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
