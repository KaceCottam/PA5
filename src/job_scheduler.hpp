#ifndef JOB_SCHEDULER_HPP
#define JOB_SCHEDULER_HPP

#include "infinite_iterator.hpp"
#include "job.hpp"

#include <cassert>
#include <iostream>
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
using MinHeap =
    std::priority_queue<T, std::vector<T>,
                        std::greater<typename std::vector<T>::value_type>>;

class JobScheduler {
public:
  void set_target(std::istream &target) noexcept { this->target = &target; }

  [[nodiscard]] bool is_running() noexcept;

  [[nodiscard]] optional<SchedulerException> tick() noexcept;

  JobScheduler(unsigned int num_processors) : total_processors{num_processors} {
    assert(num_processors > 0);
  }

  [[nodiscard]] optional<SchedulerException>
  insert_job(const unsigned int n_procs, const unsigned int n_ticks,
             const std::string &desc) noexcept;

  [[nodiscard]] optional<SchedulerException>
  insert_job(std::istream &target) noexcept;

private:
  [[nodiscard]] std::istream &get_target() const noexcept {
    assert(target != nullptr);
    return *target;
  }

  bool set_available_processors(const int increment) noexcept {
    if (available_processors + increment < 0)
      return false;
    available_processors += increment;
    return true;
  }

  [[nodiscard]] unsigned int get_available_processors() const noexcept {
    return available_processors;
  }

  unsigned int total_processors;
  unsigned int available_processors{total_processors};
  std::vector<Job> running_jobs{}; // currently running
  MinHeap<Job> job_queue{};        // waiting to run
  std::istream *target{};
  infinite_iterator<unsigned int> job_counter{1};
};
#endif // ! JOB_SCHEDULER_HPP
