#ifndef JOB_HPP
#define JOB_HPP

#include <iostream>
#include <string>

class Job
{
public:
  Job(const unsigned int id,
      const unsigned int n_procs,
      const unsigned int n_ticks,
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

  [[nodiscard]] unsigned int get_n_procs() const noexcept;
  unsigned int n_ticks;

private:
  unsigned int id;
  unsigned int n_procs;
  std::string desc;
};

#endif // ! JOB_HPP
