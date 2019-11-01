#ifndef JOB_HPP
#define JOB_HPP

#include <iostream>
#include <optional>
#include <string>

/*
 * Type declarations for more accessability
 */
using JobId = std::optional<unsigned int>;
using Description = std::optional<std::string>;

/**
 * @brief A job class, holds id (if assigned), number of processors required,
 *        number of ticks required, and a description (the command it runs)
 */
class Job {
public:
  /*
   * @brief Make a job that is possibly unassigned with or without a description
   *
   * @param n_procs
   * @param n_ticks
   * @param id
   * @param desc
   *
   * @return
   */
  Job(const unsigned int n_procs, const unsigned int n_ticks,
      const JobId id = {}, const Description &desc = {});

  /**
   * @brief Assign ID and/or description to an existing job
   *
   * @param id
   * @param desc
   * @param other
   */
  Job(const JobId id, const Description &desc, Job &&other);

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
  Job &operator=(Job &&copy);

  /**
   * @brief Print to ostream operator
   *
   * @param stream
   * @param job
   *
   * @return
   */
  friend std::ostream &operator<<(std::ostream &stream, const Job &job);

  /**
   * @brief Edit the description of a command IF it doesn't exist already
   *
   * @param ostream
   * @param istream
   *
   * @note parameters are as such in order to allow for testing
   */
  void promptDescription(std::ostream &ostream = std::cout,
                         std::istream &istream = std::cin);

private:
  unsigned int n_procs;
  unsigned int n_ticks;
  JobId id{};
  Description desc{};
};

#endif // ! JOB_HPP
