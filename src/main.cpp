#include "job_scheduler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define N_PROCESSORS 20

/**
 * @brief output to cerr any errors from f
 *
 * @tparam Callable
 * @param f
 */
template<class Callable>
void
log_error(Callable&& f)
{
  if (auto e = f())
    std::cerr << e->what() << std::endl;
}

/*
 * Test Driver
 */
int
main(int argc, char* argv[])
{
  if (argc == 2) { // use a file
    auto file = std::ifstream(argv[1]);

    JobScheduler scheduler(file, N_PROCESSORS);
    unsigned int tick_num{ 0 };

    do {
      log_error([&] {
        // Display current tick
        cout << "Tick Number " << tick_num++ << endl;
        auto retval = scheduler.tick();
        cout << endl;
        return retval;
      });
    } while (scheduler.is_running());
  } else if (argc == 1) { // use stdin
    std::cout << "Enter a list of jobs, ending with EOF, separating with \\n.\n"
              << "Currently there are " << N_PROCESSORS
              << " processors available." << std::endl;

    std::stringstream input{};
    std::string next_input{};
    while (true) {
      if (std::cin.peek() == '\n')
        input << '\n';
      std::cin >> next_input;
      if (next_input == "EOF")
        break;
      input << next_input << " ";
    }
    std::cout << "----------" << std::endl;

    JobScheduler scheduler(input, N_PROCESSORS);
    unsigned int tick_num{ 0 };

    do {
      log_error([&] {
        // Display current tick
        cout << "Tick Number " << tick_num++ << endl;
        auto retval = scheduler.tick();
        cout << endl;
        return retval;
      });

    } while (scheduler.is_running());
  } else {
    std::cerr << "usage: PA5 [input file]" << std::endl;
    std::cerr << "where [input file] is optional if you want to take input "
                 "from stdin."
              << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
