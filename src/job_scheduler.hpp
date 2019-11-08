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
using index = unsigned int;

class JobScheduler
{
 public:
  JobScheduler(std::istream& target, unsigned int num_processors);

  [[maybe_unused]] void set_target(std::istream& target) noexcept;

  [[nodiscard]] bool is_running() noexcept;

  [[nodiscard]] optional<SchedulerException> tick() noexcept;

 private:
  template<class T>
  using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

  [[nodiscard]] std::variant<SchedulerException, Job, std::nullopt_t> read_job(
      std::istream& target) noexcept;

  [[nodiscard]] std::variant<SchedulerException, Job> create_job(
      const unsigned int n_procs,
      const unsigned int n_ticks,
      const std::string& desc) noexcept;

  void insert_job(Job new_job) noexcept;

  [[nodiscard]] std::istream& get_target() const noexcept;

  [[nodiscard]] unsigned int get_available_processors() const noexcept;

  void free_proc(const Job& j) noexcept;

  [[nodiscard]] bool check_availability(const Job& j) noexcept;

  [[nodiscard]] optional<Job> find_shortest() const noexcept;
  [[nodiscard]] Job           pop_shortest() noexcept;
  void                        run_job(Job new_job) noexcept;

  void decrement_timer() noexcept;

  std::istream*      target;                // pointer because of abstraction
  unsigned int       available_processors;  // all processors
  const unsigned int max_processors{available_processors};  // all processors
  MinHeap<Job>       job_queue{};                           // waiting to run
  std::vector<Job>   running_jobs{};                        // running jobs
  infinite_iterator<unsigned int> job_counter{1};
};
#endif  // ! JOB_SCHEDULER_HPP
