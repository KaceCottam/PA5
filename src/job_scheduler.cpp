#include "job_scheduler.hpp"

#include <iomanip> // std::quoted()

[[nodiscard]] optional<SchedulerException>
JobScheduler::insert_job(unsigned int n_procs, unsigned int n_ticks,
                         const std::string &desc) noexcept {
  // assume the input is not invalid
  assert(n_procs != 0);
  assert(n_ticks != 0);
  assert(desc != "NULL");

  // Check validity of the job
  if (n_procs > total_processors)
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

Job JobScheduler::find_shortest() const
{
  return job_queue.top();
}

bool JobScheduler::check_availability(int &procs_needed)
{
  return (available_processors >= procs_needed);
}

Job &JobScheduler::delete_shortest()
{
  Job temp = job_queue.top();
  job_queue.pop();
  return temp;
}

void JobScheduler::run_job(Job &new_job)
{
  // decrement available processors
  set_available_processors(new_job.get_n_procs * -1);

  // add to the vector of running jobs
  running_jobs.push_back(new_job);
}

void JobScheduler::decrement_timer()
{
  for (int i = 0; i < running_jobs.size(); i++)
  {
    running_jobs[i].set_n_ticks(running_jobs[i].get_n_ticks() - 1);
  }
}