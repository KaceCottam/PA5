#ifndef JOB_SCHEDULER_HPP
#define JOB_SCHEDULER_HPP

#include "infinite_iterator.hpp"
#include "job.hpp"

#include <cassert>
#include <optional>
#include <stdexcept>
#include <vector>

// TODO: move implementation to cpp

struct SchedulerException : std::runtime_error {
  template <class... Args>
  SchedulerException(Args &&... args) : runtime_error{std::forward(args)...} {}
};

class JobScheduler {
public:
  [[nodiscard]] bool is_running() noexcept;

  [[nodiscard]] std::optional<SchedulerException> tick() noexcept;

  [[nodiscard]] std::optional<SchedulerException>
  insert_job(const Job &j) noexcept;

  [[nodiscard]] std::optional<SchedulerException>
  insert_next_job(const Job &j) noexcept;

  JobScheduler(const std::vector<bool>::size_type i) {
    assert(i > 0);
    processors.reserve(i);
  }

private:
  std::vector<bool> get_available_processors() {
    std::vector<bool> available;
    for (std::vector<bool>::size_type i = 0; i < processors.size(); i++) {
      if (processors[i] == false) {
        available.emplace_back(i);
      }
    }
    return available;
  }

  std::vector<bool> processors;
  infinite_iterator<unsigned int> job_counter{1};
};
#endif // ! JOB_SCHEDULER_HPP
