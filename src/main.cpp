#include "job_scheduler.hpp"
#include <array>

/*
 * Test Driver
 */
int main() { //
  JobScheduler scheduler{};
  std::array<Job, 10> jobs = {Job{NProcs{8}, NTicks{10}},  //
                              Job{NProcs{2}, NTicks{1}},   //
                              Job{NProcs{12}, NTicks{12}}, //
                              Job{NProcs{10}, NTicks{2}},  //
                              Job{NProcs{5}, NTicks{8}},   //
                              Job{NProcs{4}, NTicks{2}},   //
                              Job{NProcs{4}, NTicks{6}},   //
                              Job{NProcs{2}, NTicks{5}},   //
                              Job{NProcs{4}, NTicks{3}},   //
                              Job{NProcs{6}, NTicks{2}}};

  auto insertion_iter = jobs.begin();

  do {
    if (insertion_iter != jobs.end()) {
      try {
        scheduler.insert_job(*(insertion_iter)++);
      } catch (SchedulerException &e) {
        std::cerr << e.what() << std::endl;
      }
    }
    scheduler.tick();
  } while (scheduler.is_running());
}
