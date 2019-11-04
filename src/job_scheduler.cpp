#include "job_scheduler.hpp"

#include <algorithm>  // std::find_if()
#include <cassert>    // assert()
#include <functional> // std::mem_fn
#include <iomanip>    // std::quoted()
#include <utility>    // std::forward()

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
  // we shall count all the processors with a value 'nullptr', if that is the
  // size of the vector of processors, none of the processors have a job.
  return std::count_if(processors.begin(), processors.end(), [](auto i) {
           return i.get() == nullptr;
         }) != (int)processors.size();
}

[[nodiscard]] optional<SchedulerException> JobScheduler::tick() noexcept {
  // the final goal is to return either an exception or the output from the
  // changes. for now not implemented

  optional<SchedulerException> retval;

  // prompt and insert new job
  retval = insert_job(get_target());

  decrement_timer();

  /*
   * short function that returns an optional instead of the iterator.
   * modification of std::find_if()
   */
  auto find_if_exists = [](auto container, auto predicate) {
    auto found = std::find_if(container.begin(), container.end(), predicate);
    if (found != container.end())
      return optional<typename decltype(container)::value_type>(*found);
    else
      return optional<typename decltype(container)::value_type>{};
  };

  // Free processors
  while (auto done_job = find_if_exists(
             running_jobs, [](auto i) { return i->n_ticks <= 0; })) {
    cout << "Job finished: " << **done_job << endl;
    free_proc(*done_job);
  }

  // find next shortest job and see if we can start it
  while (auto next_job = find_shortest())
    if (check_availability(next_job->get_n_procs())) {
      auto new_job = pop_shortest();
      cout << "Job Started: " << new_job << endl;
      run_job(new_job);
    } else
      break;

  return retval;
}

[[nodiscard]] optional<SchedulerException>
JobScheduler::insert_job(unsigned int n_procs, unsigned int n_ticks,
                         const std::string &desc) noexcept {
  // assume the input is not invalid
  assert(n_procs != 0);
  assert(n_ticks != 0);
  assert(desc != "NULL");

  // we have output handled with SchedulerException::what()

  // Check validity of the job
  if (n_procs > processors.size()) {
    return SchedulerException("Failed to Insert Job, job required more "
                              "processors than total processors.");
  } else if (n_ticks == 0) {
    return SchedulerException("Failed to insert job, job needs > 0 ticks");
  } else if (desc == "NULL") {
    return SchedulerException("No job inserted: Desc is \"NULL\"");
  }

  Job j{static_cast<unsigned int>(job_counter++), n_procs, n_ticks, desc};
  job_queue.push(j);

  // we can split job insertion and job creation into 2 functions,
  // then have this cout statement in the tick() function
  cout << "Inserted job: " << j << endl;

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

  // we could add something to show the current job it is adding where the error
  // occurred. it would likely be done in the constructor for
  // SchedulerException.
  target >> std::quoted(desc);
  if (desc == "NULL") {
    // skip to end of line
    target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return SchedulerException("No job inserted: Desc is \"NULL\"");
  }
  if (desc == "") {
    return {};
  }
  target >> n_procs;
  if (n_procs == 0) {
    // skip to end of line
    target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return SchedulerException("Failed to insert job, job needs > 0 processors");
  }
  target >> n_ticks;
  if (n_ticks == 0) {
    // skip to end of line
    target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return SchedulerException("Failed to insert job, job needs > 0 ticks");
  }
  // skip to end of line
  target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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

void JobScheduler::free_proc(const std::shared_ptr<Job> &j) noexcept {
  auto job_iter = std::find(running_jobs.begin(), running_jobs.end(), j);

  // assume j is in running_jobs
  assert(job_iter != running_jobs.end());
  // assume j is finished
  assert(j->n_ticks <= 0);

  auto n_procs = j->get_n_procs();
  for (auto i = 0U; i < n_procs; ++i) {
    *std::find(processors.begin(), processors.end(), j) = nullptr;
  }

  running_jobs.erase(job_iter);
}

[[nodiscard]] bool
JobScheduler::check_availability(unsigned int procs_needed) noexcept {
  // assume >= 1 procs_needed
  assert(procs_needed > 0);

  return procs_needed <= get_available_processors().size();
}

[[nodiscard]] optional<Job> JobScheduler::find_shortest() const noexcept {
  if (job_queue.size() == 0)
    return {};
  return job_queue.top();
}

[[nodiscard]] Job JobScheduler::pop_shortest() noexcept {
  // assume job_queue has >= 1 element
  assert(job_queue.size() > 0);
  // assume find_shortest() yields a job
  auto new_job = find_shortest();
  assert(new_job);

  Job temp = *new_job;

  job_queue.pop();
  return temp;
}

void JobScheduler::run_job(Job new_job) noexcept {
  // assign processors
  auto available_procs = get_available_processors();
  auto job = std::make_shared<Job>(new_job);
  for (auto i = 0U; i < new_job.get_n_procs(); ++i) {
    processors[available_procs[i]] = job;
  }

  // add to the vector of running jobs
  running_jobs.push_back(job);
}

void JobScheduler::decrement_timer() noexcept {
  for (auto &i : running_jobs)
    --i->n_ticks;
}
