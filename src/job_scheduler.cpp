#include "job_scheduler.hpp"

#include <iomanip>

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

  job_queue.push(Job{(unsigned int)(job_counter++), n_procs, n_ticks, desc});
  return {};
}

[[nodiscard]] optional<SchedulerException>
JobScheduler::insert_job(std::istream &target) noexcept {
  std::string desc{};
  unsigned int n_procs, n_ticks;
  target >> std::quoted(desc) >> n_procs >> n_ticks;

  // skip to end of line
  target.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  if (desc == "NULL")
    return SchedulerException("No job inserted: Desc is \"NULL\"");
  if (n_procs == 0)
    return SchedulerException("Failed to insert job, job needs > 0 processors");
  if (n_ticks == 0)
    return SchedulerException("Failed to insert job, job needs > 0 ticks");

  return insert_job(n_procs, n_ticks, desc);
}

