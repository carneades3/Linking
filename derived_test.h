#ifndef DERIVED_TEST_H
#define DERIVED_TEST_H

#include "result_codes.h"
#include "pair.h"
#include "interface.h"
#include "derived.h"
#include "base_test.h"
#include "hierarchy_test.h"

Result_codes test_derived(void);

typedef struct 
#if defined(__clang__)
   __attribute__ ((__packed__))
#endif
Derived_functions {
   Base_functions base;
   
   Result_codes (*init)(Derived_t ** const, const double, const double, const double);
   void (*Derived_destroy)(Derived_t ** const);
   double (*Z)(const Derived_t * const);
   Result_codes (*virt_set_Z)(Derived_t * const, const double);
   int (*Derived_number)(const Derived_t * const);

} Derived_functions;

#endif
