#ifndef HIERARCHY_TEST_H 
#define HIERARCHY_TEST_H

#include "result_codes.h"
#include "pair.h"

typedef struct Interface_expected {
   pair_int_str pv_number;
   pair_char_str pv_char;
} Interface_expected;
void set_interface_expected(Interface_expected * interface_expected, const int n, const char c);

typedef struct Abstract_expected {
   Interface_expected interface;
   
   pair_double_str X;
   pair_double_str pv_Y;
   pair_double_str virt_area;
   pair_int_str number;
} Abstract_expected;
void set_abstract_expected(Abstract_expected * abstract_expected, int pv_n, char pv_c, double x, double pv_y, double area, int n);

typedef struct Base_expected {
   Abstract_expected abstract;
} Base_expected;
void set_base_expected(Base_expected * base_expected, int pv_n, char pv_c, double x, double pv_y, double area, int n);

typedef struct Derived_expected {
   Base_expected base;
   pair_double_str Z;
} Derived_expected;
void set_derived_expected(Derived_expected * derived_expected, int pv_n, char pv_c, double x, double pv_y, double area, int n, double z);

typedef struct Interface_real {
   int pv_number;
   char pv_char;
} Interface_real;
void set_interface_real(Interface_real * interface_real, const int n, const char c);

typedef struct Abstract_real {
   Interface_real interface;
   double X;
   double pv_Y;
   double virt_area;
   int number;
} Abstract_real;
void set_abstract_real(Abstract_real * abstract_real, int pv_n, char pv_c, double x, double pv_y, double area, int n);

typedef struct Base_real {
   Abstract_real abstract;
} Base_real;
void set_base_real(Base_real * base_real, int pv_n, char pv_c, double x, double pv_y, double area, int n);

typedef struct Derived_real {
   Base_real base;
   double Z;
} Derived_real;
void set_derived_real(Derived_real * derived_real, int pv_n, char pv_c, double x, double pv_y, double area, int n, double z);

void interface_print_and_assert(const Interface_expected * const expected, const Interface_real * const real);
void abstract_print_and_assert(const Abstract_expected * const expected, const Abstract_real * const real);
void base_print_and_assert(const Base_expected * const expected, const Base_real * const real);
void derived_print_and_assert(const Derived_expected * const expected, const Derived_real * const real);

Result_codes check_double(Result_codes (*get)(double * const), const char * message,
                                       const double expected);
Result_codes getset_coordination(Result_codes (*set)(const double), const double set_value,
                                Result_codes (*get)(double * const), const char * message);
Result_codes incorrect_set_coordination(Result_codes (*set)(const double), const double set_value,
                                Result_codes (*get)(double * const), const char * message);

Result_codes check_char(Result_codes (*get)(char * const), const char * message, const char expected);
Result_codes check_int(Result_codes (*get)(int * const), const char * message, const int expected);


void assert_double(double (*get)(const void * const), const void * const object, const char * message,
                                       const double expected);
Result_codes getset_double(Result_codes (*set)(void * const object, const double), void * const object, 
                           const double set_value, double (*get)(const void * const), const char * message);
Result_codes incorrect_set_double(Result_codes (*set)(void * const object, const double), void * const object, 
                                  const double set_value,
                                Result_codes (*get)(double * const), const char * message);

#endif
