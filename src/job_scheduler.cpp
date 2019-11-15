#include "job_scheduler.hpp"

#include <algorithm>   // std::find_if()
#include <cassert>     // assert()
#include <functional>  // std::mem_fn
#include <iomanip>     // std::quoted()
#include <utility>     // std::forward()

SchedulerException::SchedulerException(const std::string& arg)
    : runtime_error{arg}
{
}

JobScheduler::JobScheduler(std::istream& target, std::size_t num_processors)
    : target{&target}
    , available_processors(num_processors)
{
  assert(num_processors > 0);
}

[[maybe_unused]] void JobScheduler::set_target(std::istream& target) noexcept
{
  this->target = &target;
}

[[nodiscard]] bool JobScheduler::is_running() noexcept
{
  return !exited || available_processors != max_processors;
}

[[nodiscard]] optional<SchedulerException> JobScheduler::tick() noexcept
{
  // the final goal is to return either an exception or the output from the
  // changes. for now not implemented

  optional<SchedulerException> retval{};

  // prompt and insert new job
  auto read_job_output = read_job(get_target());
  if (read_job_output.index() == 1)
    {
      // index == 1 is type of Job
      auto new_job = std::get<1>(read_job_output);
      insert_job(new_job);
      cout << "Inserted a new job: " << new_job << endl;
    }
  else if (read_job_output.index() == 0)
    {
      // index == 0 is type of SchedulerException
      retval = std::get<0>(read_job_output);
    }
  // index == 2 is type of std::nullopt_t (EOF reached)
  // do nothing in this case

  decrement_timer();

  // short function that returns an optional instead of the iterator.
  // modification of std::find_if()
  auto find_if_exists = [](
      auto container,
      auto predicate) -> optional<typename decltype(container)::value_type>
  {
    auto found = std::find_if(container.begin(), container.end(), predicate);
    if (found != container.end()) { return *found; }
    return {};
  };

  auto number_of_ticks_leq_0 = [](auto i) { return i.n_ticks <= 0; };

  // Free processors
  while (auto done_job = find_if_exists(running_jobs, number_of_ticks_leq_0))
    {
      cout << "Job finished: " << *done_job << endl;
      free_proc(*done_job);
    }

  // find next shortest job and see if we can start it
  while (auto next_job = find_shortest())
    {
      if (check_availability(*next_job))
        {
          auto new_job = pop_shortest();
          cout << "Job Started: " << new_job << endl;
          run_job(new_job);
        }
      else
        {
          break;
        }
    }

  return retval;
}

[[nodiscard]] std::variant<SchedulerException, Job> JobScheduler::create_job(
    std::size_t        n_procs,
    std::size_t        n_ticks,
    const std::string& desc) noexcept
{
  // assume the input is valid
  assert(n_procs != 0);
  assert(n_ticks != 0);
  assert(desc != "NULL");

  // Check validity of the job in the
  if (n_procs > max_processors)
    {
      return SchedulerException("Failed to create Job, job required more "
                                "processors than total processors.");
    }

  return Job{job_counter++, n_procs, n_ticks, desc};
}

void JobScheduler::insert_job(const Job& new_job) noexcept
{
  // assume the input is valid
  assert(new_job.get_n_procs() <= max_processors);

  job_queue.push(new_job);
}

[[nodiscard]] std::variant<SchedulerException, Job, std::nullopt_t>
    JobScheduler::read_job(std::istream& target) noexcept
{
  // in case end of stream
  if (target.eof()) { exited = true; }
  if (exited) { return std::nullopt; }

  std::string desc{};
  std::size_t n_procs, n_ticks;

  // we could add something to show the current job it is adding where the
  // error occurred. it would likely be done in the constructor for
  // SchedulerException.
  target >> std::quoted(desc);
  if (desc == "NULL")
    {
      // skip to end of line
      target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return SchedulerException("No job read: Desc is \"NULL\"");
    }
  if (desc.empty())
    {
      // skip to end of line
      target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return SchedulerException("No job read: Description is empty");
    }
  if (desc == "exit" || desc == "EOF")
    {
      exited = true;
      return std::nullopt;
    }
  target >> n_procs;
  if (n_procs == 0)
    {
      // skip to end of line
      target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return SchedulerException(
          "Failed to read valid job, job needs > 0 processors");
    }
  target >> n_ticks;
  if (n_ticks == 0)
    {
      // skip to end of line
      target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return SchedulerException(
          "Failed to read valid job, job needs > 0 ticks");
    }
  // skip to end of line
  target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  auto job = create_job(n_procs, n_ticks, desc);
  if (job.index() == 0) { return std::get<0>(job); }
  return std::get<1>(job);
}

[[nodiscard]] std::istream& JobScheduler::get_target() const noexcept
{
  assert(target != nullptr);
  return *target;
}

[[nodiscard]] std::size_t JobScheduler::get_available_processors() const
    noexcept
{
  return available_processors;
}

void JobScheduler::free_proc(const Job& j) noexcept
{
  auto job_iter = std::find(running_jobs.begin(), running_jobs.end(), j);

  // assume j is in running_jobs
  assert(job_iter != running_jobs.end());
  // assume j is finished
  assert(j.n_ticks <= 0);

  available_processors += j.get_n_procs();

  running_jobs.erase(job_iter);
}

[[nodiscard]] bool JobScheduler::check_availability(const Job& j) noexcept
{
  // assume >= 1 procs_needed
  assert(j.get_n_procs() > 0);

  return j.get_n_procs() <= get_available_processors();
}

[[nodiscard]] optional<Job> JobScheduler::find_shortest() const noexcept
{
  if (job_queue.empty()) { return {}; }
  return job_queue.top();
}

[[nodiscard]] Job JobScheduler::pop_shortest() noexcept
{
  // assume job_queue has >= 1 element
  assert(!job_queue.empty());
  // assume find_shortest() yields a job
  auto new_job = find_shortest();
  assert(new_job);

  Job temp = *new_job;

  job_queue.pop();
  return temp;
}

void JobScheduler::run_job(const Job& new_job) noexcept
{
  // assign processors
  available_processors -= new_job.get_n_procs();

  // add to the vector of running jobs
  running_jobs.push_back(new_job);
}

void JobScheduler::decrement_timer() noexcept
{
  for (auto& i : running_jobs) { --i.n_ticks; }
}
