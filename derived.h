#ifndef DERIVED_H
#define DERIVED_H

#include "abstract.h"

struct Derived_t;

typedef struct Derived_t Derived_t;

#ifdef __cplusplus
extern "C" {
#endif
   
Result_codes Derived_init(Derived_t ** const, const double, const double, const double);   

void Derived_destroy(Derived_t ** const);

Result_codes virt_set_Z(Derived_t * const, const double);

double Z(const Derived_t * const);

Result_codes Derived_validate(const double, const char * const);

int Derived_number(const Derived_t * const);

#ifdef  __cplusplus
}
#endif

extern const int DERIVED;
extern const char DERIVED_CHAR;
extern const char * const derived_class_name;

#endif
