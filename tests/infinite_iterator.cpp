#include <infinite_iterator.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Infinite iterators can be initialized at any number.") {
  SECTION("Init @ 0 (default)") {
    infinite_iterator alpha{};
    REQUIRE(*alpha == 0);
    REQUIRE((int)alpha == 0);
  }
  SECTION("Init @ 1") {
    infinite_iterator alpha{1};
    REQUIRE(*alpha == 1);
    REQUIRE((int)alpha == 1);
  }
  SECTION("Init @ -1") {
    infinite_iterator alpha{-1};
    REQUIRE(*alpha == -1);
    REQUIRE((int)alpha == -1);
  }
  SECTION("Init @ random number") {
    auto initial = GENERATE(take(100, random(-100000, 100000)));
    infinite_iterator alpha{initial};
    REQUIRE(*alpha == initial);
    REQUIRE((int)alpha == initial);
  }
}

TEST_CASE("Infinite iterators can be incremented") {
  auto initial = GENERATE(take(100, random(-1000000, 1000000)));
  infinite_iterator alpha{initial};

  SECTION("Increment once") {
    SECTION("Prefix ++") {
      ++alpha;
      REQUIRE(*alpha == initial + 1);
      REQUIRE((int)alpha == initial + 1);
    }
    SECTION("Postfix ++") {
      auto beta = alpha++;
      REQUIRE(*beta == initial);
      REQUIRE((int)beta == initial);
      REQUIRE(*alpha == initial + 1);
      REQUIRE((int)alpha == initial + 1);
    }
  }
  SECTION("Increment any number of times with ++ operator") {
    auto times_to_increment = GENERATE(take(100, random(0, 1000)));
    for (auto i = 0; i < times_to_increment; ++i)
      ++alpha;
    REQUIRE(*alpha == initial + times_to_increment);
    REQUIRE((int)alpha == initial + times_to_increment);
  }

  SECTION("Increment 5 with += operator") {
    alpha += 5;
    REQUIRE((int)alpha == initial + 5);
  }

  SECTION("Increment 5 with -= operator") {
    alpha -= -5;
    REQUIRE((int)alpha == initial + 5);
  }
}
TEST_CASE("Infinite iterators can be decremented") {
  auto initial = GENERATE(take(100, random(-1000000, 1000000)));
  infinite_iterator alpha{initial};

  SECTION("Decrement once") {
    SECTION("Prefix --") {
      --alpha;
      REQUIRE(*alpha == initial - 1);
      REQUIRE((int)alpha == initial - 1);
    }
    SECTION("Postfix --") {
      auto beta = alpha--;
      REQUIRE(*beta == initial);
      REQUIRE((int)beta == initial);
      REQUIRE(*alpha == initial - 1);
      REQUIRE((int)alpha == initial - 1);
    }
  }
  SECTION("Decrement any number of times with -- operator") {
    auto times_to_decrement = GENERATE(take(100, random(0, 1000)));
    for (auto i = 0; i < times_to_decrement; ++i)
      --alpha;
    REQUIRE(*alpha == initial - times_to_decrement);
    REQUIRE((int)alpha == initial - times_to_decrement);
  }
  SECTION("Decrement 5 with += operator") {
    alpha -= 5;
    REQUIRE((int)alpha == initial - 5);
  }

  SECTION("Decrement 5 with -= operator") {
    alpha += -5;
    REQUIRE((int)alpha == initial - 5);
  }
}
TEMPLATE_TEST_CASE("Infinite iterators can be many types", "", int, float,
                   double, long) {
  auto initial = GENERATE(take(100, random(-1000000, 1000000)));
  infinite_iterator<TestType> alpha{(TestType)initial};

  REQUIRE((TestType)alpha == (TestType)initial);
  REQUIRE((TestType)(++alpha) == (TestType)initial + 1);
  REQUIRE((TestType)(alpha -= 2) == (TestType)initial - 1);
}
TEST_CASE("Infinite iterators can be copied and moved") {
  auto initial = GENERATE(take(100, random(-1000000, 1000000)));
  infinite_iterator alpha{initial};

  SECTION("...by operator") {
    infinite_iterator beta{};

    REQUIRE((int)alpha == initial);
    REQUIRE((int)beta == 0);

    SECTION("Copy") {
      beta = alpha;
      REQUIRE((int)alpha == initial);
      REQUIRE((int)beta == initial);
    }
    SECTION("Move") {
      beta = std::move(alpha);
      REQUIRE((int)beta == std::move(initial));
      REQUIRE((int)alpha == initial);
    }
  }
  SECTION("...by constructor") {
    SECTION("Copy") {
      infinite_iterator beta{alpha};
      REQUIRE((int)alpha == initial);
      REQUIRE((int)beta == initial);
    }
    SECTION("Move") {
      infinite_iterator beta{std::move(alpha)};
      REQUIRE((int)beta == std::move(initial));
      REQUIRE((int)alpha == initial);
    }
  }
}
