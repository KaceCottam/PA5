#ifndef JOB_SCHEDULER_HPP
#define JOB_SCHEDULER_HPP

#include <iostream>  // std::istream
#include <memory>    // std::shared_prt<>
#include <optional>  // std::optional<>
#include <queue>     // std::priority_queue<>
#include <variant>   // std::variant<>

#include "infinite_iterator.hpp"
#include "job.hpp"

using std::cout;
using std::endl;

struct SchedulerException : std::runtime_error
{
  SchedulerException(const std::string& arg);
};

using std::optional;
using index = std::size_t;

class JobScheduler
{
 public:
  JobScheduler(std::istream& target, std::size_t num_processors);

  void set_target(std::istream& target) noexcept;

  bool is_running() noexcept;

  optional<SchedulerException> tick() noexcept;

 private:
  template<class T>
  using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

  std::variant<SchedulerException, Job, std::nullopt_t> read_job(
      std::istream& target) noexcept;

  std::variant<SchedulerException, Job> create_job(
      const std::size_t  n_procs,
      const std::size_t  n_ticks,
      const std::string& desc) noexcept;

  void insert_job(Job new_job) noexcept;

  std::istream& get_target() const noexcept;

  std::size_t get_available_processors() const noexcept;

  void free_proc(const Job& j) noexcept;

  bool check_availability(const Job& j) noexcept;

  optional<Job> find_shortest() const noexcept;
  Job           pop_shortest() noexcept;
  void          run_job(Job new_job) noexcept;

  void decrement_timer() noexcept;

  std::istream*     target;                // pointer because of abstraction
  std::size_t       available_processors;  // all processors
  const std::size_t max_processors{available_processors};  // all processors
  MinHeap<Job>      job_queue{};                           // waiting to run
  std::vector<Job>  running_jobs{};                        // running jobs
  infinite_iterator<std::size_t> job_counter{1};
};
#endif  // ! JOB_SCHEDULER_HPP
