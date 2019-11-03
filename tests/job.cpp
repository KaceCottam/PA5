#include <job.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <sstream>

TEST_CASE("jobs can be compared and accessed") {

  auto alpha_ticks = GENERATE(take(100, random(0, 50000)));
  auto beta_ticks = GENERATE(take(100, random(50001, 100000)));
  Job alpha(0, 1, alpha_ticks, "");
  Job beta(1, 15, beta_ticks, "");
  REQUIRE(alpha < beta);
  REQUIRE(alpha <= beta);
  REQUIRE(beta > alpha);
  REQUIRE(beta >= alpha);

  Job delta(2, 2, alpha_ticks, "");
  REQUIRE(alpha <= delta);
  REQUIRE(alpha >= delta);
  REQUIRE(alpha == delta);

  REQUIRE(alpha.get_n_procs() == 1);
  REQUIRE(beta.get_n_procs() == 15);
  REQUIRE(delta.get_n_procs() == 2);
}
TEST_CASE("jobs can output to ostreams") {
  Job epsilon(3, 7, 34, "epsilon");

  std::stringstream output{};
  REQUIRE(output.str() == "");

  output << epsilon;
  REQUIRE(output.str() ==
          "Job3{ NProcs{7}, NTicks{34}, Description{epsilon} }");
}
