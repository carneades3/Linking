#ifndef BASE_TEST_HPP
#define BASE_TEST_HPP

#include "result_codes.h"
#include "base.hpp"
#include "hierarchy_test.hpp"

namespace tests {
   
using Hierarchy::Base;
   
#ifdef MANUAL_DLL_LOAD
typedef struct Manual_DLL_interface {
   Base* (*create) (const double, const double);
   void (*destroy) (Base * & );
   mutable void * handle;
   
   Manual_DLL_interface(const char * shared_library, const char * create, const char * destroy) ;
} Manual_DLL_interface;
#endif

struct Base_test {
private:
#ifdef MANUAL_DLL_LOAD
   static const Manual_DLL_interface manual_interface;
   static Base * base;
#else
   static Base base;
#endif
   static Interface_expected interface_expected;
   static Abstract_expected abstract_expected;
   static Base_expected base_expected;
   static Interface_real interface_real;
   static Abstract_real abstract_real;
   static Base_real base_real;
public:
   static Result_codes test_base_linking(); 
private:
#ifdef MANUAL_DLL_LOAD
   static Result_codes load_base();
#endif
   static void test_interface(int n, char c);
   static void test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n);
   static void test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n);
};
   
Result_codes test_base();

}

#endif
