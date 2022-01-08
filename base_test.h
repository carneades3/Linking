#ifndef BASE_TEST_H
#define BASE_TEST_H

#include "result_codes.h"
#include "pair.h"
#include "interface.h"
#include "base.h"
#include "hierarchy_test.h"

Result_codes test_base(void);

typedef struct 
#if defined(__clang__)
   __attribute__ ((__packed__))
#endif
Interface_functions {
   void (*Interface_destroy)(Interface_t ** const);
   int (*pv_number)(const Interface_t * const);
   char (*pv_char)(const Interface_t * const);
} Interface_functions;

typedef struct 
#if defined(__clang__)
   __attribute__ ((__packed__))
#endif
Abstract_functions {
   Interface_functions interface;
   
   void (*Abstract_destroy)(Abstract_t ** const);
   double (*X)(const Abstract_t * const);
   Result_codes (*virt_set_X)(Abstract_t * const, const double);
   double (*pv_Y)(const Abstract_t * const);
   double (*virt_area)(const Abstract_t * const);
   int (*Abstract_number)(const Abstract_t * const);
} Abstract_functions;

typedef struct 
#if defined(__clang__)
   __attribute__ ((__packed__))
#endif
Base_functions {
   Abstract_functions abstract;
   
   Result_codes (*init)(Base_t ** const, const double, const double);
   void (*Base_destroy)(Base_t ** const);
   Result_codes (*virt_set_Y)(Base_t * const, const double);
   int (*Base_number)(const Base_t * const);
   
   void * handle;
} Base_functions;

#endif
