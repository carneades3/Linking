#ifndef BASE_C_TEST_HPP
#define BASE_C_TEST_HPP

#include "hierarchy_test.hpp"
#include "result_codes.h"
#include "base_test.h"

namespace tests {

Result_codes test_base_c();

struct Base_C_test {
private:
   static Base_functions functions;
   static Base_t * base;

   static Interface_expected interface_expected;
   static Abstract_expected abstract_expected;
   static Base_expected base_expected;
   
   static Interface_real interface_real;
   static Abstract_real abstract_real;
   static Base_real base_real;
public:
   static Result_codes test_base_linking(); 
private:
   static void load_base_functions();
   static Result_codes main_test_base_linking();

   static void test_interface(int n, char c);
   static void test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n);
   static void test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n);
};

}

#endif
