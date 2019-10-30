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
  JobScheduler scheduler{5};
  std::array<Job, 10> jobs = {Job{8, 10},  //
                              Job{2, 1},   //
                              Job{12, 12}, //
                              Job{10, 2},  //
                              Job{5, 8},   //
                              Job{4, 2},   //
                              Job{4, 6},   //
                              Job{2, 5},   //
                              Job{4, 3},   //
                              Job{6, 2}};

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
