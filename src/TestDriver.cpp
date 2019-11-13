#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "job_scheduler.hpp"

#define N_PROCESSORS 20

/**
 * @brief Prints an error if it exists while calling the function f
 *
 * @tparam Callable
 * @param f
 */
template<class Callable>
void log_error(Callable&& f)
{
  if (auto e = f()) std::cerr << e->what() << std::endl;
}

/**
 * @brief runs the program on a provided filename
 *
 * @param filename
 *
 * @return
 */
int do_test_with_file(const char* filename)
{
  auto file = std::ifstream(filename);

  JobScheduler scheduler(file, N_PROCESSORS);
  std::size_t  tick_num{0};

  do
    {
      log_error([&] {
        // Display current tick
        cout << "Tick Number " << tick_num++ << endl;
        auto retval = scheduler.tick();
        cout << endl;
        return retval;
      });
    }
  while (scheduler.is_running());
  return EXIT_SUCCESS;
}

/**
 * @brief get input stream from user
 *
 * @return
 */
/*
 *std::stringstream get_input_string()
 *{
 *  std::cout << "Enter a list of jobs, ending with 'EOF' or 'exit', separating
 *" "with \\n.\n"
 *            << "Currently there are " << N_PROCESSORS
 *            << " processors available." << std::endl;
 *
 *  std::stringstream input{};
 *  std::string       next_input{};
 *  while (true)
 *    {
 *      if (std::cin.peek() == '\n') input << '\n';
 *      std::cin >> next_input;
 *      if (next_input == "EOF" || next_input == "exit") break;
 *      input << next_input << " ";
 *    }
 *  std::cout << "----------" << std::endl;
 *  return input;
 *}
 */

/**
 * @brief Runs the program on stdin
 *
 * @return
 */
int do_test_with_stdin()
{
  // auto         input = get_input_string();
  JobScheduler scheduler(std::cin, N_PROCESSORS);
  std::size_t  tick_num{0};

  do
    {
      log_error([&] {
        // Display current tick
        cout << "Tick Number " << tick_num++ << endl;
        std::cout << "Enter a job, ending with 'EOF' or 'exit', separating"
                     "jobs with \\n.\n"
                  << "Currently there are "
                  << scheduler.get_available_processors()
                  << " processors available.\n"
                  << "----------" << std::endl;
        auto retval = scheduler.tick();
        cout << endl;
        return retval;
      });
    }
  while (scheduler.is_running());
  return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
  if (argc == 2) { return do_test_with_file(argv[1]); }
  else if (argc == 1)
    {
      return do_test_with_stdin();
    }
  else
    {
      std::cerr << "usage: PA5 [input file]" << std::endl;
      std::cerr << "where [input file] is optional if you want to take input "
                   "from stdin."
                << std::endl;
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
