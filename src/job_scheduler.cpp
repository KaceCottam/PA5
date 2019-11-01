#include "job_scheduler.hpp"

[[nodiscard]] optional<SchedulerException> JobScheduler::insert_job(Job &j) noexcept
{
    // Check validity of the job
    if (j.get_desc() != "NULL")
    {

    }
    else if (j.get_n_procs() > total_processors)
    {
        cout << "Failed to Insert Job Desc:" << j.get_desc() << ". Exceeded total processors" << endl;
    }
    else
    {
        j.assign_id((unsigned int)job_counter);
        job_counter++;

        job_queue.push(j);

        cout << "Job ID=" << j.get_id() << " submitted successfully" << endl;
    }

}