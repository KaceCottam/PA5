#include "job_scheduler.hpp"

#include <algorithm> // std::find_if()
#include <cassert>   // assert()
#include <iomanip>   // std::quoted()
#include <utility>   // std::forward()

template <class... Args>
SchedulerException::SchedulerException(Args &&... args)
    : runtime_error{std::forward<Args...>(args)...} {}

JobScheduler::JobScheduler(std::istream &target, unsigned int num_processors)
    : target{&target} {
  assert(num_processors > 0);
  processors.resize(num_processors);
}

[[maybe_unused]] void JobScheduler::set_target(std::istream &target) noexcept {
  this->target = &target;
}

[[nodiscard]] bool JobScheduler::is_running() noexcept {
  // if all processors dont have a job (=nullptr), nothing is running
  return std::count(processors.begin(), processors.end(), nullptr) ==
         (int)processors.size();
}

[[nodiscard]] optional<SchedulerException> JobScheduler::tick() noexcept {
  // prompt and insert new job
  if (auto e = insert_job(get_target()))
    return e;

  // decrement all running_jobs and free procs if necessary
  for (auto &i : running_jobs) {
    if (--i.n_ticks <= 0) // finished running
      free_proc(i);
  }

  // find next shortest job and see if we can start it
  do {
    auto available_procs = get_available_processors();
    auto new_assignment = job_queue.top();
    if (new_assignment.get_n_procs() <= available_procs.size()) {
      job_queue.pop();
      running_jobs.push_back(new_assignment);
      auto job = std::make_shared<Job>(std::move(new_assignment));
      for (auto i = 0U; i < new_assignment.get_n_procs(); ++i) {
        processors[available_procs[i]] = job;
      }
    } else
      break;
  } while (true);
  return {};
}

[[nodiscard]] optional<SchedulerException>
JobScheduler::insert_job(unsigned int n_procs, unsigned int n_ticks,
                         const std::string &desc) noexcept {
  // assume the input is not invalid
  assert(n_procs != 0);
  assert(n_ticks != 0);
  assert(desc != "NULL");

  // Check validity of the job
  if (n_procs > processors.size())
    return SchedulerException("Failed to Insert Job, job required more "
                              "processors than total processors.");

  Job j{static_cast<unsigned int>(job_counter++), n_procs, n_ticks, desc};
  job_queue.push(j);
  return {};

  // We do not say what to output, the rest of the program will do that based on
  // the exception or lack of exception
}

[[nodiscard]] optional<SchedulerException>
JobScheduler::insert_job(std::istream &target) noexcept {
  // in case end of stream
  if (target.eof())
    return {};

  std::string desc{};
  unsigned int n_procs, n_ticks;

  target >> std::quoted(desc) >> n_procs >> n_ticks;

  // skip to end of line
  target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  // we could add something to show the current job it is adding where the error
  // occurred. it would likely be done in the constructor for
  // SchedulerException.
  if (desc == "NULL")
    return SchedulerException("No job inserted: Desc is \"NULL\"");
  if (n_procs == 0)
    return SchedulerException("Failed to insert job, job needs > 0 processors");
  if (n_ticks == 0)
    return SchedulerException("Failed to insert job, job needs > 0 ticks");

  return insert_job(n_procs, n_ticks, desc);

  // We do not say what to output, the rest of the program will do that based on
  // the exception or lack of exception
}

[[nodiscard]] std::istream &JobScheduler::get_target() const noexcept {
  assert(target != nullptr);
  return *target;
}

[[nodiscard]] std::vector<index> JobScheduler::get_available_processors() const
    noexcept {
  std::vector<index> indexes;
  for (auto i = 0U; i < processors.size(); ++i) {
    if (!processors[i])
      indexes.push_back(i);
  }
  return indexes;
}

void JobScheduler::free_proc(const Job &j) noexcept {
  auto job_iter = std::find(running_jobs.begin(), running_jobs.end(), j);

  // assume j is in running_jobs
  assert(job_iter != running_jobs.end());
  // assume j is finished
  assert(j.n_ticks <= 0);

  for (auto i = 0U; i < j.get_n_procs(); i++) {
    *(std::find_if(processors.begin(), processors.end(),
                   [&j](auto i) { return i.get() == &j; })) = nullptr;
  }
  running_jobs.erase(job_iter);
}
