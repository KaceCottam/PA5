#include "job_scheduler.hpp"

#include <fstream>
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
int main(int argc, char *argv[]) {
  // start a job with 5 processors
  JobScheduler scheduler{5};

  if (argc == 1) {
    std::cerr << "Usage: PA5 {target file}" << std::endl;
    return EXIT_FAILURE;
  }

  scheduler.set_target(std::ifstream(argv[1]));
}
