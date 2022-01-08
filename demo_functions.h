#ifndef DEMO_FUNCTIONS_H
#define DEMO_FUNCTIONS_H

#include "result_codes.h"

typedef struct Demo_functions {
   Result_codes (*init)(const char * );
   Result_codes (*set_name)(const char * );
   Result_codes (*get_name) (char ** );
   Result_codes (*destroy)(void);
   void * handle;
} Demo_functions; 

Result_codes test_demo_linking(void);

#endif
