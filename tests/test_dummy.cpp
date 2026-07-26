#include <catch2/catch_test_macros.hpp>
#include <printer/dummy.hpp>

TEST_CASE("add() adds two positive numbers", "[math]") {
  REQUIRE(printer::add(2, 3) == 5);
}

TEST_CASE("add() handles zero", "[math]") {
  REQUIRE(printer::add(5, 0) == 5);
  REQUIRE(printer::add(0, 5) == 5);
}

TEST_CASE("add() handles negative numbers", "[math]") {
  REQUIRE(printer::add(-2, 3) == 1);
  REQUIRE(printer::add(-2, -3) == -5);
}

TEST_CASE("add() is commutative", "[math]") {
  REQUIRE(printer::add(17, 42) == printer::add(42, 17));
}
