#include "job.hpp"

Job::Job(unsigned int n_procs, unsigned int n_ticks, const JobId id,
         const Description &desc)
    : n_procs{n_procs}, n_ticks{n_ticks}, id{id}, desc{desc} {}

Job::Job(const JobId id, const Description &desc, Job &&other) {
  *this = std::move(other);
  if (id) {
    this->id = *id;
  }
  if (desc) {
    this->desc = *desc;
  }
}

Job::Job(const Job &copy)
    : n_procs{copy.n_procs}, n_ticks{copy.n_ticks}, id{copy.id},
      desc{copy.desc} {}

Job::Job(Job &&other)
    : n_procs{std::move(other.n_procs)}, n_ticks{std::move(other.n_ticks)},
      id{std::move(other.id)}, desc{std::move(other.desc)} {}

Job &Job::operator=(const Job &copy) noexcept {
  n_procs = copy.n_procs;
  n_ticks = copy.n_ticks;
  desc = copy.desc;
  id = copy.id;
  return *this;
}

Job &Job::operator=(Job &&copy) {
  *this = copy;
  copy.n_procs = 0;
  copy.n_ticks = 0;
  copy.desc = {};
  copy.id = {};
  return *this;
}

std::ostream &operator<<(std::ostream &stream, const Job &job) {
  stream << "Job";
  if (job.id) {
    stream << *job.id;
  } else {
    stream << "(U)";
  }
  stream << "{ NProcs{" << job.n_procs << "}, NTicks{" << job.n_ticks << "}";
  if (job.desc) {
    stream << ", Description{" << *job.desc << "}";
  }
  return stream << " }";
}

void Job::promptDescription(std::ostream &ostream, std::istream &istream) {
  if (!desc)
    return;
  ostream << "Please enter a description for Job";
  if (id)
    ostream << *id;
  else
    ostream << "(U)";
  ostream << ": ";

  std::string new_desc;
  std::getline(istream, new_desc);

  if (new_desc == "NULL")
    return;
  desc = std::move(new_desc);
}
