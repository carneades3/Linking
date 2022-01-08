#include "hierarchy_test.hpp"

namespace tests {
   
   void interface_print_and_assert(const Interface_expected & expected, const Interface_real & real) {
      print_and_assert(real.pv_number, expected.pv_number.second, expected.pv_number.first);
      print_and_assert(real.pv_char,expected.pv_char.second, expected.pv_char.first);
   }

   void derived_print_and_assert(const Derived_expected & expected, const Derived_real & real) {
      abstract_print_and_assert(expected, real);
      print_and_assert(real.Z, expected.Z.second, expected.Z.first);
   }
   
}
