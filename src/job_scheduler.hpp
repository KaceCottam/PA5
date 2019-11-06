#ifndef JOB_SCHEDULER_HPP
#define JOB_SCHEDULER_HPP

#include "infinite_iterator.hpp"
#include "job.hpp"

#include <iostream> // std::istream
#include <memory>   // std::shared_prt<>
#include <optional> // std::optional<>
#include <queue>    // std::priority_queue<>
#include <variant>  // std::variant<>

using std::cout;
using std::endl;

struct SchedulerException : std::runtime_error {
  template <class... Args> SchedulerException(Args &&... args);
};

using std::optional;
using index = unsigned int;

class JobScheduler {
public:
  JobScheduler(std::istream &target, unsigned int num_processors);

  [[maybe_unused]] void set_target(std::istream &target) noexcept;

  [[nodiscard]] bool is_running() noexcept;

  [[nodiscard]] optional<SchedulerException> tick() noexcept;

private:
  template <class T>
  using MinHeap =
      std::priority_queue<T, std::vector<T>,
                          std::greater<typename std::vector<T>::value_type>>;

  [[nodiscard]] std::variant<SchedulerException, Job>
  read_job(std::istream &target) noexcept;

  [[nodiscard]] std::variant<SchedulerException, Job>
  create_job(const unsigned int n_procs, const unsigned int n_ticks,
             const std::string &desc) noexcept;

  void insert_job(Job new_job) noexcept;

  [[nodiscard]] std::istream &get_target() const noexcept;

  [[nodiscard]] std::vector<index> get_available_processors() const noexcept;

  void free_proc(const std::shared_ptr<Job> &j) noexcept;

  [[nodiscard]] bool
  check_availability(const unsigned int procs_needed) noexcept;

  [[nodiscard]] optional<Job> find_shortest() const noexcept;
  [[nodiscard]] bool check_availability(int &procs_needed);
  [[nodiscard]] Job pop_shortest() noexcept;
  void run_job(Job new_job) noexcept;

  void decrement_timer() noexcept;

  std::istream *target;     // pointer because of abstraction
  MinHeap<Job> job_queue{}; // waiting to run
  std::vector<std::shared_ptr<Job>> processors{};   // all processors
  std::vector<std::shared_ptr<Job>> running_jobs{}; // running jobs
  infinite_iterator<unsigned int> job_counter{1};
};
#endif // ! JOB_SCHEDULER_HPP
