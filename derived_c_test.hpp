#ifndef DERIVED_C_TEST_HPP
#define DERIVED_C_TEST_HPP

#include "hierarchy_test.hpp"
#include "result_codes.h"
#include "derived_test.h"

namespace tests {

Result_codes test_derived_c();

struct Derived_C_test {
private:
   static Derived_functions functions;
   static Derived_t * derived;

   static Interface_expected interface_expected;
   static Abstract_expected abstract_expected;
   static Base_expected base_expected;
   static Derived_expected derived_expected;
   
   static Interface_real interface_real;
   static Abstract_real abstract_real;
   static Base_real base_real;
   static Derived_real derived_real;
public:
   static Result_codes test_derived_linking(); 
private:
   static void load_derived_functions();
   static Result_codes main_test_derived_linking();
   
   static Result_codes test_constructor();
   
   static inline Result_codes incorrect_construct(const double x, const double y, const double z) {
      Result_codes result = functions.init(&derived, x, y, z);
      assert_many(result == INVALID_ARG, "result == ", result);
      assert_many(derived == nullptr, "derived pointer == ", derived);
      return (result == INVALID_ARG) ? OK : BAD_FUNTION_CALL;
   }

   static void test_interface(int n, char c);
   static void test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n);
   static void test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n);
   static void test_derived(int pv_n, char pv_c, double x, double pv_y, double z, double area, int n);
   static void print_assert();
};

}

#endif
