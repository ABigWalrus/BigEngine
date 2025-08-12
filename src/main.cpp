// #include "./util.hpp"
#include "./component.hpp"
#include "./entity.hpp"
#include "glm/fwd.hpp"
// #include <catch2/catch_test_macros.hpp>
#include <iostream>

// unsigned int Factorial(unsigned int number) {
//   return number <= 1 ? number : Factorial(number - 1) * number;
// }
//
// TEST_CASE("Factorials are computed", "[factorial]") {
//   REQUIRE(Factorial(1) == 1);
//   REQUIRE(Factorial(2) == 2);
//   REQUIRE(Factorial(3) == 6);
//   REQUIRE(Factorial(10) == 3628800);
// }

int main() {
  // Entity ent = spawn_entity();
  Component cmp = spawn_component();
  Component cmp2 = spawn_component();
  // ent.add_component(cmp);
  // ent.add_component(cmp2);
  // Entity ent2 = spawn_entity();
  // ent.add_child(ent2);
  // ent.translate(glm::vec3{1, 1, 1});
  // std::cout << ent << "\n";
  // std::cout << cmp << "\n";
}
