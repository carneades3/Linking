#ifndef DERIVED_CPP_TEST_H
#define DERIVED_CPP_TEST_H

#include "result_codes.h"
#include "hierarchy_test.h"
#include "base_cpp_test.h"

typedef struct __attribute__ ((__packed__)) Derived_connector {
   Abstract_connector abstract;
   Result_codes (*init)(const double, const double, const double);
   Result_codes (*Z)(double * const  z);
   Result_codes (*virt_set_Z)(const double);
} Derived_connector;

Result_codes test_derived_cpp(void);

#endif
