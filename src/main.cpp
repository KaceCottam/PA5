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
  if (argc == 2) {
    auto file = std::ifstream(argv[1]);

    JobScheduler scheduler(file, 10);

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

    JobScheduler scheduler(input, 10);

    while (scheduler.is_running())
      log_error([&] { return scheduler.tick(); });
  }
}
