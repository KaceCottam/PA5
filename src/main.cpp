#include "job_scheduler.hpp"

#include <array>
#include <iostream>

/**
 * @brief output to cerr any errors from f
 *
 * @tparam Callable
 * @param f
 */
template <class Callable> void log_error(Callable &&f) {
  if (auto e = f())
    std::cerr << e->what() << std::endl;
}

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

      insertion_iter->promptDescription();

      log_error([&] { return scheduler.insert_next_job(*insertion_iter); });

      ++insertion_iter;
    }

    log_error([&] { return scheduler.tick(); });
  } while (scheduler.is_running());
}
