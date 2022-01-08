#ifndef HUMAN_DERIVED_FUNCTIONS_H
#define HUMAN_DERIVED_FUNCTIONS_H

#include "result_codes.h"
#include "human.h"
#include "human_derived.h"

typedef struct Human_derived_functions {
   Result_codes (*init)(Human_derived_t ** const, const char * const, const unsigned int);
   Result_codes (*set_name)(Human_t * const, const char * const);
   Result_codes (*get_name) (const Human_t * const, char ** const);
   Result_codes (*set_age)(Human_derived_t * const, const unsigned int);
   Result_codes (*get_age) (const Human_derived_t * const, unsigned int * const);
   void (*destroy)(Human_derived_t ** const);
   void * handle;
} Human_derived_functions;

Result_codes test_human_derived(void);

#endif
