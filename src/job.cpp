#include "job.hpp"

#include <cassert>

Job::Job(
    std::size_t        id,
    std::size_t        n_procs,
    std::size_t        n_ticks,
    const std::string& desc)
    : n_ticks{n_ticks}
    , id{id}
    , n_procs{n_procs}
    , desc{desc}
{
  assert(n_procs > 0);
  assert(n_ticks > 0);
  assert(desc != "NULL");
}

Job::Job(const Job& copy)
    : n_ticks{copy.n_ticks}
    , id{copy.id}
    , n_procs{copy.n_procs}
    , desc{copy.desc}
{
}

Job::Job(Job&& move)
    : n_ticks{move.n_ticks}
    , id{move.id}
    , n_procs{move.n_procs}
    , desc{std::move(move.desc)}
{
}

Job& Job::operator=(const Job& copy) noexcept = default;

Job& Job::operator=(Job&& move)
{
  id      = move.id;
  n_procs = move.n_procs;
  n_ticks = move.n_ticks;
  desc    = std::move(move.desc);
  return *this;
}

std::ostream& operator<<(std::ostream& stream, const Job& job)
{
  return stream << "Job" << job.id << "{ NProcs{" << job.n_procs << "}, NTicks{"
                << job.n_ticks << "}, Description{" << job.desc << "} }";
}

bool operator<(const Job& lhs, const Job& rhs) noexcept
{
  return lhs.n_ticks < rhs.n_ticks;
}
bool operator>(const Job& lhs, const Job& rhs) noexcept
{
  return lhs.n_ticks > rhs.n_ticks;
}
bool operator==(const Job& lhs, const Job& rhs) noexcept
{
  return lhs.n_ticks == rhs.n_ticks;
}
bool operator>=(const Job& lhs, const Job& rhs) noexcept
{
  return lhs.n_ticks >= rhs.n_ticks;
}
bool operator<=(const Job& lhs, const Job& rhs) noexcept
{
  return lhs.n_ticks <= rhs.n_ticks;
}

[[nodiscard]] std::size_t Job::get_n_procs() const noexcept { return n_procs; }
