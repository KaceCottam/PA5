#ifndef JOB_SCHEDULER_HPP
#define JOB_SCHEDULER_HPP

#include "infinite_iterator.hpp"
#include "job.hpp"

#include <vector>

struct SchedulerException : std::logic_error {
public:
  SchedulerException(const char *message) : logic_error{message} {}

private:
};

class JobScheduler {
public:
  [[nodiscard]] bool is_running() noexcept;

  void tick();

  void insert_job(const Job &j);

private:
  infinite_iterator<> job_counter{1};
};
#endif // ! JOB_SCHEDULER_HPP
