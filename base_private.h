#ifndef BASE_PRIVATE_H
#define BASE_PRIVATE_H

#include "abstract_private.h"
#include "base.h"

typedef Result_codes (*virt_set_Y_t)(const Base_t * const, const double);

typedef void (*Base_destroy_t) (Base_t ** const);

typedef struct Base_t {
   Abstract_t abstract;
   
   double * y;
   
   virt_set_Y_t virt_set_Y_f;
   Base_destroy_t base_destroy_f;
} Base_t;

Result_codes Base_check(const Base_t * const, const double n, const char * const);

bool_t Base_pv_valid_B(const Base_t * const object, const double n);

void Base_destroy_B(Base_t ** const object);

Result_codes virt_set_Y_B(Base_t * const object, const double number);

double virt_area_B(const Base_t * const object);

#endif
