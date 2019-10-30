#ifndef JOB_SCHEDULER_HPP
#define JOB_SCHEDULER_HPP

#include "infinite_iterator.hpp"
#include "job.hpp"

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

private:
  infinite_iterator<unsigned int> job_counter{1};
};
#endif // ! JOB_SCHEDULER_HPP
