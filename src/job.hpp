#ifndef JOB_HPP
#define JOB_HPP

#include <iostream>
#include <optional>
#include <string>

// TODO: Move implementation to cpp

/*
 * Type declarations for more verbose constructors.
 * Example:
 * auto new_job = Job(NProcs{4}, NTicks{256}, "echo \"Hello world\";")
 */
using JobId = std::optional<unsigned int>;
using NProcs = unsigned int;
using NTicks = unsigned int;
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
  Job(const NProcs n_procs, const NTicks n_ticks, const JobId id = {},
      const Description &desc = {})
      : n_procs{n_procs}, n_ticks{n_ticks}, id{id}, desc{desc} {}

  /**
   * @brief Assign ID and/or description to an existing job
   *
   * @param id
   * @param desc
   * @param other
   */
  Job(JobId id, Description &desc, Job &&other) {
    *this = std::move(other);
    if (id) {
      this->id = *id;
    }
    if (desc) {
      this->desc = *desc;
    }
  }

  /**
   * @brief Copy constructor
   *
   * @param copy
   */
  Job(const Job &copy)
      : n_procs{copy.n_procs}, n_ticks{copy.n_ticks}, id{copy.id},
        desc{copy.desc} {}

  /**
   * @brief Move constructor
   *
   * @param other
   */
  Job(Job &&other)
      : n_procs{std::move(other.n_procs)}, n_ticks{std::move(other.n_ticks)},
        id{std::move(other.id)}, desc{std::move(other.desc)} {}

  /**
   * @brief Copy operator
   *
   * @param copy
   *
   * @return
   */
  Job &operator=(const Job &copy) noexcept {
    n_procs = copy.n_procs;
    n_ticks = copy.n_ticks;
    desc = copy.desc;
    id = copy.id;
    return *this;
  }

  /**
   * @brief Move operator
   *
   * @param copy
   *
   * @return
   */
  Job &operator=(Job &&copy) {
    *this = copy;
    copy.n_procs = 0;
    copy.n_ticks = 0;
    copy.desc = {};
    copy.id = {};
    return *this;
  }

  /**
   * @brief Print to ostream operator
   *
   * @param stream
   * @param job
   *
   * @return
   */
  friend std::ostream &operator<<(std::ostream &stream, const Job &job) {
    stream << "Job";
    if (job.id) {
      stream << *job.id;
    } else {
      stream << "(U)";
    }
    stream << "{ NProcs{" << job.n_procs << "}, NTicks{" << job.n_ticks << "}";
    if (job.desc) {
      stream << ", Description{" << *job.desc << "}";
    }
    return stream << " }";
  }

  /**
   * @brief Edit the description of a command IF it doesn't exist already
   */
  void promptDescription() {
    if (!desc)
      return;
    std::cout << "Please enter a description for Job";
    if (id)
      std::cout << *id;
    else
      std::cout << "(U)";
    std::cout << ": ";

    std::string new_desc;
    std::getline(std::cin, new_desc);
    desc = std::move(new_desc);
  }

private:
  NProcs n_procs;
  NTicks n_ticks;
  JobId id{};
  Description desc{};
};

#endif // ! JOB_HPP
