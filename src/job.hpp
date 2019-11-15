#ifndef JOB_HPP
#define JOB_HPP

#include <iostream>
#include <string>

class Job
{
 public:
  /**
   * @brief Constructs a job
   *
   * @param id
   * @param n_procs
   * @param n_ticks
   * @param desc
   */
  Job(const std::size_t  id,
      const std::size_t  n_procs,
      const std::size_t  n_ticks,
      const std::string& desc);

  /**
   * @brief Copy constructor
   *
   * @param copy
   */
  Job(const Job& copy);

  /**
   * @brief Move constructor
   *
   * @param move
   */
  Job(Job&& move);

  /**
   * @brief Copy assignment operator
   *
   * @param copy
   *
   * @return
   */
  Job& operator=(const Job& copy) noexcept;

  /**
   * @brief Move assignment operator
   *
   * @param move
   *
   * @return
   */
  Job& operator=(Job&& move);

  /**
   * @brief Stream output handling
   *
   * @param stream
   * @param job
   *
   * @return
   */
  friend std::ostream& operator<<(std::ostream& stream, const Job& job);

  /**
   * @brief Comparison operators for n_procs
   *
   * @param lhs
   * @param rhs
   *
   * @return
   */
  friend bool operator<(const Job& lhs, const Job& rhs) noexcept;
  friend bool operator>(const Job& lhs, const Job& rhs) noexcept;
  friend bool operator==(const Job& lhs, const Job& rhs) noexcept;
  friend bool operator>=(const Job& lhs, const Job& rhs) noexcept;
  friend bool operator<=(const Job& lhs, const Job& rhs) noexcept;

  /**
   * @brief Getter for n_procs
   *
   * @return
   */
  [[nodiscard]] std::size_t get_n_procs() const noexcept;

  std::size_t n_ticks;

 private:
  std::size_t id;
  std::size_t n_procs;
  std::string desc;
};

#endif  // ! JOB_HPP
