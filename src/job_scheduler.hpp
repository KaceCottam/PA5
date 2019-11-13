#ifndef JOB_SCHEDULER_HPP
#define JOB_SCHEDULER_HPP

#include <iostream>  // std::istream
#include <memory>    // std::shared_prt<>
#include <optional>  // std::optional<>
#include <queue>     // std::priority_queue<>
#include <variant>   // std::variant<>

#include "job.hpp"

using std::cout;
using std::endl;

struct SchedulerException : std::runtime_error
{
  SchedulerException(const std::string& arg);
};

using std::optional;

class JobScheduler
{
 public:
  // constructor
  JobScheduler(std::istream& target, std::size_t num_processors);

  // sets target stream program is reading from
  [[maybe_unused]] void set_target(std::istream& target) noexcept;

  // returns if scheduler is running
  [[nodiscard]] bool is_running() noexcept;

  // performs actions necessary in each tick
  [[nodiscard]] optional<SchedulerException> tick() noexcept;

  // returns the number of available processors
  [[nodiscard]] std::size_t get_available_processors() const noexcept;

 private:
  template<class T>
  using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

  // get job from target stream
  [[nodiscard]] std::variant<SchedulerException, Job, std::nullopt_t> read_job(
      std::istream& target) noexcept;

  // makes a job object
  [[nodiscard]] std::variant<SchedulerException, Job> create_job(
      const std::size_t  n_procs,
      const std::size_t  n_ticks,
      const std::string& desc) noexcept;

  // inserts job into the priority queue (MinHeap) of waiting jobs
  void insert_job(Job new_job) noexcept;

  // gets target stream
  [[nodiscard]] std::istream& get_target() const noexcept;

  // puts processors back into the pool of available processors when job is
  // complete
  void free_proc(const Job& j) noexcept;

  // returns if job can be run with the current number of free processors
  [[nodiscard]] bool check_availability(const Job& j) noexcept;

  // finds the shortest waiting job (top)
  [[nodiscard]] optional<Job> find_shortest() const noexcept;

  // pops the shortest job off the priority queue
  [[nodiscard]] Job pop_shortest() noexcept;

  // moves job to the running queue
  void run_job(Job new_job) noexcept;

  // goes through the waiting jobs and decrements their time remaining
  void decrement_timer() noexcept;

  std::istream* target;                // pointer because of abstraction
  std::size_t   available_processors;  // number of available processors - using
                                       // std::size_t for unsigned int
  const std::size_t max_processors{available_processors};  // all processors
  MinHeap<Job>      job_queue{};                           // waiting to run
  std::vector<Job>  running_jobs{};                        // running jobs
  std::size_t       job_counter{1};                        // for job IDs
  bool exited{false};  // for telling the program to stop reading jobs
};
#endif  // ! JOB_SCHEDULER_HPP
