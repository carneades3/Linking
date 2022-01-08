#ifndef HUMAN_FUNCTIONS_H
#define HUMAN_FUNCTIONS_H

#include "result_codes.h"
#include "human.h"

typedef struct Human_functions {
   Result_codes (*init)(Human_t ** const, const char * const);
   Result_codes (*set_name)(Human_t * const, const char * const);
   Result_codes (*get_name) (const Human_t * const, char ** const);
   void (*destroy)(Human_t ** const);
   void * handle;
} Human_functions;

Result_codes test_human_linking(void);

#endif
