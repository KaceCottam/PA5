#ifndef JOB_HPP
#define JOB_HPP

#include <iostream>
#include <string>

/**
 * @brief A job class, holds id, number of processors required,
 *        number of ticks required, and a description (the command it runs)
 *
 *        A struct because everything should be visible by default
 */
class Job {
public:
  /*
   * @brief constructor
   *
   * @param id
   * @param n_procs
   * @param n_ticks
   * @param desc
   *
   * @return
   */
  Job(const unsigned int id, const unsigned int n_procs,
      const unsigned int n_ticks, const std::string &desc);

  /**
   * @brief Copy constructor
   *
   * @param copy
   */
  Job(const Job &copy);

  /**
   * @brief Move constructor
   *
   * @param other
   */
  Job(Job &&other);

  /**
   * @brief Copy operator
   *
   * @param copy
   *
   * @return
   */
  Job &operator=(const Job &copy) noexcept;

  /**
   * @brief Move operator
   *
   * @param copy
   *
   * @return
   */
  Job &operator=(Job &&move);

  /**
   * @brief Print to ostream operator
   *
   * @param stream
   * @param job
   *
   * @return
   */
  friend std::ostream &operator<<(std::ostream &stream, const Job &job);

  /*
   * Comparison operators
   */
  friend bool operator<(const Job &lhs, const Job &rhs) noexcept;
  friend bool operator>(const Job &lhs, const Job &rhs) noexcept;
  friend bool operator==(const Job &lhs, const Job &rhs) noexcept;
  friend bool operator>=(const Job &lhs, const Job &rhs) noexcept;
  friend bool operator<=(const Job &lhs, const Job &rhs) noexcept;

  unsigned int get_n_procs() const noexcept;
  unsigned int n_ticks;

private:
  unsigned int id;
  unsigned int n_procs;
  std::string desc;
};

#endif // ! JOB_HPP
