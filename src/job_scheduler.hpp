#ifndef JOB_SCHEDULER_HPP
#define JOB_SCHEDULER_HPP

#include "infinite_iterator.hpp"
#include "job.hpp"

#include <cassert>
#include <optional>
#include <queue>
#include <stdexcept>
#include <vector>

// TODO: move implementation to cpp

struct SchedulerException : std::runtime_error {
  template <class... Args>
  SchedulerException(Args &&... args) : runtime_error{std::forward(args)...} {}
};

using std::optional;
template <class T>
using MinHeap = std::priority_queue<T, std::vector<T>,
                                    std::greater<std::vector<T>::value_type>>;

class JobScheduler {
public:
  void set_target(std::istream &target) noexcept;

  [[nodiscard]] bool is_running() noexcept;

  [[nodiscard]] optional<SchedulerException> tick() noexcept;

  JobScheduler(unsigned int i) : available_processors{i} { assert(i > 0); }

  [[nodiscard]] optional<SchedulerException> insert_job(const Job &j) noexcept;

private:
  void set_available_processors(const int increment) noexcept {
    if (available_processors + increment < 0)
      return;
    available_processors += increment;
  }

  [[nodiscard]] unsigned int get_available_processors() const noexcept {
    return available_processors;
  }

  unsigned int available_processors;
  std::vector<Job> running_jobs;
  MinHeap<Job> job_queue;
  std::istream *target;
  infinite_iterator<unsigned int> job_counter{1};
};
#endif // ! JOB_SCHEDULER_HPP
