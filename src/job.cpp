#include "job.hpp"

Job::Job(unsigned int id, unsigned int n_procs, unsigned int n_ticks,
         const std::string &desc)
    : id{id}, n_procs{n_procs}, n_ticks{n_ticks}, desc{desc} {}

Job::Job(const Job &copy)
    : id{copy.id}, n_procs{copy.n_procs}, n_ticks{copy.n_ticks},
      desc{copy.desc} {};

Job::Job(Job &&move)
    : id{std::move(move.id)}, n_procs{std::move(move.n_procs)},
      n_ticks{std::move(move.n_ticks)}, desc{std::move(move.desc)} {}

Job &Job::operator=(const Job &copy) noexcept {
  id = copy.id;
  n_procs = copy.n_procs;
  n_ticks = copy.n_ticks;
  desc = copy.desc;
  return *this;
}

Job &Job::operator=(Job &&move) {
  id = std::move(move.id);
  n_procs = std::move(move.n_procs);
  n_ticks = std::move(move.n_ticks);
  desc = std::move(move.desc);
  return *this;
}

std::ostream &operator<<(std::ostream &stream, const Job &job) {
  return stream << "Job" << job.id << "{ NProcs{" << job.n_procs << "}, NTicks{"
                << job.n_ticks << "}, Description{" << job.desc << "} }";
}

// getters
[[nodiscard]] unsigned int Job::get_id() const noexcept { return id; }

[[nodiscard]] unsigned int Job::get_n_procs() const noexcept {}
