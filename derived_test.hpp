#ifndef DERIVED_TEST_HPP
#define DERIVED_TEST_HPP

#include "result_codes.h"
#include "derived.hpp"
#include "hierarchy_test.hpp"

namespace tests {
   
using Hierarchy::Derived;
   
#ifdef MANUAL_DLL_LOAD
typedef struct Manual_DLL_derived {
   Derived* (*create) (const double, const double, const double);
   void (*destroy) (Derived * & );
   mutable void * handle;
   
   Manual_DLL_derived(const char * shared_library, const char * create, const char * destroy);
} Manual_DLL_derived;
#endif
   
struct Derived_test {
public:   
   static Result_codes test_derived_linking(); 
private:
#ifdef MANUAL_DLL_LOAD
   static const Manual_DLL_derived manual_interface;
   static Derived * d;
#else
   static Derived d;
#endif
   
   static Interface_expected interface_expected;
   static Abstract_expected abstract_expected;
   static Base_expected base_expected;
   static Derived_expected derived_expected;
   
   static Interface_real interface_real;
   static Abstract_real abstract_real;
   static Base_real base_real;
   static Derived_real derived_real;

   static void test_interface(int n, char c);
   static void test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n);
   static void test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n);
   static void test_base_cutting(int pv_n, char pv_c, double x, double pv_y, double area, int n);
   static void test_derived(int pv_n, char pv_c, double x, double pv_y, double z, double area, int n);
   static void print_assert();
   
#ifdef MANUAL_DLL_LOAD
   static Derived * construct(const double, const double, const double);
#else
   static Derived construct(const double, const double, const double);
#endif
   
   static void test_move();
};

Result_codes test_derived();

}

#endif
