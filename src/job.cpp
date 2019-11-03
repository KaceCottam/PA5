#include "job.hpp"

Job::Job(unsigned int id, unsigned int n_procs, unsigned int n_ticks,
         const std::string &desc)
    : n_ticks{n_ticks}, id{id}, n_procs{n_procs}, desc{desc} {}

Job::Job(const Job &copy)
    : n_ticks{copy.n_ticks}, id{copy.id}, n_procs{copy.n_procs},
      desc{copy.desc} {}

Job::Job(Job &&move)
    : n_ticks{std::move(move.n_ticks)}, id{std::move(move.id)},
      n_procs{std::move(move.n_procs)}, desc{std::move(move.desc)} {}

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

bool operator<(const Job &lhs, const Job &rhs) noexcept {
  return lhs.n_ticks < rhs.n_ticks;
}
bool operator>(const Job &lhs, const Job &rhs) noexcept {
  return lhs.n_ticks > rhs.n_ticks;
}
bool operator==(const Job &lhs, const Job &rhs) noexcept {
  return lhs.n_ticks == rhs.n_ticks;
}
bool operator>=(const Job &lhs, const Job &rhs) noexcept {
  return lhs.n_ticks >= rhs.n_ticks;
}
bool operator<=(const Job &lhs, const Job &rhs) noexcept {
  return lhs.n_ticks <= rhs.n_ticks;
}

[[nodiscard]] unsigned int Job::get_n_procs() const noexcept { return n_procs; }
