#ifndef ABSTRACT_PRIVATE_H
#define ABSTRACT_PRIVATE_H

#include "interface_private.h"
#include "abstract.h"

typedef double (*pv_Y_t) (const Abstract_t * const);
typedef double (*X_t) (const Abstract_t * const);
typedef Result_codes (*virt_set_X_t) (const Abstract_t * const, const double);
typedef double (*virt_area_t) (const Abstract_t * const);

typedef void (*Abstract_destroy_t) (Abstract_t ** const);

typedef struct Abstract_t {
   Interface_t interface;
   
   double * x;
   
   pv_Y_t pv_Y_f;
   X_t X_f;
   virt_set_X_t virt_set_X_f;
   virt_area_t virt_area_f;
   Abstract_destroy_t abstract_destroy_f;
} Abstract_t;

Result_codes Abstract_init(Abstract_t ** const, const double);

void Abstract_destroy_A(Abstract_t ** const object);

Result_codes virt_set_X_A(Abstract_t * const object, const double number);

#endif
