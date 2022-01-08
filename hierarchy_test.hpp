#ifndef HIERARCHY_TEST_HPP 
#define HIERARCHY_TEST_HPP

#include <string>
#include <iostream>

#include "print.hpp"

using std::pair;
using std::string;
using std::is_base_of;
using std::enable_if_t;
using std::cerr;

namespace tests {
   
typedef struct Interface_expected {
   void set(int n, char c) {
      pv_number.second = n;
      pv_char.second = c;
   }
   pair<string, int> pv_number = {"pv_number", 0};
   pair<string, char> pv_char = {"pv_char", '0'};
} Interface_expected;

typedef struct Abstract_expected : public Interface_expected {
   void set(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
      Interface_expected::set(pv_n, pv_c);
      X.second = x;
      pv_Y.second = pv_y;
      virt_area.second = area;
      number.second = n;
   }
   pair<string, double> X = {"x", 0};
   pair<string, double> pv_Y = {"y", 0};
   pair<string, double> virt_area = {"virt_area", 0};
   pair<string, int> number = {"number", 0};
} Abstract_expected;

typedef struct Base_expected : public Abstract_expected {
} Base_expected;

typedef struct Interface_real {
   void set(int n, char c) {
      pv_number = n;
      pv_char = c;
   }
   int pv_number;
   char pv_char;
} Interface_real;

typedef struct Abstract_real : public Interface_real {
   void set(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
      Interface_real::set(pv_n, pv_c);
      X = x;
      pv_Y = pv_y;
      virt_area = area;
      number = n;
   }
   double X;
   double pv_Y;
   double virt_area;
   int number;
} Abstract_real;

typedef struct Base_real : public Abstract_real {
} Base_real;

template <typename E, typename R, enable_if_t<is_base_of<Abstract_expected, E>::value, bool> = true,
   enable_if_t<is_base_of<Abstract_real, R>::value, bool> = true> 
void abstract_print_and_assert(const E & expected, const R & real) {
   interface_print_and_assert(expected, real);
   
   cerr << __func__ << " for classes: " << typeid(E).name() << " and " << typeid(R).name() << '\n';
   print_and_assert(real.X, expected.X.second, expected.X.first);
   print_and_assert(real.pv_Y, expected.pv_Y.second, expected.pv_Y.first);
   print_and_assert(real.virt_area, expected.virt_area.second, expected.virt_area.first);
   print_and_assert(real.number, expected.number.second, expected.number.first);
}

typedef struct Derived_expected : public Base_expected {
   void set(int pv_n, char pv_c, double x, double pv_y, double area, int n, double z) {
      Base_expected::set(pv_n, pv_c, x, pv_y, area, n);
      Z.second = z;
   }
   pair<string, double> Z = {"z", 0};
} Derived_expected;

typedef struct Derived_real : public Base_real {
   void set(int pv_n, char pv_c, double x, double pv_y, double area, int n, double z) {
      Base_real::set(pv_n, pv_c, x, pv_y, area, n);
      Z = z;
   }
   double Z;
} Derived_real;

void interface_print_and_assert(const Interface_expected & expected, const Interface_real & real);
void derived_print_and_assert(const Derived_expected & expected, const Derived_real & real);

}

#endif
