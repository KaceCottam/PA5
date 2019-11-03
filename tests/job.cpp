#include <job.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("jobs can be compared") {

  auto alpha_ticks = GENERATE(take(100, random(0, 50000)));
  auto beta_ticks = GENERATE(take(100, random(50001, 100000)));
  Job alpha(0, 0, alpha_ticks, "");
  Job beta(1, 0, beta_ticks, "");
  REQUIRE(alpha < beta);
}
