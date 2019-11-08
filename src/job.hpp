#ifndef JOB_HPP
#define JOB_HPP

#include <iostream>
#include <string>

class Job
{
 public:
  Job(const std::size_t  id,
      const std::size_t  n_procs,
      const std::size_t  n_ticks,
      const std::string& desc);

  Job(const Job& copy);

  Job(Job&& other);

  Job& operator=(const Job& copy) noexcept;

  Job& operator=(Job&& move);

  friend std::ostream& operator<<(std::ostream& stream, const Job& job);

  friend bool operator<(const Job& lhs, const Job& rhs) noexcept;
  friend bool operator>(const Job& lhs, const Job& rhs) noexcept;
  friend bool operator==(const Job& lhs, const Job& rhs) noexcept;
  friend bool operator>=(const Job& lhs, const Job& rhs) noexcept;
  friend bool operator<=(const Job& lhs, const Job& rhs) noexcept;

  [[nodiscard]] std::size_t get_n_procs() const noexcept;
  std::size_t               n_ticks;

 private:
  std::size_t id;
  std::size_t n_procs;
  std::string desc;
};

#endif  // ! JOB_HPP
