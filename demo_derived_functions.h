#ifndef DEMO_DERIVED_FUNCTIONS_H
#define DEMO_DERIVED_FUNCTIONS_H

#include "result_codes.h"

typedef struct Demo_derived_functions {
   Result_codes (*init)(const char * const, const unsigned int);
   Result_codes (*set_name)(const char * const);
   Result_codes (*get_name) (char ** const);
   Result_codes (*set_age)(const unsigned int);
   Result_codes (*get_age) (unsigned int * const);
   Result_codes (*destroy)(void);
   void * handle;
} Demo_derived_functions;

Result_codes test_demo_derived(void);

#endif
