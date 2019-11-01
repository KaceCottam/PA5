#include "job_scheduler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

  if (argc == 2) {
    auto file = std::ifstream(argv[1]);

    scheduler.set_target(file);

    while (scheduler.is_running())
      log_error([&] { return scheduler.tick(); });

  } else {
    std::cout << "Enter a list of jobs, ending with EOF, separating with \\n."
              << std::endl;
    std::stringstream input{};
    std::string next_input{};
    while (true) {
      std::cin >> next_input;
      if (next_input == "EOF")
        break;
      input << next_input << " ";
    }

    scheduler.set_target(input);

    while (scheduler.is_running())
      log_error([&] { return scheduler.tick(); });
  }
}
