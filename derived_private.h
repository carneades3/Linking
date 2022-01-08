#ifndef DERIVED_PRIVATE_H
#define DERIVED_PRIVATE_H

#include "base_private.h"
#include "derived.h"

typedef double (*Z_t)(const Derived_t * const);

typedef Result_codes (*virt_set_Z_t)(const Derived_t * const, const double);

typedef void (*Derived_destroy_t) (Derived_t ** const);

typedef struct Derived_t {
   Base_t base;
   
   double * z;
   
   Z_t Z_f;
   virt_set_Z_t virt_set_Z_f;
   Derived_destroy_t derived_destroy_f;
} Derived_t;

Result_codes Derived_check(const Derived_t * const, const double n, const char * const);

bool_t Derived_pv_valid_local(const Derived_t * const object, const double n);

void Derived_destroy_local(Derived_t ** const object);

#endif
